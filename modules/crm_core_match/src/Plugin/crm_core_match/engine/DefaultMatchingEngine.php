<?php

/**
 * @file
 * Contains \Drupal\crm_core_match\Plugin\crm_core_match\engine\DefaultMatchingEngine.
 */

namespace Drupal\crm_core_match\Plugin\crm_core_match\engine;

use Drupal\Component\Plugin\PluginManagerInterface;
use Drupal\Core\Entity\EntityManagerInterface;
use Drupal\Core\Form\FormStateInterface;
use Drupal\crm_core_contact\ContactInterface;
use Drupal\crm_core_match\Plugin\crm_core_match\field\FieldHandlerInterface;
use Symfony\Component\DependencyInjection\ContainerInterface;

/**
 * DefaultMatchingEngine class.
 *
 * Extends CrmCoreMatchEngine to provide rules for identifying duplicate
 * contacts.
 *
 * @CrmCoreMatchEngine(
 *   id = "default",
 *   label = @Translation("Default Matching Engine"),
 *   summary = @Translation("This is a simple matching engine from CRM Core. Allows administrators to specify matching rules for individual contact types on a field-by-field basis."),
 * )
 */
class DefaultMatchingEngine extends MatchEngineBase {

  const MATCH_CHARS_DEFAULT = 3;

  /**
   * The match field plugin manager.
   *
   * @var \Drupal\Component\Plugin\PluginManagerInterface
   */
  protected $pluginManager;

  /**
   * The entity manager service.
   *
   * @var \Drupal\Core\Entity\EntityManagerInterface
   */
  protected $entityManager;

  /**
   * Constructs a default matching engine.
   */
  public function __construct($configuration, $id, $definition, PluginManagerInterface $plugin_manager, EntityManagerInterface $entity_manager) {
    parent::__construct($configuration, $id, $definition);
    $this->pluginManager = $plugin_manager;
    $this->entityManager = $entity_manager;
  }

  /**
   * {@inheritdoc}
   */
  public static function create(ContainerInterface $container, array $configuration, $plugin_id, $plugin_definition) {
    return new static(
      $configuration,
      $plugin_id,
      $plugin_definition,
      $container->get('plugin.manager.crm_core_match.match_field'),
      $container->get('entity.manager')
    );
  }

  /**
   * {@inheritdoc}
   */
  public function match(ContactInterface $contact) {
    $ids = array();

    $fields = $contact->getFieldDefinitions();
    $results = array();
    $configuration_rules = $this->getConfigurationItem('rules') ?: [];
    foreach ($configuration_rules as $name => $rules) {
      if (isset($fields[$name])) {
        $rules['field'] = $fields[$name];

        if (!$this->pluginManager->hasDefinition($rules['field']->getType())) {
          continue;
        }

        /* @var \Drupal\crm_core_match\Plugin\crm_core_match\field\FieldHandlerInterface $field_handler */
        $field_handler = $this->pluginManager->createInstance($rules['field']->getType(), $rules);

        foreach ($field_handler->getPropertyNames() as $name) {
          $results += $field_handler->match($contact, $name);
        }
      }
    }
    foreach ($results as $id => $rule_matches) {
      $total_score = array_sum($rule_matches);
      if ($total_score >= $this->getConfigurationItem('threshold')) {
        $ids[] = $id;
      }
    }

    return $ids;
  }

  /**
   * {@inheritdoc}
   */
  public function buildConfigurationForm(array $form, FormStateInterface $form_state) {
    $form = parent::buildConfigurationForm($form, $form_state);

    $form['threshold'] = array(
      '#type' => 'textfield',
      '#title' => $this->t('Threshold'),
      '#description' => $this->t('Defines the score at which a contact is considered a match.'),
      '#maxlength' => 28,
      '#size' => 28,
      '#required' => TRUE,
      '#default_value' => $this->getConfigurationItem('threshold'),
    );

    $return_description = $this->t(<<<EOF
If two or more contact records result in matches with identical scores, CRM Core
will give preference to one over the other base on selected option.
EOF
    );
    $form['return_order'] = array(
      '#type' => 'select',
      '#title' => $this->t('Return Order'),
      '#description' => $return_description,
      '#default_value' => $this->getConfigurationItem('return_order'),
      '#options' => array(
        'created' => $this->t('Most recently created'),
        'updated' => $this->t('Most recently updated'),
        'associated' => $this->t('Associated with user'),
      ),
    );

    $strict_description = $this->t(<<<EOF
Check this box to return a match for this contact type the first time one is
identified that meets the threshold. Stops redundant processing.
EOF
    );
    $form['strict'] = array(
      '#type' => 'checkbox',
      '#title' => $this->t('Strict matching'),
      '#description' => $strict_description,
      '#default_value' => $this->getConfigurationItem('strict'),
    );

    $form['field_matching'] = array(
      '#type' => 'item',
      '#title' => $this->t('Field Matching'),
    );

    $form['rules'] = array(
      '#type' => 'table',
      '#tree' => TRUE,
      '#header' => $this->buildHeader(),
      '#empty' => $this->t('There are no fields available.'),
      '#tabledrag' => array(
        array(
          'action' => 'order',
          'relationship' => 'sibling',
          'group' => 'weight',
        ),
      ),
    );

    // @todo: Display fields per bundle.
    $contact_types = $this->entityManager->getStorage('crm_core_contact_type')->loadMultiple();
    $fields = [];
    foreach ($contact_types as $contact_type_id => $value) {
      $fields += $this->entityManager->getFieldDefinitions('crm_core_contact', $contact_type_id);
    }
    foreach ($fields as $field) {

      $rules = $this->getConfigurationItem('rules');
      $config = empty($rules[$field->getName()]) ? array() : $rules[$field->getName()];
      $config['field'] = $field;

      $match_field_id = 'unsupported';
      if ($this->pluginManager->hasDefinition($field->getType())) {
        $match_field_id = $field->getType();
      }

      /* @var \Drupal\crm_core_match\Plugin\crm_core_match\field\FieldHandlerInterface $match_field */
      $match_field = $this->pluginManager->createInstance($match_field_id, $config);

      $disabled = ($match_field_id == 'unsupported');

      foreach ($match_field->getPropertyNames($field) as $name) {
        $row = $this->buildRow($match_field, $name, $disabled);
        $form['rules'][$field->getName() . ':' . $name] = $row;
      }
    }

    return $form;
  }

  /**
   * Builds the header row for the rule listing.
   *
   * @return array
   *   A render array structure of header strings.
   */
  public function buildHeader() {
    $header = array();

    $header['status'] = $this->t('Enabled');
    $header['label'] = $this->t('Name');
    $header['field_type'] = $this->t('Field type');
    $header['operator'] = $this->t('Operator');
    $header['options'] = $this->t('Options');
    $header['score'] = $this->t('Score');
    $header['weight'] = $this->t('Weight');

    return $header;
  }

  /**
   * Builds a row for an rule in the rule listing.
   *
   * @param \Drupal\crm_core_match\Plugin\crm_core_match\field\FieldHandlerInterface $field
   *   The match field of this rule.
   * @param string $name
   *   The property name of this rule.
   * @param bool $disabled
   *   Disables the form elements.
   *
   * @return array
   *   A render array structure of fields for this rule.
   */
  public function buildRow(FieldHandlerInterface $field, $name, $disabled) {
    $row = array();
    $row['#attributes']['class'][] = 'draggable';
    $row['#weight'] = $field->getWeight($name);

    $row['status'] = array(
      '#type' => 'checkbox',
      '#default_value' => $field->getStatus($name),
      '#disabled' => $disabled,
    );

    $row['label'] = array(
      '#markup' => $field->getLabel($name),
    );

    $row['type'] = array(
      '#markup' => $field->getType(),
    );

    $row['operator'] = array(
      '#type' => 'select',
      '#default_value' => $field->getOperator($name),
      '#empty_option' => !$disabled ? NULL : $this->t('- Please Select -'),
      '#options' => $field->getOperators($name),
      '#disabled' => $disabled,
    );

    $row['options'] = array(
      '#type' => 'textfield',
      '#maxlength' => 28,
      '#size' => 28,
      '#default_value' => $field->getOptions($name),
      '#disabled' => $disabled,
    );

    $row['score'] = array(
      '#type' => 'textfield',
      '#maxlength' => 4,
      '#size' => 3,
      '#default_value' => $field->getScore($name),
      '#disabled' => $disabled,
    );

    $row['weight'] = array(
      '#type' => 'weight',
      '#title' => $this->t('Weight for @field', array(
        '@field' => $field->getLabel(),
      )),
      '#title_display' => 'invisible',
      '#default_value' => $field->getWeight($name),
      '#attributes' => array('class' => array('weight')),
    );

    return $row;
  }

  /**
   * {@inheritdoc}
   */
  public function validateConfigurationForm(array &$form, FormStateInterface $form_state) {
    parent::validateConfigurationForm($form, $form_state);

    if (!is_numeric($form_state->getValue(['configuration', 'threshold']))) {
      $form_state->setErrorByName('configuration[threshold]', $this->t('Threshold must be a number.'));
    }
    $rules = $form_state->getValue(['configuration', 'rules']);
    foreach ($rules as $field_name => $config) {
      if ($config['status'] && empty($config['operator'])) {
        $name = 'rules][' . $field_name . '][operator';
        $message = $this->t('You must select an operator for enabled field.');
        $form_state->setErrorByName($name, $message);
      }
      if (!is_numeric($config['score'])) {
        $name = 'rules][' . $field_name . '][score';
        $message = $this->t('You must enter number in "Score" column.');
        $form_state->setErrorByName($name, $message);
      }
    }
  }

  /**
   * {@inheritdoc}
   */
  public function submitConfigurationForm(array &$form, FormStateInterface $form_state) {
    // @todo: Build the same form and configuration structure.
    $rules = [];

    $this->configuration = $form_state->getValue('configuration');
    foreach ($form_state->getValue(['configuration', 'rules']) as $name => $config) {
      if (strpos($name, ':') !== FALSE) {
        list($parent, $child) = explode(':', $name, 2);
        $rules[$parent][$child] = $config;
      }
      else {
        $rules[$name] = $config;
      }
    }
    $this->configuration['rules'] = $rules;
  }

  /**
   * {@inheritdoc}
   */
  public function defaultConfiguration() {
    // TODO: Implement defaultConfiguration() method.
  }

  /**
   * {@inheritdoc}
   */
  public function calculateDependencies() {
    // TODO: Implement calculateDependencies() method.
    return [];
  }

}

<?php
/**
 * @file
 * Contains \Drupal\crm_core_default_matching_engine\Form\MatchingRuleForm.
 */

namespace Drupal\crm_core_default_matching_engine\Form;

use Drupal\Core\Entity\EntityForm;
use Drupal\Core\Entity\EntityInterface;
use Drupal\Core\Entity\EntityManagerInterface;
use Drupal\Core\Form\FormStateInterface;
use Drupal\crm_core_default_matching_engine\Plugin\crm_core_match\field\FieldHandlerInterface;
use Symfony\Component\DependencyInjection\ContainerInterface;

class MatchingRuleForm extends EntityForm {

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
   * Constructs a new form for the matching config rule entity.
   *
   * @param \Drupal\Core\Entity\EntityManagerInterface $plugin_manager
   *   The plugin manager for match fields.
   * @param \Drupal\Core\Entity\EntityManagerInterface $entity_manager
   *   The entity manager service.
   */
  public function __construct(EntityManagerInterface $plugin_manager, EntityManagerInterface $entity_manager) {
    $this->pluginManager = $plugin_manager;
    $this->entityManager = $entity_manager;
  }

  /**
   * {@inheritdoc}
   */
  public static function create(ContainerInterface $container) {
    return new static(
      $container->get('plugin.manager.crm_core_match.match_field'),
      $container->get('entity.manager')
    );
  }

  /**
   * {@inheritdoc}
   */
  public function form(array $form, FormStateInterface $form_state) {
    $form = parent::form($form, $form_state);

    $form['status'] = array(
      '#type' => 'checkbox',
      '#title' => $this->t('Enable matching for this contact type'),
      '#description' => $this->t('Check this box to allow CRM Core to check for duplicate contact records for this contact type.'),
      '#default_value' => $this->entity->status(),
    );

    $form['threshold'] = array(
      '#type' => 'textfield',
      '#title' => $this->t('Threshold'),
      '#description' => $this->t('Defines the score at which a contact is considered a match.'),
      '#maxlength' => 28,
      '#size' => 28,
      '#required' => TRUE,
      '#default_value' => $this->entity->threshold,
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
      '#default_value' => $this->entity->return_order,
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
      '#default_value' => $this->entity->strict,
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

    $fields = $this->entityManager->getFieldDefinitions('crm_core_contact', $this->entity->id());
    foreach ($fields as $field) {

      $config = empty($this->entity->rules[$field->getName()]) ? array() : $this->entity->rules[$field->getName()];
      $config['field'] = $field;

      $match_field_id = 'unsupported';
      if ($this->pluginManager->hasDefinition($field->getType())) {
        $match_field_id = $field->getType();
      }

      /* @var \Drupal\crm_core_default_matching_engine\Plugin\crm_core_match\field\FieldHandlerInterface $match_field */
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
   * @param \Drupal\crm_core_default_matching_engine\Plugin\crm_core_match\field\FieldHandlerInterface $field
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
      '#empty_option' => $this->t('-- Please Select --'),
      '#empty_value' => NULL,
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
  public function validate(array $form, FormStateInterface $form_state) {
    parent::validate($form, $form_state);

    $rules = array();
    if (isset($form_state->getValue('rules'))) {
      $rules = $form_state->getValue('rules');
    }
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
  public function save(array $form, FormStateInterface $form_state) {
    $this->entity->save();
    drupal_set_message($this->t('The configuration options have been saved.'));
  }

  /**
   * {@inheritdoc}
   */
  protected function copyFormValuesToEntity(EntityInterface $entity, array $form, FormStateInterface $form_state) {
    foreach ($form_state->getValues() as $key => $value) {
      switch ($key) {
        case 'rules':
          $rules = array();
          foreach ($value as $name => $config) {
            if (strpos($name, ':') !== FALSE) {
              list($parent,$child) = explode(':', $name, 2);
              $rules[$parent][$child] = $config;
            }
            else {
              $rules[$name] = $config;
            }
          }
          $entity->rules = $rules;
          break;

        case 'status':
          $entity->setStatus($value);
          break;

        default:
          $entity->set($key, $value);
      }
    }
  }
}

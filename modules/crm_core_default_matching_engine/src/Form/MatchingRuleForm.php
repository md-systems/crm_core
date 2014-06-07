<?php
/**
 * @file
 * Contains \Drupal\crm_core_default_matching_engine\Form\MatchingRuleForm.
 */

namespace Drupal\crm_core_default_matching_engine\Form;

use Drupal\Core\Entity\EntityForm;
use Drupal\Core\Entity\EntityInterface;
use Drupal\crm_core_default_matching_engine\Plugin\MatchFieldPluginManager;
use Symfony\Component\DependencyInjection\ContainerInterface;

class MatchingRuleForm extends EntityForm {

  /**
   * The match field plugin manager.
   *
   * @var \Drupal\crm_core_default_matching_engine\Plugin\MatchFieldPluginManager.
   */
  protected $matchFieldManager;

  /**
   * Constructs a new form for the matching config rule entity.
   *
   * @param MatchFieldPluginManager $match_field_manager
   *   The plugin manager for match fields.
   */
  public function __construct(MatchFieldPluginManager $match_field_manager) {
    $this->matchFieldManager = $match_field_manager;
  }

  /**
   * {@inheritdoc}
   */
  public static function create(ContainerInterface $container) {
    return new static(
      $container->get('plugin.manager.crm_core_match.match_field')
    );
  }

  /**
   * {@inheritdoc}
   */
  public function form(array $form, array &$form_state) {
    $form = parent::form($form, $form_state);

    $form['status'] = array(
      '#type' => 'checkbox',
      '#title' => $this->t('Enable matching for this contact type'),
      '#description' => $this->t('Check this box to allow CRM Core to check for duplicate contact records for this contact type.'),
      '#default_value' => $this->entity->status,
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

    $form['fields'] = array(
      '#type' => 'container',
      '#tree' => TRUE,
    );

    $instances = \Drupal::entityManager()->getFieldDefinitions('crm_core_contact', $this->entity->id());
    foreach ($instances as $instance) {
      $config = empty($this->entity->fields[$instance->getName()]) ? array() : $this->entity->fields[$instance->getName()];

      $match_field_id = 'unsupported';
      if ($this->matchFieldManager->hasDefinition($instance->getType())) {
        $match_field_id = $instance->getType();
      }

      /* @var \Drupal\crm_core_default_matching_engine\Plugin\MatchFieldInterface $match_field */
      $match_field = $this->matchFieldManager->createInstance($match_field_id, $config);
      $form['fields'] += $match_field->fieldRender($instance);
    }

    return $form;
  }

  /**
   * {@inheritdoc}
   */
  public function validate(array $form, array &$form_state) {
    parent::validate($form, $form_state);

    $fields_rules = array();
    if (isset($form_state['values']['fields'])) {
      $fields_rules = $form_state['values']['fields'];
    }
    foreach ($fields_rules as $field_name => $config) {
      if ($config['status'] && empty($config['operator'])) {
        $name = 'field_matching][' . $field_name . '][operator';
        $message = $this->t('You must select an operator for enabled field.');
        $this->setFormError($name, $message);
      }
      if (!is_numeric($config['score'])) {
        $name = 'field_matching][' . $field_name . '][score';
        $message = $this->t('You must enter number in "Score" column.');
        $this->setFormError($name, $message);
      }
    }
  }

  /**
   * {@inheritdoc}
   */
  public function save(array $form, array &$form_state) {
    $this->entity->save();
    drupal_set_message($this->t('The configuration options have been saved.'));
  }
}

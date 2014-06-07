<?php

/**
 * @file
 * Contains \Drupal\crm_core_default_matching_engine\Plugin\crm_core\MatchField\MatchFieldBase.
 */

namespace Drupal\crm_core_default_matching_engine\Plugin\crm_core\MatchField;

use Drupal\Core\Field\FieldDefinitionInterface;
use Drupal\Core\Plugin\ContainerFactoryPluginInterface;
use Symfony\Component\DependencyInjection\ContainerInterface;

abstract class MatchFieldBase implements MatchFieldInterface, ContainerFactoryPluginInterface {

  const WEIGHT_DELTA = 25;

  /**
   * The plugin id.
   *
   * @var string
   */
  protected $id;

  /**
   * The plugin definition.
   *
   * @var array
   */
  protected $definition;

  /**
   * The configuration.
   *
   * @var array
   */
  protected $configuration;

  /**
   * Constructs an plugin instance.
   */
  public function __construct($configuration, $id, $definition) {
    $this->configuration = $configuration + array(
      'weight' => self::WEIGHT_DELTA,
      'status' => FALSE,
      'operator' => NULL,
      'options' => '',
      'score' => 0,
    );
    $this->definition = $definition;
    $this->id = $id;
  }

  /**
   * {@inheritdoc}
   */
  public static function create(ContainerInterface $container, array $configuration, $plugin_id, $plugin_definition) {
    return new static(
      $configuration,
      $plugin_id,
      $plugin_definition
    );
  }


  /**
   * {@inheritdoc}
   */
  public function fieldRender(FieldDefinitionInterface $field) {
    $form = array();

    $field_name = $field->getName();
    $field_label = $field->getLabel();

    if ($this->configuration['weight'] == 0) {
      // Table row positioned incorrectly if "#weight" is 0.
      $display_weight = 0.001;
    }
    else {
      $display_weight = $this->configuration['weight'];
    }

    $form[$field_name]['#weight'] = $display_weight;

    $form[$field_name]['supported'] = array(
      '#type' => 'value',
      '#value' => TRUE,
    );

    $form[$field_name]['field_type'] = array(
      '#type' => 'value',
      '#value' => $field->getType(),
    );

    $form[$field_name]['field_name'] = array(
      '#type' => 'value',
      '#value' => $field_name,
    );

    $form[$field_name]['field_item'] = array(
      '#type' => 'value',
      '#value' => '',
    );

    $form[$field_name]['status'] = array(
      '#type' => 'checkbox',
      '#default_value' => $this->configuration['status'],
    );

    $form[$field_name]['name'] = array(
      '#type' => 'item',
      '#markup' => check_plain($field_label),
    );

    $form[$field_name]['field_type_markup'] = array(
      '#type' => 'item',
      '#markup' => $field->getType(),
    );

    $operator = array(
      '#type' => 'select',
      '#default_value' => $this->configuration['operator'],
      '#empty_option' => t('-- Please Select --'),
      '#empty_value' => '',
    );
    switch ($field->getType()) {
      case 'date':
      case 'datestamp':
      case 'datetime':
        $operator += array(
          '#options' => $this->operators($field),
        );
        break;

      default:
        $operator += array(
          '#options' => $this->operators(),
        );
    }

    $form[$field_name]['operator'] = $operator;

    $form[$field_name]['options'] = array(
      '#type' => 'textfield',
      '#maxlength' => 28,
      '#size' => 28,
      '#default_value' => $this->configuration['options'],
    );

    $form[$field_name]['score'] = array(
      '#type' => 'textfield',
      '#maxlength' => 4,
      '#size' => 3,
      '#default_value' => $this->configuration['score'],
    );

    $form[$field_name]['weight'] = array(
      '#type' => 'weight',
      '#default_value' => $this->configuration['weight'],
      '#attributes' => array(
        'class' => array('crm-core-match-engine-order-weight'),
      ),
      '#delta' => self::WEIGHT_DELTA,
    );

    return $form;
  }

  /**
   * Field query to search matches.
   *
   * @param object $contact
   *   CRM Core contact entity.
   * @param object $rule
   *   Matching rule object.
   *
   * @return array
   *   Founded matches.
   */
  public function fieldQuery($contact, $rule) {

    $results = array();
    $contact_wrapper = entity_metadata_wrapper('crm_core_contact', $contact);
    $needle = '';
    $field_item = '';

    if (empty($rule->field_item)) {
      $needle = $contact_wrapper->{$rule->field_name}->value();
      $field_item = 'value';
    }
    else {
      $field_value = $contact_wrapper->{$rule->field_name}->value();
      if (isset($field_value)) {
        $needle = $contact_wrapper->{$rule->field_name}->{$rule->field_item}->value();
        $field_item = $rule->field_item;
      }
    }

    if (!empty($needle)) {
      $query = new EntityFieldQuery();
      $query->entityCondition('entity_type', 'crm_core_contact')
        ->entityCondition('bundle', $contact->type);
      $query->entityCondition('entity_id', $contact->contact_id, '<>');

      switch ($rule->operator) {
        case 'equals':
          $query->fieldCondition($rule->field_name, $field_item, $needle);
          break;

        case 'starts':
          $needle = db_like(substr($needle, 0, MATCH_DEFAULT_CHARS)) . '%';
          $query->fieldCondition($rule->field_name, $field_item, $needle, 'LIKE');
          break;

        case 'ends':
          $needle = '%' . db_like(substr($needle, -1, MATCH_DEFAULT_CHARS));
          $query->fieldCondition($rule->field_name, $field_item, $needle, 'LIKE');
          break;

        case 'contains':
          $needle = '%' . db_like($needle) . '%';
          $query->fieldCondition($rule->field_name, $field_item, $needle, 'LIKE');
          break;
      }
      $results = $query->execute();
    }

    return isset($results['crm_core_contact']) ? array_keys($results['crm_core_contact']) : $results;
  }

  /**
   * Each field handler MUST implement this method.
   *
   * Must return associative array of supported operators for current field.
   * By default now supports only this keys: 'equals', 'starts', 'ends',
   * 'contains'. In case you need additional operators you must implement
   * this method and MatchFieldInterface::fieldQuery.
   *
   * @return array
   *   Assoc array, keys must be
   */
  public function operators() {
  }
}

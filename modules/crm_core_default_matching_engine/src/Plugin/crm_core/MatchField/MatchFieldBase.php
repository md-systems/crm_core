<?php

/**
 * @file
 * Contains \Drupal\crm_core_default_matching_engine\Plugin\crm_core\MatchField\MatchFieldBase.
 */

namespace Drupal\crm_core_default_matching_engine\Plugin\crm_core\MatchField;

use Drupal\Core\Field\FieldDefinitionInterface;
use Drupal\Core\Plugin\ContainerFactoryPluginInterface;
use Drupal\crm_core_contact\Entity\Contact;
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
   * The field.
   *
   * @var \Drupal\Core\Field\FieldDefinitionInterface
   */
  protected $field;

  /**
   * Constructs an plugin instance.
   */
  public function __construct(FieldDefinitionInterface $field, array $configuration, $id, $definition) {
    $this->configuration = $configuration;
    $this->definition = $definition;
    $this->id = $id;
    $this->field = $field;
  }

  /**
   * {@inheritdoc}
   */
  public static function create(ContainerInterface $container, array $configuration, $plugin_id, $plugin_definition) {
    $field = $configuration['field'];
    unset($configuration['field']);
    return new static(
      $field,
      $configuration,
      $plugin_id,
      $plugin_definition
    );
  }

  /**
   * {@inheritdoc}
   */
  public function getPropertyNames() {
    return array('value');
  }

  /**
   * {@inheritdoc}
   */
  public function getLabel($property = 'value') {
    return $this->field->getLabel();
  }

  /**
   * {@inheritdoc}
   */
  public function getStatus($property = 'value') {
    return isset($this->configuration[$property]['status']) ? $this->configuration[$property]['status'] : FALSE;
  }

  /**
   * {@inheritdoc}
   */
  public function getType() {
    return $this->field->getType();
  }

  /**
   * {@inheritdoc}
   */
  public function getOperator($property = 'value') {
    return isset($this->configuration[$property]['operator']) ? $this->configuration[$property]['operator'] : NULL;
  }

  /**
   * {@inheritdoc}
   */
  public function getOptions($property = 'value') {
    return isset($this->configuration[$property]['options']) ? $this->configuration[$property]['options'] : '';
  }

  /**
   * {@inheritdoc}
   */
  public function getScore($property = 'value') {
    return isset($this->configuration[$property]['score']) ? $this->configuration[$property]['score'] : 0;
  }

  /**
   * {@inheritdoc}
   */
  public function getWeight($property = 'value') {
    return isset($this->configuration[$property]['weight']) ? $this->configuration[$property]['weight'] : 0;
  }

  /**
   * {@inheritdoc}
   *
   * @todo Update to new query API.
   */
  public function match(Contact $contact, $property = 'value') {

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
          $needle = db_like(substr($needle, 0, DefaultMatchingEngine::MATCH_CHARS_DEFAULT)) . '%';
          $query->fieldCondition($rule->field_name, $field_item, $needle, 'LIKE');
          break;

        case 'ends':
          $needle = '%' . db_like(substr($needle, -1, DefaultMatchingEngine::MATCH_CHARS_DEFAULT));
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
}

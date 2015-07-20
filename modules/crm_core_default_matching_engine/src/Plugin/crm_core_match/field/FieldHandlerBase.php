<?php

/**
 * @file
 * Contains \Drupal\crm_core_default_matching_engine\Plugin\crm_core_match\field\FieldHandlerBase.
 */

namespace Drupal\crm_core_default_matching_engine\Plugin\crm_core_match\field;

use Drupal\Core\Entity\Query\QueryInterface;
use Drupal\Core\Field\FieldDefinitionInterface;
use Drupal\Core\Plugin\ContainerFactoryPluginInterface;
use Drupal\crm_core_contact\ContactInterface;
use Drupal\crm_core_contact\Entity\Contact;
use Drupal\crm_core_default_matching_engine\Plugin\crm_core_match\engine\DefaultMatchingEngine;
use Drupal\field\FieldConfigInterface;
use Symfony\Component\DependencyInjection\ContainerInterface;

abstract class FieldHandlerBase implements FieldHandlerInterface, ContainerFactoryPluginInterface {

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
   * A Contact query object.
   *
   * @var \Drupal\Core\Entity\Query\QueryInterface
   */
  protected $query;

  /**
   * Constructs an plugin instance.
   */
  public function __construct(FieldDefinitionInterface $field, QueryInterface $query, array $configuration, $id, $definition) {
    $this->configuration = $configuration;
    $this->definition = $definition;
    $this->id = $id;
    $this->field = $field;
    $this->query = $query;
  }

  /**
   * {@inheritdoc}
   */
  public static function create(ContainerInterface $container, array $configuration, $plugin_id, $plugin_definition) {
    $field = $configuration['field'];
    unset($configuration['field']);
    return new static(
      $field,
      $container->get('entity.query')->get('crm_core_contact', 'AND'),
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
   */
  public function match(ContactInterface $contact, $property = 'value') {

    $ids = array();

    $field = $this->field->getName();
    $needle = $contact->get($field)->{$property};

    if (!empty($needle)) {
      $this->query->condition('type', $contact->bundle());
      if ($contact->id()) {
        $this->query->condition('contact_id', $contact->id(), '<>');
      }

      if ($field instanceof FieldConfigInterface) {
        $field .= '.' . $property;
      }
      $this->query->condition($field, $needle, $this->getOperator($property));
      $ids = $this->query->execute();
    }

    // Get the score for this field/propery.
    $score = array(
      $this->field->getName() . '.' . $property => $this->getScore($property),
    );
    // Returning an array holding the score as value and the contact id as key.
    return array_fill_keys($ids, $score);
  }
}

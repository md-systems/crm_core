<?php

/**
 * @file
 * Contains \Drupal\crm_core_match\Plugin\crm_core_match\field\FieldHandlerBase.
 */

namespace Drupal\crm_core_match\Plugin\crm_core_match\field;

use Drupal\Core\Entity\Query\QueryFactory;
use Drupal\Core\Field\FieldDefinitionInterface;
use Drupal\Core\Plugin\ContainerFactoryPluginInterface;
use Drupal\crm_core_contact\ContactInterface;
use Drupal\field\FieldConfigInterface;
use Symfony\Component\DependencyInjection\ContainerInterface;

/**
 * Class FieldHandlerBase.
 *
 * @package Drupal\crm_core_match\Plugin\crm_core_match\field
 */
abstract class FieldHandlerBase implements FieldHandlerInterface, ContainerFactoryPluginInterface {

  /**
   * The weight.
   *
   * @var integer
   */
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
   * The settings.
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
   * @var \Drupal\Core\Entity\Query\QueryFactory
   */
  protected $queryFactory;

  /**
   * Constructs an plugin instance.
   */
  public function __construct(FieldDefinitionInterface $field, QueryFactory $query_factory, array $configuration, $id, $definition) {
    $this->configuration = $configuration;
    $this->definition = $definition;
    $this->id = $id;
    $this->field = $field;
    $this->queryFactory = $query_factory;
  }

  /**
   * {@inheritdoc}
   */
  public static function create(ContainerInterface $container, array $configuration, $plugin_id, $plugin_definition) {
    $field = $configuration['field'];
    unset($configuration['field']);
    return new static(
      $field,
      $container->get('entity.query'),
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
    $query = $this->queryFactory->get('crm_core_contact', 'AND');

    if (!empty($needle)) {
      $query->condition('type', $contact->bundle());
      if ($contact->id()) {
        $query->condition('contact_id', $contact->id(), '<>');
      }

      if ($field instanceof FieldConfigInterface) {
        $field .= '.' . $property;
      }
      $query->condition($field, $needle, $this->getOperator($property));
      $ids = $query->execute();
    }

    // Get the score for this field/propery.
    $score = array(
      $this->field->getName() . '.' . $property => $this->getScore($property),
    );
    // Returning an array holding the score as value and the contact id as key.
    return array_fill_keys($ids, $score);
  }

}

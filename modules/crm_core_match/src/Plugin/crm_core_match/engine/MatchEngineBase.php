<?php

/**
 * @file
 * Contains \Drupal\crm_core_match\Plugin\crm_core_match\engine\MatchEngineBase.
 */

namespace Drupal\crm_core_match\Plugin\crm_core_match\engine;

use Drupal\Core\Form\FormStateInterface;
use Drupal\Core\Plugin\ContainerFactoryPluginInterface;
use Drupal\Core\Plugin\PluginBase;
use Drupal\crm_core_contact\ContactInterface;
use Drupal\crm_core_default_matching_engine\Plugin\crm_core_match\field\FieldHandlerInterface;
use Symfony\Component\DependencyInjection\ContainerInterface;

/**
 * Default implementation of MatchEngineInterface.
 *
 * Safe for use by most matching engines.
 */
abstract class MatchEngineBase extends PluginBase implements MatchEngineInterface, ContainerFactoryPluginInterface {

  /**
   * The engine configuration.
   *
   * @var array
   */
  protected $configuration;

  /**
   * The plugin_id.
   *
   * @var string
   */
  protected $pluginId;

  /**
   * The plugin implementation definition.
   *
   * @var array
   */
  protected $pluginDefinition;

  /**
   * Constructs an plugin instance.
   */
  public function __construct($configuration, $plugin_id, $plugin_definition) {
    $this->configuration = $configuration;
    $this->pluginDefinition = $plugin_definition;
    $this->pluginId = $plugin_id;
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
  public function getPluginId() {
    return $this->pluginId;
  }

  /**
   * {@inheritdoc}
   */
  public function getPluginDefinition() {
    return $this->pluginDefinition;
  }

  /**
   * {@inheritdoc}
   */
  public function buildConfigurationForm(array $form, FormStateInterface $form_state) {
    return $form;
  }

  /**
   * {@inheritdoc}
   */
  public function validateConfigurationForm(array &$form, FormStateInterface $form_state) {
    // Do nothing.
  }

  /**
   * {@inheritdoc}
   */
  public function submitConfigurationForm(array &$form, FormStateInterface $form_state) {
    // Do nothing.
  }

  /**
   * Builds the header row for the rule listing.
   *
   * @return array
   *   A render array structure of header strings.
   */
  abstract public function buildHeader();

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
  abstract public function buildRow(FieldHandlerInterface $field, $name, $disabled);

  /**
   * Applies logical rules for identifying matches in the database.
   *
   * Any matching engine should implement this to apply it's unique matching
   * logic.
   *
   * @see MatchEngineInterface::match()
   */
  abstract public function match(ContactInterface $contact);

}

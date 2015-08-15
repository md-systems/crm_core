<?php

/**
 * @file
 * Contains \Drupal\crm_core_match\Plugin\crm_core_match\engine\MatchEngineBase.
 */

namespace Drupal\crm_core_match\Plugin\crm_core_match\engine;

use Drupal\Component\Utility\NestedArray;
use Drupal\Core\Form\FormStateInterface;
use Drupal\Core\Plugin\ContainerFactoryPluginInterface;
use Drupal\Core\Plugin\PluginBase;
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
   * The plugin id.
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
  public function getConfiguration() {
    return $this->configuration;
  }

  /**
   * {@inheritdoc}
   */
  public function setConfiguration(array $configuration) {
    $this->configuration = $configuration;
  }

  /**
   * {@inheritdoc}
   */
  public function getConfigurationItem($key) {
    $configuration = $this->getConfiguration();
    return NestedArray::getValue($configuration, (array) $key);
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
   * {@inheritdoc}
   */
  public function getRules() {
    return array();
  }

}

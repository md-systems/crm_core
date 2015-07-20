<?php

/**
 * @file
 * Contains \Drupal\crm_core_match\Matcher.
 */

namespace Drupal\crm_core_match;

use Drupal\Component\Plugin\PluginManagerInterface;
use Drupal\Core\Config\Config;
use Drupal\crm_core_contact\ContactInterface;
use Drupal\crm_core_contact\Entity\Contact;
use Drupal\crm_core_match\Plugin\crm_core_match\engine\MatchEngineInterface;

class Matcher implements MatcherInterface {

  /**
   * The engine plugin manager.
   *
   * @var \Drupal\Component\Plugin\PluginManagerInterface
   */
  protected $pluginManager;

  /**
   * The configuration object.
   *
   * @var \Drupal\Core\Config\Config
   */
  protected $config;

  /**
   * Array of all registered match engines, keyed by ID.
   *
   * @var \Drupal\crm_core_match\Plugin\crm_core_match\engine\MatchEngineInterface[]
   */
  protected $engines;

  /**
   * Array of all engines and their priority.
   *
   * @var array
   */
  protected $engineOrders = array();

  /**
   * Sorted list of registered engines.
   *
   * @var \Drupal\crm_core_match\Plugin\crm_core_match\engine\MatchEngineInterface[]
   */
  protected $sortedEngines;

  /**
   * Constructs a matcher instance.
   *
   * @param \Drupal\Component\Plugin\PluginManagerInterface $plugin_manager
   *   The plugin manager used to discover engines.
   * @param \Drupal\Core\Config\Config $config
   *   The configuration object.
   */
  public function __construct(PluginManagerInterface $plugin_manager, Config $config) {
    $this->pluginManager = $plugin_manager;
    $this->config = $config;
  }

  /**
   * Discovers the engines and creates instances of the active ones.
   *
   * @todo Consider to load only the enabled engines instead of skipping them.
   */
  protected function loadEngines() {
    $engine_configs = $this->config->get('engines');
    $engine_definitions = $this->pluginManager->getDefinitions();
    foreach ($engine_definitions as $id => $definition) {
      // Skip disable engines.
      if ($engine_configs && !$engine_configs->get($id . '.status')) {
        continue;
      }
      $engine = $this->pluginManager->createInstance($id, $definition);
      // @todo Check if priority was overwritten.
      $this->addMatchEngine($id, $engine, $definition['priority']);
    }
  }

  /**
   * Adds a match engine to the array of registered engines.
   *
   * @param string $engine_id
   *   Identifier of the match engine.
   * @param \Drupal\crm_core_match\Plugin\crm_core_match\engine\MatchEngineInterface $engine
   *   The engine object.
   * @param int $priority
   *   The engines priority.
   */
  protected function addMatchEngine($engine_id, MatchEngineInterface $engine, $priority = 0) {
    $this->engines[$engine_id] = $engine;
    $this->engineOrders[$priority][$engine_id] = $engine;
    // Force the builders to be re-sorted.
    $this->sortedEngines = NULL;
  }

  /**
   * {@inheritdoc}
   */
  public function getEngines() {
    if (!isset($this->sortedEngines)) {
      $this->loadEngines();
      // Sort the builders according to priority.
      krsort($this->engineOrders);
      // Merge nested engines from $this->engines into $this->sortedEngines.
      $this->sortedEngines = array();
      foreach ($this->engineOrders as $engines) {
        $this->sortedEngines = array_merge($this->sortedEngines, $engines);
      }
    }
    return $this->sortedEngines;
  }

  /**
   * Finds matches for given contact.
   *
   * Loops over all registered match engines and returns the aggregated matches.
   *
   * @param \Drupal\crm_core_contact\Entity\Contact $contact
   *   A contact entity used to pass data for identifying a match.
   *
   * @return int[]
   *   An array of entity ids for potential matches.
   *
   * @todo Check engine status. Skip disabled engines.
   */
  public function match(ContactInterface $contact) {
    $ids = array();

    foreach ($this->getEngines() as $engine) {
      $ids = array_merge($ids, $engine->match($contact));
    }

    return array_unique($ids);
  }
}

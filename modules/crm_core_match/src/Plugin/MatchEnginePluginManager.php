<?php

/**
 * @file
 * Contains \Drupal\crm_core_match\Plugin\MatchEnginePluginManager.
 */

namespace Drupal\crm_core_match\Plugin;

use Drupal\Core\Cache\CacheBackendInterface;
use Drupal\Core\Extension\ModuleHandlerInterface;
use Drupal\Core\Language\LanguageManager;
use Drupal\Core\Plugin\DefaultPluginManager;

/**
 * Manages Match Field plugins.
 *
 * @todo: Add alter hook glue code.
 */
class MatchEnginePluginManager extends DefaultPluginManager {

  /**
   * Constructs a MatchEnginePluginManager object.
   *
   * @param \Traversable $namespaces
   *   An object that implements \Traversable which contains the root paths
   *   keyed by the corresponding namespace to look for plugin implementations.
   * @param \Drupal\Core\Cache\CacheBackendInterface $cache_backend
   *   Cache backend instance to use.
   * @param \Drupal\Core\Language\LanguageManager $language_manager
   *   The language manager.
   * @param \Drupal\Core\Extension\ModuleHandlerInterface $module_handler
   *   The module handler.
   */
  public function __construct(\Traversable $namespaces, CacheBackendInterface $cache_backend, LanguageManager $language_manager, ModuleHandlerInterface $module_handler) {
    parent::__construct("Plugin/crm_core_match/engine", $namespaces, $module_handler, 'Drupal\crm_core_match\Annotation\CrmCoreMatchEngine');
    $this->setCacheBackend($cache_backend, 'crm_core_match_engine_plugins');
  }

}

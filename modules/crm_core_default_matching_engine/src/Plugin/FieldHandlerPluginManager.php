<?php

/**
 * @file
 * Contains \Drupal\crm_core_default_matching_engine\Plugin\FieldHandlerPluginManager.
 */

namespace Drupal\crm_core_default_matching_engine\Plugin;

use Drupal\Core\Cache\CacheBackendInterface;
use Drupal\Core\Extension\ModuleHandlerInterface;
use Drupal\Core\Language\LanguageManager;
use Drupal\Core\Plugin\DefaultPluginManager;

/**
 * Manages Match Field plugins.
 *
 * @todo: Add alter hook glue code.
 */
class FieldHandlerPluginManager extends DefaultPluginManager {

  /**
   * Constructs a FieldHandlerPluginManager object.
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
    parent::__construct("Plugin/crm_core_match/field", $namespaces, $module_handler, 'Drupal\crm_core_default_matching_engine\Annotation\CrmCoreMatchFieldHandler');
    $this->setCacheBackend($cache_backend, 'crm_core_default_matching_engine_match_field_plugins');
  }

}

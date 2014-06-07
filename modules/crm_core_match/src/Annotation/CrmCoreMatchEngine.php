<?php

/**
 * @file
 * Contains \Drupal\crm_core_match\Annotation\CrmCoreMatchEngine.
 */

namespace Drupal\crm_core_match\Annotation;

use Drupal\Component\Annotation\Plugin;

/**
 * Defines a Plugin annotation object for match field plugins.
 *
 * @Annotation
 */
class CrmCoreMatchEngine extends Plugin {

  /**
   * The engine ID.
   *
   * @var string
   */
  public $name;

  /**
   * The engines label.
   *
   * @var string
   *
   * @ingroup plugin_translatable
   */
  public $label;

  /**
   * The default priority for this engined.
   *
   * This can be overridden.
   *
   * @var int
   */
  public $priority;

  /**
   * An array listing settings pages for the matching engine.
   *
   * The keys
   * @var array
   *
   * Example structure:
   * @code
   * $settings = array(
   *   'settings' => array(
   *     'route' => 'crm_core_match.example', // The route identifier.
   *     'label' => t('Settings page'), // Translated label for link.
   *   ),
   * );
   * @endcode
   */
  public $settings;
}

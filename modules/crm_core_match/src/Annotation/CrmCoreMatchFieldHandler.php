<?php

/**
 * @file
 * Contains \Drupal\crm_core_match\Annotation\CrmCoreMatchFieldHandler.
 */

namespace Drupal\crm_core_match\Annotation;

use Drupal\Component\Annotation\Plugin;

/**
 * Defines a Plugin annotation object for match field plugins.
 *
 * @Annotation
 */
class CrmCoreMatchFieldHandler extends Plugin {

  /**
   * The plugin ID.
   *
   * @var string
   */
  public $field;
}

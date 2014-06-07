<?php

/**
 * @file
 * Contains \Drupal\crm_core_default_matching_engine\Annotation\MatchField.
 */

namespace Drupal\crm_core_default_matching_engine\Annotation;

use Drupal\Component\Annotation\Plugin;

/**
 * Defines a Plugin annotation object for match field plugins.
 *
 * @Annotation
 */
class MatchField extends Plugin {

  /**
   * The plugin ID.
   *
   * @var string
   */
  public $field;
}

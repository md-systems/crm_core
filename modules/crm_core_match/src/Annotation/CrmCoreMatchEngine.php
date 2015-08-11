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
  protected $id;

  /**
   * The engines label.
   *
   * @var \Drupal\Core\StringTranslation\TranslationWrapper
   *
   * @ingroup plugin_translatable
   */
  protected $label;

  /**
   * The match engine summary.
   *
   * @var \Drupal\Core\StringTranslation\TranslationWrapper
   */
  protected $summary;


}

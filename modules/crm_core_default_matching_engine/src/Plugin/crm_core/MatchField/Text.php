<?php

/**
 * @file
 * Contains \Drupal\crm_core_default_matching_engine\Plugin\crm_core\MatchField\Text.
 */

namespace Drupal\crm_core_default_matching_engine\Plugin\crm_core\MatchField;

/**
 * Class for evaluating text fields.
 *
 * @MatchField (
 *   id = "text"
 * )
 */
class Text extends MatchFieldBase {

  /**
   * {@inheritdoc}
   */
  public function operators() {
    return array(
      'equals' => t('Equals'),
      'starts' => t('Starts with'),
      'ends' => t('Ends with'),
      'contains' => t('Contains'),
    );
  }
}

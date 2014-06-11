<?php

/**
 * @file
 * Contains \Drupal\crm_core_default_matching_engine\Plugin\crm_core_match\field\Text.
 */

namespace Drupal\crm_core_default_matching_engine\Plugin\crm_core_match\field;

/**
 * Class for evaluating text fields.
 *
 * @CrmCoreMatchFieldHandler (
 *   id = "text"
 * )
 */
class Text extends FieldHandlerBase {

  /**
   * {@inheritdoc}
   */
  public function getOperators($property = 'value') {
    return array(
      'equals' => t('Equals'),
      'starts' => t('Starts with'),
      'ends' => t('Ends with'),
      'contains' => t('Contains'),
    );
  }
}

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
      '=' => t('Equals'),
      'STARTS_WITH' => t('Starts with'),
      'ENDS_WITH' => t('Ends with'),
      'CONTAINS' => t('Contains'),
    );
  }
}

<?php

/**
 * @file
 * Implementation of FieldHandlerInterface for select fields.
 */

namespace Drupal\crm_core_match\Plugin\crm_core_match\field;

/**
 * Class for handling select fields.
 */
class SelectMatchField extends FieldHandlerBase {

  /**
   * Defines logical operators to use with this field.
   *
   * This operators would be interpreted in fieldQuery() method.
   *
   * @return array
   *   Assoc array of operators.
   */
  public function operators() {
    return array(
      'equals' => t('Equals'),
    );
  }
}

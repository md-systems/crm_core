<?php

/**
 * @file
 * Implementation of DefaultMatchingEngineFieldTypeInterface for select fields.
 */

/**
 * Class for handling select fields.
 */
class SelectMatchField extends DefaultMatchingEngineFieldType {

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

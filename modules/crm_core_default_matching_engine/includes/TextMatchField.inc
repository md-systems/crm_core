<?php

/**
 * @file
 * Implementation of DefaultMatchingEngineFieldTypeInterface for text fields.
 */

/**
 * Class for evaluating text fields.
 */
class TextMatchField extends DefaultMatchingEngineFieldType {

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
      'starts' => t('Starts with'),
      'ends' => t('Ends with'),
      'contains' => t('Contains'),
    );
  }
}

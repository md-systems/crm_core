<?php

/**
 * @file
 * Implementation of DefaultMatchingEngineFieldTypeInterface for text field.
 */

/**
 * Class for evaluating text fields.
 */
class textMatchField extends DefaultMatchingEngineFieldType {

  /**
   * Constructs the UI for configuring matching on text fields.
   *
   * @see CrmCoreMatchField::fieldRender()
   */
  public function fieldRender($field, $field_info, &$form) {
    $this->fieldRenderTemplate($field, $field_info, $form);
  }

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

  public function fieldQuery($contact, $rule) {
  }
}

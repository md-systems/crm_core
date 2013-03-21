<?php

/**
 * @file
 * Implementation of DefaultMatchingEngineFieldTypeInterface for email field.
 */

/**
 * Class for evaluating email fields.
 */
class emailMatchField extends DefaultMatchingEngineFieldType {

  /**
   * Constructs the UI for configuring matching on email fields.
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
    );
  }

  public function fieldQuery($contact, $rule) {
  }
}

<?php

/**
 * @file
 * Implementation of DefaultMatchingEngineFieldTypeInterface for select field.
 */

/**
 * Class for handling select fields.
 */
class selectMatchField extends DefaultMatchingEngineFieldType {

  /**
   * Handles matching for select lists with pre-determined values.
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

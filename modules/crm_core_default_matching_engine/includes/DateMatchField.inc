<?php

/**
 * @file
 * Implementation of DefaultMatchingEngineFieldTypeInterface for date fields.
 */

/**
 * Class for evaluating date fields.
 */
class DateMatchField extends DefaultMatchingEngineFieldType {

  /**
   * Constructs the UI for configuring matching on date fields.
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
   * @param array $field_info
   *   Array returned by field_info_field($field_name).
   *
   * @return array
   *   Assoc array of operators.
   */
  public function operators($field_info = NULL) {

    $operators = array(
      'equals' => t('Equals'),
    );

    if (isset($field_info['columns']['value2'])) {
      $operators['between'] = t('Between');
    }

    return $operators;
  }

  public function fieldQuery($contact, $rule) {
  }
}

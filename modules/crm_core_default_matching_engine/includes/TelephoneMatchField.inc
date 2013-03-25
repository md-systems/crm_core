<?php

/**
 * @file
 * Implementation of DefaultMatchingEngineFieldTypeInterface for telephone field.
 */

/**
 * Class for evaluating name fields.
 */
class TelephoneMatchField extends DefaultMatchingEngineFieldType {

  /**
   * @see DefaultMatchingEngineFieldType::fieldRender()
   */
  public function fieldRender($field, $field_info, &$form) {
    $item = new selectMatchField();
    $item->fieldRender($field, $field_info, $form);
  }
}

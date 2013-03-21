<?php

/**
 * @file
 * Implementation of DefaultMatchingEngineFieldTypeInterface for name field.
 */

/**
 * Class for evaluating name fields.
 */
class nameMatchField extends DefaultMatchingEngineFieldType {

  /**
   * This function is going to add a number of fields based on what the name field is configured to display.
   *
   * The name field uses text and select fields to set values we will need to pass information into other field handlers
   * to get the right records to pass back.
   *
   * @see DefaultMatchingEngineFieldType::fieldRenderTemplate()
   */
  public function fieldRender($field, $field_info, &$form) {
    _crm_core_default_matching_engine_name_field_helper($field, $field_info, $form, __FUNCTION__);
  }

  public function operators() {
  }

  public function fieldQuery() {
  }
}

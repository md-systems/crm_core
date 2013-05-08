<?php

/**
 * @file
 * Implementation of DefaultMatchingEngineFieldTypeInterface for name fields.
 */

/**
 * Class for evaluating name fields.
 */
class NameMatchField extends DefaultMatchingEngineFieldType {

  /**
   * This function is going to add a number of fields based on what the name field is configured to display.
   *
   * The name field uses text and select fields to set values we will need to pass information into other field handlers
   * to get the right records to pass back.
   *
   * @see DefaultMatchingEngineFieldType::fieldRender()
   */
  public function fieldRender($field, $field_info, &$form) {
    foreach ($field_info['columns'] as $item => $info) {
      if ($field['settings']['inline_css'][$item] != 'display:none') {
        $field_item['field_name'] = $field['field_name'];
        $field_item['label'] = $field['label'] . ': ' . $field_info['settings']['labels'][$item];
        $field_item['bundle'] = $field['bundle'];
        $field_item['field_item'] = $item;

        if (isset($field['settings'][$item . '_field']) && $field['settings'][$item . '_field'] == 'select') {
          $item = new selectMatchField();
          $item->fieldRender($field_item, $field_info, $form);
        }
        else {
          $item = new textMatchField();
          $item->fieldRender($field_item, $field_info, $form);
        }
      }
    }
  }
}

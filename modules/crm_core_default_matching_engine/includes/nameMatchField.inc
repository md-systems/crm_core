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

  public function operators() {
  }

  public function fieldQuery($contact, $rule) {
    $contact_wrapper = entity_metadata_wrapper('crm_core_contact', $contact);
    $needle = $contact_wrapper->{$rule->field_name}->{$rule->field_item}->value();

    $query = new EntityFieldQuery();
    $query->entityCondition('entity_type', 'crm_core_contact')->entityCondition('bundle', $contact->type);

    switch ($rule->operator) {
      case 'equals':
        $query->fieldCondition($rule->field_name, $rule->field_item, $needle);
        break;

      case 'starts':
        $needle = db_like(substr($needle, 0, MATCH_DEFAULT_CHARS)) . '%';
        $query->fieldCondition($rule->field_name, $rule->field_item, $needle, 'LIKE');
        break;

      case 'ends':
        $needle = '%' . db_like(substr($needle, -1, MATCH_DEFAULT_CHARS));
        $query->fieldCondition($rule->field_name, $rule->field_item, $needle, 'LIKE');
        break;

      case 'contains':
        $needle = '%' . db_like($needle) . '%';
        $query->fieldCondition($rule->field_name, $rule->field_item, $needle, 'LIKE');
        break;
    }

    $results = $query->execute();

    return array_keys($results['crm_core_contact']);
  }
}

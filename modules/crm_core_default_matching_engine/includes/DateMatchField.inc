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
    dpm($contact, 'Contact');

    $contact_wrapper = entity_metadata_wrapper('crm_core_contact', $contact);
    $items = field_get_items('crm_core_contact', $contact, $rule->field_name);
    dpm($rule, 'Date field value');

    $needle = '';
    $field_item = '';
    if (empty($rule->field_item)) {
      $needle = $contact_wrapper->{$rule->field_name}->value();
      $field_item = 'value';
    }
    else {
      $needle = $contact_wrapper->{$rule->field_name}->{$rule->field_item}->value();
      $field_item = $rule->field_item;
    }

    $query = new EntityFieldQuery();
    $query->entityCondition('entity_type', 'crm_core_contact')->entityCondition('bundle', $contact->type);
    $query->entityCondition('entity_id', $contact->contact_id, '<>');

    switch ($rule->operator) {
      case 'equals':
        dpm($needle);
        $query->fieldCondition($rule->field_name, $field_item, $needle);
        break;

      case 'starts':
        $needle = db_like(substr($needle, 0, MATCH_DEFAULT_CHARS)) . '%';
        $query->fieldCondition($rule->field_name, $field_item, $needle, 'LIKE');
        break;

      case 'ends':
        $needle = '%' . db_like(substr($needle, -1, MATCH_DEFAULT_CHARS));
        $query->fieldCondition($rule->field_name, $field_item, $needle, 'LIKE');
        break;

      case 'contains':
        $needle = '%' . db_like($needle) . '%';
        $query->fieldCondition($rule->field_name, $field_item, $needle, 'LIKE');
        break;
    }

    $results = $query->execute();

    return isset($results['crm_core_contact']) ? array_keys($results['crm_core_contact']) : $results;
  }
}

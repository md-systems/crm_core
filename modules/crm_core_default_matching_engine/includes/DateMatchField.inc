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
    $results = array();
    $field_item = 'value';
    $field = field_get_items('crm_core_contact', $contact, $rule->field_name);
    $needle = isset($field[0]['value']) ? $field[0]['value'] : '';

    if (!empty($needle)) {
      $query = new EntityFieldQuery();
      $query->entityCondition('entity_type', 'crm_core_contact')->entityCondition('bundle', $contact->type)
        ->entityCondition('entity_id', $contact->contact_id, '<>')
        ->fieldCondition($rule->field_name, $field_item, $needle);

      $results = $query->execute();
    }

    return isset($results['crm_core_contact']) ? array_keys($results['crm_core_contact']) : $results;
  }
}

/**
 * Just extender of DateMatchField to catch field type.
 */
class DateTimeMatchField extends DateMatchField {
}

/**
 * Just extender of DateMatchField to catch field type.
 */
class DateStampMatchField extends DateMatchField {
}

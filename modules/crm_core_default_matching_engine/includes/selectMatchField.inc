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
    $contact_wrapper = entity_metadata_wrapper('crm_core_contact', $contact);
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

<?php

/**
 * @file
 * Implementation of FieldHandlerInterface for date fields.
 */

namespace Drupal\crm_core_default_matching_engine\Plugin\crm_core_match\field;

use Drupal\crm_core_contact\ContactInterface;

/**
 * Class for evaluating date fields.
 */
class DateMatchField extends FieldHandlerBase {

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
      '=' => t('Equals'),
      '>=' => t('Greater than'),
      '<=' => t('Less than'),
    );

    return $operators;
  }

  /**
   * {@inheritdoc}
   *
   * @todo Update to new query API.
   */
  public function match(ContactInterface $contact, $property = 'value') {
    $results = array();
    $field_item = 'value';
    $field = field_get_items('crm_core_contact', $contact, $rule->field_name);
    $needle = isset($field[0]['value']) ? $field[0]['value'] : '';

    if (!empty($needle)) {
      $query = new EntityFieldQuery();
      $query->entityCondition('entity_type', 'crm_core_contact')->entityCondition('bundle', $contact->type)
        ->entityCondition('entity_id', $contact->contact_id, '<>')
        ->fieldCondition($rule->field_name, $field_item, $needle, $rule->operator);

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

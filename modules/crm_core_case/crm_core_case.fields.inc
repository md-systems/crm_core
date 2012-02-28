<?php


/**
 * Implements hook_entity_insert().
 *
 * Add default fields for case type.
 */
function crm_core_case_entity_insert($entity, $entity_type) {
  if ($entity_type != 'crm_core_case_type') {
    return;
  }

  $type = $entity->type;

//  module_load_include('inc', 'crm_core_case', 'crm_core_case.fields');
  $fields = _crm_core_case_type_default_fields();

  // Create the fields if they don't exist
  foreach ($fields as $field) {
    $info = field_info_field($field['field_name']);
    if (empty($info)) {
      field_create_field($field);
    }
  }

  // Create field instances for the type if they don't exist
  $instances = array();

  // Date field.
  $instances[] = array(
    'field_name' => 'field_case_date',
    'entity_type' => 'crm_core_case',
    'bundle' => $type,
    'label' => t('Date'),
    'required' => FALSE,
    'settings' => array(
      'default_value' => 'now',
      'default_value_code' => '',
      'default_value2' => 'blank',
      'default_value_code2' => '',
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'weight' => '1',
      'type' => 'date_popup',
      'module' => 'date',
      'active' => 1,
      'settings' => array(
        'input_format' => 'm/d/Y - H:i:s',
        'input_format_custom' => '',
        'year_range' => '-3:+3',
        'increment' => '15',
        'label_position' => 'above',
        'text_parts' => array(),
        'repeat_collapsed' => 0,
      ),
    ),
    'display' => array(
      'default' => array(
        'label' => 'above',
        'type' => 'date_default',
        'settings' => array(
          'format_type' => 'long',
          'show_repeat_rule' => 'show',
          'multiple_number' => '',
          'multiple_from' => '',
          'multiple_to' => '',
          'fromto' => 'both',
        ),
        'module' => 'date',
        'weight' => 1,
      ),
    ),
  );

  $instances[] = array (
    'label' => 'Location',
    'widget' =>
    array (
      'weight' => '2',
      'type' => 'options_select',
      'module' => 'options',
      'active' => 1,
      'settings' =>
      array (
      ),
    ),
    'settings' =>
    array (
      'user_register_form' => false,
    ),
    'display' =>
    array (
      'default' =>
      array (
        'label' => 'above',
        'type' => 'taxonomy_term_reference_link',
        'settings' =>
        array (
        ),
        'module' => 'taxonomy',
        'weight' => 2,
      ),
    ),
    'required' => 1,
    'description' => '',
    'default_value' => NULL,
    'field_name' => 'field_case_location',
    'entity_type' => 'crm_core_case',
    'bundle' => $type,
    'deleted' => '0',
  );

  // Create field instances if they don't exist
  foreach ($instances as $instance) {
    $info_instance = field_info_instance('crm_core_case', $instance['field_name'], $type);
    if (empty($info_instance)) {
      field_create_instance($instance);
    }
  }
}

function _crm_core_case_type_default_fields() {

  $fields = array();

  // Date field.
  $fields[] = array(
    'field_name' => 'field_case_date',
    'type' => 'datetime',
    'module' => 'date',
    'active' => TRUE,
    'entity_types' => array('crm_core_case'),
    'translatable' => FALSE,
    // Allow admin to change settings of this field as for
    // example meeting might need end date.
    'locked' => FALSE,
    'settings' => array(
      'repeat' => 0,
      'granularity' => array(
        'month' => 'month',
        'day' => 'day',
        'hour' => 'hour',
        'minute' => 'minute',
        'year' => 'year',
        'second' => 0,
      ),
      'tz_handling' => 'site',
      'timezone_db' => 'UTC',
      'todate' => 'optional',
    ),
    'cardinality' => 1,
  );

  // Location field.
  $fields[] = array(
    'translatable' => 0,
    'entity_types' => array('crm_core_case'),
    'settings' => array(
      'allowed_values' => array(
        0 => array(
          'vocabulary' => 'site_location',
          'parent' => 0,
        ),
      ),
    ),
    'field_name' => 'field_case_location',
    'type' => 'taxonomy_term_reference',
    'module' => 'taxonomy',
    'active' => 1,
    'locked' => 1,
    'cardinality' => 1,
    'deleted' => 0,
  );

  return $fields;
}

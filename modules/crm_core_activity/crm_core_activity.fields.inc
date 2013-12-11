<?php

/**
 * @file
 * Adds fields to the activity entity type used by CRM Core.
 */

/**
 * Define the default fields for the CRM Core Activity entity type.
 *
 * @return array
 *   Return default fields to be attached to newly created activity type.
 *
 * @see field_create_field()
 */
function _crm_core_activity_type_default_fields() {
  $fields = array();

  // Participants field.
  $fields[] = array(
    'field_name' => 'field_activity_participants',
    'type' => 'entityreference',
    'entity_types' => array('crm_core_activity'),
    'translatable' => FALSE,
    'locked' => TRUE,
    'settings' => array(
      'target_type' => 'crm_core_contact',
      'handler' => 'base',
      'handler_submit' => 'Change handler',
      'handler_settings' => array('target_bundles' => array()),
    ),
    'cardinality' => -1,
  );

  // Date field.
  $fields[] = array(
    'field_name' => 'field_activity_date',
    'type' => 'datetime',
    'module' => 'date',
    'active' => TRUE,
    'entity_types' => array('crm_core_activity'),
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
      'todate' => '',
    ),
    'cardinality' => 1,
  );

  // Notes field.
  $fields[] = array(
    'field_name' => 'field_activity_notes',
    'type' => 'text_long',
    'entity_types' => array('crm_core_activity'),
    'translatable' => FALSE,
    'locked' => TRUE,
    'module' => 'text',
    'settings' => array(),
    'cardinality' => 1,
  );

  return $fields;
}

/**
 * Defines the default field instances for the CRM Core Activity entity type.
 *
 * @param string $type
 *   Activity type to get instances for.
 *
 * @return array
 *   Field instances.
 */
function _crm_core_activity_type_default_field_instances($type) {
  $instances = array();

  // Participants field.
  $instances[] = array(
    'field_name' => 'field_activity_participants',
    'entity_type' => 'crm_core_activity',
    'bundle' => $type,
    'label' => t('Participants'),
    'required' => TRUE,
    'settings' => array(
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'type' => 'entityreference_autocomplete_tags',
      'module' => 'entityreference',
      'active' => 1,
      'settings' => array(
        'match_operator' => 'CONTAINS',
        'size' => 60,
        'path' => '',
      ),
    ),
    'display' => array(
      'default' => array(
        'label' => 'above',
        'module' => 'entityreference',
        'settings' => array(
          'link' => 1,
        ),
        'type' => 'entityreference_label',
        'weight' => '0',
      ),
    ),
  );

  // Date field.
  $instances[] = array(
    'field_name' => 'field_activity_date',
    'entity_type' => 'crm_core_activity',
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

  // Participants field.
  $instances[] = array(
    'field_name' => 'field_activity_notes',
    'entity_type' => 'crm_core_activity',
    'bundle' => $type,
    'label' => t('Notes'),
    'required' => FALSE,
    'settings' => array(
      'text_processing' => '0',
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'weight' => 2,
      'type' => 'text_textarea',
      'module' => 'text',
      'active' => 1,
      'settings' => array(
        'rows' => 5,
      ),
    ),
    'display' => array(
      'default' => array(
        'label' => 'above',
        'type' => 'text_default',
        'settings' => array(),
        'module' => 'text',
        'weight' => 2,
      ),
    ),
  );

  return $instances;
}

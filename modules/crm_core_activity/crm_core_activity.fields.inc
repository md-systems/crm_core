<?php

/**
 * @return
 *   Return default fields definition that have to be attached to newly created activity type.
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

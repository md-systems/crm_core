<?php
/**
 * @return
 *   Return default fields definition that have to be attached to newly created relation type.
 *
 * @see field_create_field()
 */
function _crm_core_relationship_field_default_fields() {
  $fields = array();

  $fields[] = array(
    'active' => '1',
    'cardinality' => '1',
    'deleted' => '0',
    'entity_types' => array(),
    'field_name' => 'crm_core_relationship_status',
    'foreign keys' => array(),
    'indexes' => array(
      'value' => array(
        0 => 'value',
      ),
    ),
    'locked' => '1',
    'module' => 'list',
    'settings' => array(
      'allowed_values' => array(
        0 => t('Inactive'),
        1 => t('Active'),
      ),
      'allowed_values_function' => '',
    ),
    'translatable' => '0',
    'type' => 'list_boolean',
  );

  return $fields;
}

/**
 * @param $type string
 *  Bundle type.
 * @return array
 *  Array of field instances config.
 */
function _crm_core_relationship_field_status_instance($type) {
  $fields = array();
  $fields[] = array(
    'bundle' => $type,
    'default_value' => array(
      0 => array(
        'value' => 1,
      ),
    ),
    'deleted' => '0',
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'module' => 'list',
        'settings' => array(),
        'type' => 'list_default',
        'weight' => 1,
      ),
    ),
    'entity_type' => 'relation',
    'field_name' => 'crm_core_relationship_status',
    'label' => t('Status'),
    'required' => 0,
    'settings' => array(
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'options',
      'settings' => array(
        'display_label' => 0,
      ),
      'type' => 'options_onoff',
      'weight' => '1',
    ),
  );
  return $fields;
}

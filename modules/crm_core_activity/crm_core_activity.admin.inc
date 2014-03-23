<?php

/**
 * @file
 * Provides basic activity management functionality in CRM Core.
 */

/**
 * Generates the profile type editing form.
 */
function crm_core_activity_type_form($form, &$form_state, $crm_activity_type, $op = 'edit') {

  if ($op == 'clone') {
    $crm_activity_type->label .= ' (cloned)';
    $crm_activity_type->type = '';
  }

  $form['label'] = array(
    '#title' => t('Label'),
    '#type' => 'textfield',
    '#default_value' => $crm_activity_type->label,
    '#description' => t('The human-readable name of this profile type.'),
    '#required' => TRUE,
    '#size' => 30,
  );
  // Machine-readable type name.
  $form['type'] = array(
    '#type' => 'machine_name',
    '#default_value' => isset($crm_activity_type->type) ? $crm_activity_type->type : '',
    '#maxlength' => 32,
    '#disabled' => $crm_activity_type->isLocked() && $op != 'clone',
    '#machine_name' => array(
      'exists' => 'crm_core_activity_types',
      'source' => array('label'),
    ),
    '#description' => t('A unique machine-readable name for this profile type. It must only contain lowercase letters, numbers, and underscores.'),
  );

  $form['description'] = array(
    '#type' => 'textarea',
    '#default_value' => isset($crm_activity_type->description) ? $crm_activity_type->description : '',
    '#description' => t('Description about the activity type.'),
  );

  $form['activity_string_container'] = array(
    '#type' => 'fieldset',
    '#title' => t('Display settings'),
  );

  $form['activity_string_container']['activity_string'] = array(
    '#type' => 'textfield',
    '#title' => t('Activity string'),
    '#description' => t('Enter text describing the relationship between the contact and this activity. For example: Someone Somewhere registered for this activity.'),
    '#default_value' => empty($crm_activity_type->activity_string) ? '' : $crm_activity_type->activity_string,
  );

  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save activity type'),
    '#weight' => 40,
  );

  if (!$crm_activity_type->isLocked() && $op != 'add') {
    $form['actions']['delete'] = array(
      '#type' => 'submit',
      '#value' => t('Delete activity type'),
      '#weight' => 45,
      '#limit_validation_errors' => array(),
      '#submit' => array('crm_core_activity_type_form_submit_delete'),
    );
  }

  return $form;
}

/**
 * Submit handler for creating/editing crm_activity_type.
 */
function crm_core_activity_type_form_submit(&$form, &$form_state) {
  $crm_activity_type = entity_ui_form_submit_build_entity($form, $form_state);
  // Save and go back.
  $crm_activity_type->save();

  // If we create new activity type we need to add default fields to it.
  if ($form_state['op'] == 'add') {
    crm_core_activity_type_add_default_fields($crm_activity_type);
  }

  // Redirect user back to list of activity types.
  $form_state['redirect'] = 'admin/structure/crm-core/activity-types';
}

/**
 * Submit handler for deletion button for crm_activity_type.
 */
function crm_core_activity_type_form_submit_delete(&$form, &$form_state) {
  $form_state['redirect'] = 'admin/structure/crm-core/activity-types/manage/' . $form_state['crm_core_activity_type']->type . '/delete';
}

/**
 * Add default fields to newly created activity type.
 */
function crm_core_activity_type_add_default_fields($activity_type) {
  $type = $activity_type->type;

  module_load_include('inc', 'crm_core_activity', 'crm_core_activity.fields');
  $fields = _crm_core_activity_type_default_fields();
  drupal_alter('crm_core_activity_type_add_fields', $fields, $activity_type);

  foreach ($fields as $field) {
    $info = field_info_field($field['field_name']);
    if (empty($info)) {
      field_create_field($field);
    }
  }

  $instances = _crm_core_activity_type_default_field_instances($type);
  drupal_alter('crm_core_activity_type_add_field_instances', $instances, $activity_type);

  foreach ($instances as $instance) {
    $info_instance = field_info_instance('crm_core_activity', $instance['field_name'], $type);
    if (empty($info_instance)) {
      field_create_instance($instance);
    }
  }
}

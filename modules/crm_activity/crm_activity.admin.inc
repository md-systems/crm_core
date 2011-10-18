<?php

/**
 * UI controller for CRM Activity Type.
 */
class CRMActivityTypeUIController extends EntityDefaultUIController {
  /**
   * Overrides hook_menu() defaults.
   */
  public function hook_menu() {
    $items = parent::hook_menu();
    $items[$this->path]['description'] = 'Manage activity types.';
    return $items;
  }
}

/**
 * Generates the profile type editing form.
 */
function crm_activity_type_form($form, &$form_state, $crm_activity_type, $op = 'edit') {

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
      'exists' => 'crm_activity_types',
      'source' => array('label'),
    ),
    '#description' => t('A unique machine-readable name for this profile type. It must only contain lowercase letters, numbers, and underscores.'),
  );

  $form['description'] = array(
    '#type' => 'textarea',
    '#default_value' => isset($crm_activity_type->description) ? $crm_activity_type->description : '',
    '#description' => t('Description about the activity type.'),
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
      '#submit' => array('crm_activity_type_form_submit_delete')
    );
  }
  return $form;
}

/**
 * Submit handler for creating/editing crm_activity_type.
 */
function crm_activity_type_form_submit(&$form, &$form_state) {
  $crm_activity_type = entity_ui_form_submit_build_entity($form, $form_state);
  // Save and go back.
  $crm_activity_type->save();

  // If we create new activity type we need to add default fields to it.
  if ($form_state['op'] == 'add') {
    crm_activity_type_add_default_fields($crm_activity_type);
  }

  // Redirect user back to list of activity types.
  $form_state['redirect'] = 'crm/admin/activity_types';
}

function crm_activity_type_form_submit_delete(&$form, &$form_state) {
  $form_state['redirect'] = 'crm/admin/activity_types/manage/' . $form_state['crm_activity_type']->type . '/delete';
}

/**
 * Add default fields to newly created activity type.
 */
function crm_activity_type_add_default_fields($activity_type) {
  $type = $activity_type->type;

  $fields = array();

  // Participants field.
  $fields[] = array(
    'field_name' => 'field_acitivity_participants',
    'type' => 'entityreference',
    'entity_types' => array('crm_activity'),
    'translatable' => FALSE,
    'locked' => TRUE,
    'settings' => array(
      'target_type' => 'crm_contact',
      'handler' => 'base',
      'handler_submit' => 'Change handler',
      'handler_settings' => array('target_bundles' => array()),
    ),
    'cardinality' => -1,
  );

  // Create the fields if they don't exist
  foreach ($fields as $field) {
    $info = field_info_field($field['field_name']);
    if (empty($info)) {
      field_create_field($field);
    }
  }

  // Create field instances for the type if they don't exist
  $instances = array();

  $instances[] = array(
    'field_name' => 'field_acitivity_participants',
    'entity_type' => 'crm_activity',
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
    'display' => array(),
  );

  // Create field instances if they don't exist
  foreach ($instances as $instance) {
    $info_instance = field_info_instance('crm_activity', $instance['field_name'], $type);
    if (empty($info_instance)) {
      field_create_instance($instance);
    }
  }
}

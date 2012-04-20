<?php



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
      '#submit' => array('crm_core_activity_type_form_submit_delete')
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
  $form_state['redirect'] = 'admin/structure/crm/activity-types';
}

/**
 * Submit handler for deletion button for crm_activity_type.
 */
function crm_core_activity_type_form_submit_delete(&$form, &$form_state) {
  $form_state['redirect'] = 'admin/structure/crm/activity-types/manage/' . $form_state['crm_core_activity_type']->type . '/delete';
}

/**
 * Add default fields to newly created activity type.
 */
function crm_core_activity_type_add_default_fields($activity_type) {

  $type = $activity_type->type;

  module_load_include('inc', 'crm_core_activity', 'crm_core_activity.fields');
  $fields = _crm_core_activity_type_default_fields();

  // Create the fields if they don't exist
  foreach ($fields as $field) {
    $info = field_info_field($field['field_name']);
    if (empty($info)) {
      field_create_field($field);
    }
  }

  // Create field instances for the type if they don't exist
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
    'display' => array(),
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

  // Create field instances if they don't exist
  foreach ($instances as $instance) {
    $info_instance = field_info_instance('crm_core_activity', $instance['field_name'], $type);
    if (empty($info_instance)) {
      field_create_instance($instance);
    }
  }
}

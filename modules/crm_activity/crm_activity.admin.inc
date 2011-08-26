<?php

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
      'exists' => 'crm_activity_get_types',
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
  $form_state['redirect'] = 'admin/structure/crm_activity_types';
}

function crm_activity_type_form_submit_delete(&$form, &$form_state) {
  $form_state['redirect'] = 'admin/structure/crm_activity_types/manage/' . $form_state['crm_activity_type']->type . '/delete';
}

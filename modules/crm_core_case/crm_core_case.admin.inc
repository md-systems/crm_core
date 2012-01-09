<?php

/**
 * Generates the profile type editing form.
 */
function crm_core_case_type_form($form, &$form_state, $crm_case_type, $op = 'edit') {

  if ($op == 'clone') {
    $crm_case_type->label .= ' (cloned)';
    $crm_case_type->type = '';
  }

  $form['label'] = array(
    '#title' => t('Label'),
    '#type' => 'textfield',
    '#default_value' => $crm_case_type->label,
    '#description' => t('The human-readable name of this profile type.'),
    '#required' => TRUE,
    '#size' => 30,
  );
  // Machine-readable type name.
  $form['type'] = array(
    '#type' => 'machine_name',
    '#default_value' => isset($crm_case_type->type) ? $crm_case_type->type : '',
    '#maxlength' => 32,
    '#disabled' => $crm_case_type->isLocked() && $op != 'clone',
    '#machine_name' => array(
      'exists' => 'crm_core_case_types',
      'source' => array('label'),
    ),
    '#description' => t('A unique machine-readable name for this profile type. It must only contain lowercase letters, numbers, and underscores.'),
  );

  $form['description'] = array(
    '#type' => 'textarea',
    '#default_value' => isset($crm_case_type->description) ? $crm_case_type->description : '',
    '#description' => t('Description about the case type.'),
  );

  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save case type'),
    '#weight' => 40,
  );

  if (!$crm_case_type->isLocked() && $op != 'add' && $op != 'clone') {
    $form['actions']['delete'] = array(
      '#type' => 'submit',
      '#value' => t('Delete case type'),
      '#weight' => 45,
      '#limit_validation_errors' => array(),
      '#submit' => array('crm_core_case_type_form_submit_delete')
    );
  }
  return $form;
}

/**
 * Submit handler for creating/editing crm_case_type.
 */
function crm_core_case_type_form_submit(&$form, &$form_state) {
  $crm_case_type = entity_ui_form_submit_build_entity($form, $form_state);
  // Save and go back.
  $crm_case_type->save();

  // If we create new case type we need to add default fields to it.
  if ($form_state['op'] == 'add') {
//    crm_core_case_type_add_default_fields($crm_case_type);
  }

  // Redirect user back to list of case types.
  $form_state['redirect'] = 'admin/structure/crm/case-types';
}

function crm_core_case_type_form_submit_delete(&$form, &$form_state) {
  $form_state['redirect'] = 'admin/structure/crm/case-types/' . $form_state['crm_core_case_type']->type . '/delete';
}

/**
 * Case type delete form.
 */
function crm_core_case_type_form_delete_confirm($form, &$form_state, $case_type) {
  $form_state['case_type'] = $case_type;
  // Always provide entity id in the same form key as in the entity edit form.
  $form['case_type_id'] = array('#type' => 'value', '#value' => $case_type->id);
  return confirm_form($form,
    t('Are you sure you want to delete Case type %title?', array('%title' => $case_type->label)),
    'crm/activity/' . $case_type->id,
    t('This action cannot be undone.'),
    t('Delete'),
    t('Cancel')
  );
}

/**
 * Case type delete form submit handler.
 */
function crm_core_case_type_form_delete_confirm_submit($form, &$form_state) {
  $case_type = $form_state['case_type'];
  crm_core_case_type_delete($case_type);

  watchdog('crm_core_case_type', '@type: deleted %title.', array('@type' => $case_type->type, '%title' => $case_type->label));
  drupal_set_message(t('@type %title has been deleted.', array('@type' => $case_type->type, '%title' => $case_type->label)));

  $form_state['redirect'] = 'admin/structure/crm/case-types';
}

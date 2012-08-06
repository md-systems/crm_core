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
    'crm/case/' . $case_type->id,
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

function crm_core_case_admin_page() {
  return 'case lists';
}

function crm_core_case_admin_add_page() {
  $items = array();
  foreach (crm_core_case_types() as $case_type_key => $case_type) {
    $items[] = l($case_type->label, 'crm/case/add/' . $case_type_key);
  }
  return array('list' => array('#theme' => 'item_list', '#items' => $items, '#title' => t('Select type of case to create.')));
}

/**
 * Add new case page callback.
 */
function crm_core_case_add($type) {
  $case_type = crm_core_case_types($type);
  $case = crm_core_case_case_new($type);
  drupal_set_title(t('Create @name', array('@name' => $case_type->label)));

  $output = drupal_get_form($type . '_crm_core_case_form', $case);

  return $output;
}

function crm_core_case_form($form, &$form_state, $case) {
  $form_state['case'] = $case;

  $form['title'] = array(
    '#type' => 'textfield',
    '#required' => TRUE,
    '#title' => t('Name'),
    '#default_value' => $case->title,
  );

  $form['description'] = array(
    '#type' => 'textarea',
    '#title' => t('Description'),
    '#default_value' => $case->description,
  );

  $form['status'] = array(
    '#type' => 'select',
    '#options' => crm_core_case_status(),
    '#title' => t('Status'),
    '#default_value' => $case->status,
  );

  $form['uid'] = array(
    '#type' => 'value',
    '#value' => $case->uid,
  );

  field_attach_form('crm_core_case', $case, $form, $form_state);

  $submit = array();
  if (!empty($form['#submit'])) {
    $submit += $form['#submit'];
  }

  $form['actions'] = array(
    '#weight' => 100,
  );

  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save Case'),
    '#submit' => $submit + array('crm_core_case_form_submit'),
  );

  // Show Delete button if we edit case.
  if (!empty($case->cid) && crm_core_case_edit_access($case)) {
    $form['actions']['delete'] = array(
      '#type' => 'submit',
      '#value' => t('Delete'),
      '#submit' => array('crm_core_case_form_submit_delete'),
    );
  }

  $form['#validate'][] = 'crm_core_case_form_validate';

  return $form;
}

function crm_core_case_form_validate($form, &$form_state) {

}

/**
 * Case submit handler.
 */
function crm_core_case_form_submit($form, &$form_state) {
  $case = $form_state['case'];

  entity_form_submit_build_entity('crm_core_case', $case, $form, $form_state);

  entity_save('crm_core_case', $case);

  $uri = crm_core_case_uri($case);
  $form_state['redirect'] = $uri['path'];

  drupal_set_message(t('Case %title saved.', array('%title' => crm_core_case_label($case))));
}


function crm_core_case_form_submit_delete($form, &$form_state) {
  $destination = array();
  if (isset($_GET['destination'])) {
    $destination = drupal_get_destination();
    unset($_GET['destination']);
  }
  $case = $form_state['case'];
  $uri = crm_core_case_uri($case);
  $form_state['redirect'] = array($uri['path'] . '/delete', array('query' => $destination));
}

/**
 * Delete confirmation form.
 */
function crm_core_case_delete_form($form, &$form_state, $case) {
  $form_state['case'] = $case;
  // Always provide entity id in the same form key as in the entity edit form.
  $form['case_type_id'] = array('#type' => 'value', '#value' => $case->cid);
  $uri = crm_core_case_uri($case);
  return confirm_form($form,
    t('Are you sure you want to delete Case %title?', array('%title' => crm_core_case_label($case))),
    $uri['path'],
    t('This action cannot be undone.'),
    t('Delete'),
    t('Cancel')
  );
}

/**
 * Delete form submit handler.
 */
function crm_core_case_delete_form_submit($form, &$form_state) {
  $case = $form_state['case'];
  crm_core_case_delete($case);

  drupal_set_message(t('Case %title deleted.', array('%title' => crm_core_case_label($case))));

  $form_state['redirect'] = '<front>';
}

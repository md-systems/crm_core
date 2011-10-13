<?php

/**
 * A list of activities this contact has participated in.
 */
function crm_contact_activity() {
  return "This is a stub";
}

/**
 * Wrapper around entity_ui_get_form().
 *
 * Create new crm_activity entity and pass it to entity_ui_get_form().
 *
 * @param type $contact
 * @param type $type
 */
function crm_activity_entity_ui_get_form_wrapper($contact, $type) {
  $values = array(
    'type' => $type,
    'uid' => $contact->uid,
    'title' => '',
  );
  $activity = crm_activity_create($values);

  return entity_ui_get_form('crm_activity', $activity, 'add');
}

/**
 * Form builder for CRM Activity forms.
 */
function crm_activity_form($form, &$form_state, $activity) {
  // Ensure this include file is loaded when the form is rebuilt from the cache.
  $form_state['build_info']['file'] = drupal_get_path('module', 'crm_ui') . '/pages/contact_activity.pages.inc';

  $form['title'] = array(
    '#type' => 'textfield',
    '#required' => TRUE,
    '#title' => t('Title'),
    '#default_value' => $activity->title,
  );

  field_attach_form('crm_activity', $activity, $form, $form_state);

  $form['actions'] = array(
    '#type' => 'container',
    '#attributes' => array('class' => array('form-actions')),
    '#weight' => 40,
  );

  // We add the form's #submit array to this button along with the actual submit
  // handler to preserve any submit handlers added by a form callback_wrapper.
  $submit = array();

  if (!empty($form['#submit'])) {
    $submit += $form['#submit'];
  }

  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Add Activity'),
    '#submit' => $submit + array('crm_activity_form_submit'),
  );

  // We append the validate handler to #validate in case a form callback_wrapper
  // is used to add validate handlers earlier.
  $form['#validate'][] = 'crm_activity_form_validate';
  return $form;
}

function crm_activity_form_validate($form, &$form_state) {
  $activity = $form_state['crm_activity'];
  // Notify field widgets to validate their data.
  field_attach_form_validate('crm_activity', $activity, $form, $form_state);
}

function crm_activity_form_submit($form, $form_state) {
  $activity = $form_state['crm_activity'];

  $activity->title = $form_state['values']['title'];

  if (!isset($activity->created) || empty($activity->created)) {
    $activity->created = REQUEST_TIME;
  }

  $activity->changed = REQUEST_TIME;

  // Notify field widgets.
  field_attach_submit('crm_activity', $activity, $form, $form_state);

  crm_activity_save($activity);
}

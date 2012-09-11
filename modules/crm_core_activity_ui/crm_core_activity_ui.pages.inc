<?php

/**
 * Wrapper around entity_ui_get_form().
 *
 * Create new crm_activity entity and pass it to entity_ui_get_form().
 *
 * @param type $contact
 * @param type $type
 */
function crm_core_activity_entity_ui_get_form_wrapper($contact, $type) {
  // Default empty Activity.
  $values = array(
    'type' => $type,
    'field_activity_participants' => array(
      LANGUAGE_NONE => array(array(
          'target_id' => $contact->contact_id,
          'target_type' => 'crm_core_contact',
        ),
      ),
    ),
  );

  $activity = entity_create('crm_core_activity', $values);

  return entity_ui_get_form('crm_core_activity', $activity, 'add');
}

/**
 * Form builder for CRM Activity forms.
 */
function crm_core_activity_form($form, &$form_state, $activity) {
  // Ensure this include file is loaded when the form is rebuilt from the cache.
  $form_state['build_info']['files'][] = 
    drupal_get_path('module', 'crm_core_activity_ui') . '/crm_core_activity_ui.pages.inc';
  // get the contact id of who is trying to add the activity
  if (isset($activity->field_activity_participants['und']['0'])) {
      $form['contact_id'] = array(
        '#type' => 'value',
        '#value' => arg(1) == 'contact' ? arg(2) : $activity->field_activity_participants['und']['0']['target_id'],
      );
  }

  $form['title'] = array(
    '#type' => 'textfield',
    '#required' => TRUE,
    '#title' => t('Title'),
    '#default_value' => $activity->title,
  );

  $form['uid'] = array(
    '#type' => 'value',
    '#value' => $activity->uid,
  );

  field_attach_form('crm_core_activity', $activity, $form, $form_state);

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
    '#value' => t('Save Activity'),
    '#submit' => $submit + array('crm_core_activity_form_submit'),
  );

  // Show Delete button if we edit activity.
  if ($form_state['op'] == 'edit') {
    $form['actions']['delete'] = array(
      '#type' => 'submit',
      '#value' => t('Delete'),
      '#submit' => array('crm_core_activity_form_submit_delete'),
    );
  }

  // We append the validate handler to #validate in case a form callback_wrapper
  // is used to add validate handlers earlier.
  $form['#validate'][] = 'crm_core_activity_form_validate';

  // Add after build to set proper page Title as it
  // got set by entity_ui_main_form_defaults
  // @see entity_ui_get_form().
  $form['#after_build'][] = 'crm_core_activity_form_after_build';

  return $form;
}

/**
 * Validation callback for crm_activity_form.
 */
function crm_core_activity_form_validate($form, &$form_state) {

  $activity = $form_state['crm_core_activity'];
  if (isset($activity->field_activity_participants[LANGUAGE_NONE])) {

    // Ensure that contact set in $activity is present in values. We create activity
    // from the page of the contact. So we ensure that this contact is added in
    // participants field.
    $form_state_values = array();
    foreach ($form_state['values']['field_activity_participants'][LANGUAGE_NONE] as $value) {
      $form_state_values[] = $value['target_id'];
    }

    $activity_values = array();
    foreach ($activity->field_activity_participants[LANGUAGE_NONE] as $value) {
      $activity_values[] = $value['target_id'];
    }
    $array_intersect = array_intersect($form_state_values, $activity_values);

    // Not all default values present in form state values.
    if (array_values($array_intersect) != $activity_values) {
      form_set_error('field_activity_participants', t('Original contact should be included in Participants field.'));
    }
  }

  // Notify field widgets to validate their data.
  field_attach_form_validate('crm_core_activity', $activity, $form, $form_state);

}

/**
 * Submit callback for crm_activity_form.
 */
function crm_core_activity_form_submit($form, &$form_state) {
  $activity = $form_state['crm_core_activity'];

  $activity->title = $form_state['values']['title'];
  $activity->uid = $form_state['values']['uid'];

  if (!isset($activity->created) || empty($activity->created)) {
    $activity->created = REQUEST_TIME;
  }

  $activity->changed = REQUEST_TIME;

  // Notify field widgets.
  field_attach_submit('crm_core_activity', $activity, $form, $form_state);

  crm_core_activity_save($activity);

  // Show message to user.
  if ($form_state['op'] == 'add') {
    drupal_set_message(t('Activity @title created.', array('@title' => $activity->title)));
  }
  elseif ($form_state['op'] == 'edit') {
    drupal_set_message(t('Activity @title edited.', array('@title' => $activity->title)));
  }

  $form_state['redirect'] = 'crm/activity/' . $activity->activity_id;
}

/**
 * Submit handler for Delete button.
 *
 * Redirect to delete confirmation page.
 */
function crm_core_activity_form_submit_delete($form, &$form_state) {
  $destination = array();
  if (isset($_GET['destination'])) {
    $destination = drupal_get_destination();
    unset($_GET['destination']);
  }
  $activity = $form_state['crm_core_activity'];
  $form_state['redirect'] = array('crm/activity/' . $activity->activity_id . '/delete', array('query' => $destination));
}

function crm_core_activity_form_delete_confirm($form, &$form_state, $activity) {
  $form_state['activity'] = $activity;
  // Always provide entity id in the same form key as in the entity edit form.
  $form['activity_id'] = array('#type' => 'value', '#value' => $activity->activity_id);
  return confirm_form($form,
    t('Are you sure you want to delete activity %title?', array('%title' => $activity->title)),
    'crm/activity/' . $activity->activity_id,
    t('This action cannot be undone.'),
    t('Delete'),
    t('Cancel')
  );
}

/**
 * Execute node deletion.
 */
function crm_core_activity_form_delete_confirm_submit($form, &$form_state) {
  if ($form_state['values']['confirm']) {
    $activity = $form_state['activity'];
    crm_core_activity_delete($activity);

    watchdog('crm_core_activity', '@type: deleted %title.', array('@type' => $activity->type, '%title' => $activity->title));
    drupal_set_message(t('@type %title has been deleted.', array('@type' => crm_core_activity_type_get_name($activity->type), '%title' => $activity->title)));
  }
}

/**
 * After build callback for crm_activity_form
 *
 * Set proper page title.
 */
function crm_core_activity_form_after_build(&$form, &$form_state) {
  $crm_activity_type = crm_core_activity_types($form_state['crm_core_activity']->type);
  if ($form_state['op'] == 'add') {
    drupal_set_title(t('Add new activity @label', array('@label' => $crm_activity_type->label)));
  }
  return $form;
}

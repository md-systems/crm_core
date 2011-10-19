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
  global $user;
  // Default empty Activity.
  $values = array(
    'type' => $type,
    'uid' => $user->uid,
    'field_acitivity_participants' => array(
      LANGUAGE_NONE => array(array(
          'target_id' => $contact->contact_id,
          'target_type' => 'crm_contact',
        ),
      ),
    ),
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

  $form['uid'] = array(
    '#type' => 'value',
    '#value' => $activity->uid,
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

  // Add after build to set proper page Title as it
  // got set by entity_ui_main_form_defaults
  // @see entity_ui_get_form().
  $form['#after_build'][] = 'crm_activity_form_after_build';

  return $form;
}

/**
 * Validation callback for crm_activity_form.
 */
function crm_activity_form_validate($form, &$form_state) {
  $activity = $form_state['crm_activity'];

  // Ensure that contact set in $activity is present in values. We create activity
  // from the page of the contact. So we ensure that this contact is added in
  // participants field.
  $form_state_values = array();
  foreach ($form_state['values']['field_acitivity_participants'][LANGUAGE_NONE] as $value) {
    $form_state_values[] = $value['target_id'];
  }

  $activity_values = array();
  foreach ($activity->field_acitivity_participants[LANGUAGE_NONE] as $value) {
    $activity_values[] = $value['target_id'];
  }
  $array_intersect = array_intersect($form_state_values, $activity_values);

  // Not all default values present in form state values.
  if (array_values($array_intersect) != $activity_values) {
    form_set_error('field_acitivity_participants', t('Original contact should be included in Participants field.'));
  }

  // Notify field widgets to validate their data.
  field_attach_form_validate('crm_activity', $activity, $form, $form_state);
}

/**
 * Submit callback for crm_activity_form.
 */
function crm_activity_form_submit($form, &$form_state) {
  $activity = $form_state['crm_activity'];

  $activity->title = $form_state['values']['title'];
  $activity->uid = $form_state['values']['uid'];

  if (!isset($activity->created) || empty($activity->created)) {
    $activity->created = REQUEST_TIME;
  }

  $activity->changed = REQUEST_TIME;

  // Notify field widgets.
  field_attach_submit('crm_activity', $activity, $form, $form_state);

  crm_activity_save($activity);

  // Show message to user.
  drupal_set_message(t('Activity @title created.', array('@title' => $activity->title)));

  $form_state['redirect'] = 'crm/contact/' . arg(2) . '/activity';
}

/**
 * After build callback for crm_activity_form
 *
 * Set proper page title.
 */
function crm_activity_form_after_build(&$form, &$form_state) {
  $crm_activity_type = crm_activity_types($form_state['crm_activity']->type);
  drupal_set_title(t('Add new activity @label', array('@label' => $crm_activity_type->label)));
  return $form;
}

/**
 * Activity view callback.
 */
function crm_activity_view($activity) {
  $content = entity_get_controller('crm_activity')->view(array($activity->activity_id => $activity), 'full');

  // We don't want to run output through entity theming function.
  // To avoid this hack apply patch from http://drupal.org/node/1259916
  // Then we can load content in following way
  // $content = entity_build_content('crm_activity', $activity);
  unset($content['crm_activity'][$activity->activity_id]['#theme']);
  return $content;
}

<?php

/*
 *
 */
function crm_core_contact_ui_form_wrapper($contact) {
  return drupal_get_form('crm_core_contact_ui_form', $contact);
}


/**
 * Show a list of contact types that can be added into the CRM.
 */
function crm_core_contact_ui_add_page() {
  drupal_set_title(t('Add a contact'));
  $item = menu_get_item();
  $types = crm_core_contact_types(TRUE);
  $content = array();
  foreach ($types as $type) {
    $_content = menu_get_item($item['href'] . '/' . $type->type);
    // access check
    if ($_content['access']) {
      $content[] = $_content;
    }
  }

  // If there is only one contact type addition page we go straight to it
  if (count($content) == 1) {
    $item = array_shift($content);
    drupal_goto($item['href']);
  }

  return theme('crm_core_contact_ui_add_list', array('content' => $content));
}

/**
 * Wrapper around entity_ui_get_form().
 *
 * Create new crm_activity entity and pass it to entity_ui_get_form().
 *
 * @param type $contact
 * @param type $type
 */
function crm_core_contact_entity_ui_get_form_wrapper($type) {
  // Default empty Contact
  $values = array(
    'type' => $type,
  );

  $contact = entity_create('crm_core_contact', $values);

  return drupal_get_form('crm_core_contact_ui_form', $contact);
}

/**
 * Form callback: create or edit a contact.
 *
 * @param $contact
 *   The contact object to edit or for a create form an empty contact object
 *     with only a contact type defined.
 */
function crm_core_contact_ui_form($form, &$form_state, $contact) {
  // Ensure this include file is loaded when the form is rebuilt from the cache.
  $form_state['build_info']['file'] = drupal_get_path('module', 'crm_core_contact_ui') . '/crm_core_contact_ui.pages.inc';

  // Add the field related form elements.
  $form_state['crm_core_contact'] = $contact;
  field_attach_form('crm_core_contact', $contact, $form, $form_state);

  $form['actions'] = array('#type' => 'actions');

  // We add the form's #submit array to this button along with the actual submit
  // handler to preserve any submit handlers added by a form callback_wrapper.
  $submit = array();

  if (!empty($form['#submit'])) {
    $submit += $form['#submit'];
  }

  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => crm_core_contact_ui_save_contact_button_name($contact->type),
    '#submit' => $submit + array('crm_core_contact_ui_form_submit'),
  );

  // Show delete button if entity exists and user has appropriate permission
  if (!empty($contact->contact_id) && entity_access('delete', 'crm_core_contact', $contact->contact_id)) {
    $form['actions']['delete'] = array(
      '#type' => 'submit',
      '#value' => t('Delete'),
      '#submit' => array('crm_core_contact_ui_form_submit_delete'),
    );
  }

  // We append the validate handler to #validate in case a form callback_wrapper
  // is used to add validate handlers earlier.
  $form['#validate'][] = 'crm_core_contact_ui_form_validate';

  return $form;
}

/**
 * Validation callback for crm_core_contact_ui_form().
 */
function crm_core_contact_ui_form_validate($form, &$form_state) {
  $contact = $form_state['crm_core_contact'];

  // Notify field widgets to validate their data.
  field_attach_form_validate('crm_core_contact', $contact, $form, $form_state);
}

/**
 * Submit callback for crm_core_contact_ui_form().
 */
function crm_core_contact_ui_form_submit($form, &$form_state) {
  global $user;

  $create = FALSE;

  $contact = &$form_state['crm_core_contact'];


  // Set the contact's uid if it's being created at this time.
  if (empty($contact->contact_id)) {
    $contact->uid = $user->uid;
    $create = TRUE;
  }

  // Notify field widgets.
  field_attach_submit('crm_core_contact', $contact, $form, $form_state);

  // Save the contact
  crm_core_contact_save($contact);

  // Redirect based on the button clicked.
  drupal_set_message(t('Contact saved.'));
  $form_state['redirect'] = ($create) ? 'crm-core/contact' : "crm-core/contact/{$contact->contact_id}";
}

/**
 * Redirect user on case delete form.
 */
function crm_core_contact_ui_form_submit_delete($form, &$form_state) {
  $destination = array();
  if (isset($_GET['destination'])) {
    $destination = drupal_get_destination();
    unset($_GET['destination']);
  }
  $contact = $form_state['crm_core_contact'];
  $uri = entity_uri('crm_core_contact', $contact);
  $form_state['redirect'] = array($uri['path'] . '/delete', array('query' => $destination));
}

/**
 * Form for deleting a contact.
 */
function crm_core_contact_ui_delete_form($form, &$form_state, $contact) {
  $form['contact_id'] = array('#type' => 'value', '#value' => $contact->contact_id);
  return confirm_form($form,
    t("Are you sure want to delete '@name' contact?", array('@name' => crm_core_contact_title($contact))),
    'crm-core/contact/' . $contact->contact_id, $description = NULL, $yes = t('Yes'), $no = t('No'));
}

/**
 * Submit callback for crm_core_contact_ui_delete_form().
 */
function crm_core_contact_ui_delete_form_submit($form, &$form_state) {
  if (crm_core_contact_delete($form_state['values']['contact_id'])) {
    drupal_set_message(t('Contact deleted.'));
  }
  $form_state['redirect'] = 'crm-core/contact';
}

/**
 * Validate callback for crm_core_contact_ui_delete_form().
 */
function crm_core_contact_ui_delete_form_validate($form, &$form_state) {

}


/**
 * Form for comparing different versions of a contact.
 */
function crm_core_contact_ui_revision_form($contact) {
  $revisions = crm_core_contact_ui_revision_list($contact);
  var_dump($revisions);

  $form['placeholder'] = array(
    '#markup' => 'This is a placeholder for now.',
  );

  return $form;
}

/**
 * Form for reverting a contact to a previous version.
 */
function crm_core_contact_ui_revert_form($form, &$form_state, $contact, $vid) {

  $form['vid'] = array(
    '#type' => 'value',
    '#value' => $vid,
  );

  $form_state['crm_core_ui']['contact'] = $contact;

  return confirm_form($form,
    t("Are you sure want to revert this contact?"),
    'crm-core/contact/' . $contact->contact_id . '/revisions', 'Reverting the contact will result in a new revision being created', t('Yes'), t('Cancel'));
}

function crm_core_contact_ui_revert_form_submit($form, &$form_state) {

  $contact = $form_state['crm_core_ui']['contact'];

  if (crm_core_contact_revert($contact, $form_state['values']['vid'])) {
    drupal_set_message(t('Contact reverted.'));
  }
  $form_state['redirect'] = 'crm-core/contact/' . $contact->contact_id . '/revisions';
}

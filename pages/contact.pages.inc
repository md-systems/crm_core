<?php

/**
 * View a single contact record.
 */
function crm_contact_view($contact) {
  field_attach_prepare_view('crm_contact', array($contact->contact_id => $contact), 'full');
  return field_attach_view('crm_contact', $contact, 'full');
}

/*
 *
 */
function crm_contact_form_wrapper($contact) {
  return drupal_get_form('crm_contact_form', $contact);
}


/**
 * Show a list of contact types that can be added into the CRM.
 */
function crm_contact_add_page() {
  $item = menu_get_item();
  
  $types = crm_contact_types();
  $content = array();
  foreach ($types as $type) {
    $content[] = menu_get_item($item['href'] . '/' . $type->type);
  }


  // If there is only one contact type addition page we go straight to it
  if (count($content) == 1) {
    $item = array_shift($content);
    drupal_goto($item['href']);
  }

  return theme('crm_contact_add_list', array('content' => $content));
}


/**
 * Form callback: create or edit a contact.
 *
 * @param $contact
 *   The contact object to edit or for a create form an empty contact object
 *     with only a contact type defined.
 */
function crm_contact_form($form, &$form_state, $contact) {
  // Ensure this include file is loaded when the form is rebuilt from the cache.
  $form_state['build_info']['file'] = drupal_get_path('module', 'crm_contact') . '/pages/contact.pages.inc';
  
  // Add the field related form elements.
  $form_state['crm_contact'] = $contact;
  field_attach_form('crm_contact', $contact, $form, $form_state);

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
    '#value' => t('Save Contact'),
    '#submit' => $submit + array('crm_contact_form_submit'),
  );

  // We append the validate handler to #validate in case a form callback_wrapper
  // is used to add validate handlers earlier.
  $form['#validate'][] = 'crm_contact_form_validate';

  return $form;
}


/**
 * Validation callback for crm_contact_form().
 */
function crm_contact_form_validate($form, &$form_state) {
  $contact = $form_state['crm_contact'];

  // Notify field widgets to validate their data.
  field_attach_form_validate('crm_contact', $contact, $form, $form_state);
}


/**
 * Submit callback for commerce_product_product_form().
 */
function crm_contact_form_submit($form, &$form_state) {
  global $user;

  $contact = &$form_state['crm_contact'];


  // Set the contact's uid if it's being created at this time.
  if (empty($contact->contact_id)) {
    $contact->uid = $user->uid;
  }

  // Notify field widgets.
  field_attach_submit('crm_contact', $contact, $form, $form_state);

  // Save the contact
  crm_contact_save($contact);

  // Redirect based on the button clicked.
  drupal_set_message(t('Contact saved.'));
}


/**
 * Form for deleteing a contact.
 */
function crm_contact_delete_form($contact) {

}


/**
 * Form for comparing different versions of a contact.
 */
function crm_contact_revision_form($contact) {

  $revisions = crm_contact_revision_list($contact);
  var_dump($revisions);

  $form['placeholder'] = array(
    '#markup' => 'This is a placeholder for now.',
  );

  return $form;
}

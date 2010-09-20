<?php

function crm_contact_view($contact) {
  return field_attach_view('crm_contact', $contact, 'full');
}

function crm_contact_form_wrapper($contact) {
  return drupal_get_form('crm_contact_form', $contact);
}


function crm_contact_add_page() {
  
}


/**
 * Form callback: create or edit a product.
 *
 * @param $product
 *   The product object to edit or for a create form an empty product object
 *     with only a product type defined.
 */
function crm_contact_form($form, &$form_state, $contact) {
  // Ensure this include file is loaded when the form is rebuilt from the cache.
  //$form_state['build_info']['file'] = drupal_get_path('module', 'commerce_product') . '/includes/commerce_product.forms.inc';

  
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


  // Set the product's uid if it's being created at this time.
  if (empty($contact->contact_id)) {
    $product->uid = $user->uid;
  }

  // Notify field widgets.
  field_attach_submit('crm_contact', $contact, $form, $form_state);

  // Save the product.
  crm_contact_save($contact);

  // Redirect based on the button clicked.
  drupal_set_message(t('Contact saved.'));
}

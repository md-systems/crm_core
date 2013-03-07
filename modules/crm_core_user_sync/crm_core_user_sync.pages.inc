<?php
/**
 * @file Non admin pages.
 */

/**
 * Contact->User relation edit form.
 */
function crm_core_user_sync_edit_form($form, &$form_state, $contact) {
  drupal_set_title('Edit Relation');
  $form['description_wrapper'] = array(
    '#type' => 'container',
  );
  $form['description_wrapper']['description'] = array(
    '#type' => 'item',
    '#markup' => t('You can edit the details of a contact record to user account relationship using the following form. '
      . 'Leave either field blank to remove the relationship.'),
  );
  $form['contact'] = array(
    '#title' => t('Contact Record'),
    '#type' => 'textfield',
    '#default_value' => $contact->label() . " [cid:{$contact->contact_id}]",
    '#autocomplete_path' => '',
  );
  $form['user'] = array(
    '#title' => t('User Record'),
    '#type' => 'textfield',
    '#default_value' => '',
    '#autocomplete_path' => '',
  );
  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
  );
  $form['actions']['cancel'] = array(
    '#markup' => l(t('Cancel'), 'crm-core/contact/' . $contact->contact_id),
  );
  return  $form;
}

<?php

/**
 * @file
 * Interface elements for adding, editing, and otherwise working with contact types.
 */


function crm_core_contact_ui_types_overview() {
  $header = array(
    t('Name'),
    t('Operations'),
  );
  $rows = array();
  foreach (crm_core_contact_types() as $type => $contact_type) {
    $links = menu_contextual_links('crm-core', 'admin/structure/crm-core/contact-types', array($type));

    // @todo: this might not be the best place to hide these
    if ((bool) $contact_type->disabled) {
      unset($links['crm-fields'], $links['crm-display']);
    }

    $ops = array();
    foreach ($links as $key => $link) {
      $ops['links'][$key] = array(
        'title' => $link['title'],
        'href' => $link['href'],
      );
    }

    $rows[] = array(
      theme('crm_core_contact_ui_type_overview', array('contact_type' => $contact_type)),
      theme('links__ctools_dropbutton', $ops),
    );
  }

  crm_core_ui_ctools_add_dropbutton_files();
  $build['contact_type_table'] = array(
    '#theme' => 'table',
    '#header' => $header,
    '#rows' => $rows,
    '#empty' => t('There are no contact types. <a href="@link">Add contact type</a>.', array('@link' => url('crm-core/admin/contact-types/manage'))),
  );

  return $build;
}


//function crm_core_contact_ui_type_form_wrapper($contact_type) {
//  return drupal_get_form('crm_core_contact_ui_type_form', $contact_type);
//}




/**
 * Given a contact type, present a form prompting for disabling of the contact type.
 *
 * @param $form
 *  The confirmation form.
 * @param $form_state
 *  The state of the form.
 * @param $contact_type
 *  The machine readable name of the contact type to be deleted.
 */
function crm_core_contact_ui_type_toggle_form($form, &$form_state, $contact_type, $toggle = 'disable') {
  $form_state['contact_type'] = $contact_type;

  $form['op'] = array(
    '#type' => 'value',
    '#value' => check_plain($toggle),
  );

  $question = t('Are you sure you want to @toggle this contact type?', array('@toggle' => $toggle));
  $path = 'admin/structure/crm-core/contact-types';

  $description = '';
  if ($toggle == 'disable') {
    $description = t('When a contact type is disabled, you cannot add any contacts to this contact type. You will also not be able to search for contacts of disabled contact type.');
  }

  return confirm_form($form, $question, $path, $description, check_plain($toggle), 'Cancel', 'toggle_confirm');
}


/**
 * Form submit handler: enable or disable a contact type
 */
function crm_core_contact_ui_type_toggle_form_submit($form, &$form_state) {

  $crm_core_contact_type = $form_state['contact_type'];

  switch ($form_state['values']['op']) {
    case 'enable':
      $crm_core_contact_type->disabled = 0;
    break;

    case 'disable':
    default:
      $crm_core_contact_type->disabled = 1;
    break;
  }

  crm_core_contact_type_save($crm_core_contact_type);
  $form_state['redirect'] = 'admin/structure/crm-core/contact-types';
  drupal_set_message(t('Contact type saved'));
}


/**
 * Given a contact type, present a form prompting for deletion of the contact type.
 *
 * @param $form
 *  The confirmation form.
 * @param $form_state
 *  The state of the form.
 * @param $contact_type
 *  The machine readable name of the contact type to be deleted.
 */
function crm_core_contact_ui_type_delete_confirm($form, &$form_state, $contact_type) {
  $form_state['contact_type'] = $contact_type;

  $form['message'] = array(
    '#markup' => t('Are you sure you want to delete this contact type?  All contacts of this type will be deleted.'),
  );

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
    '#value' => t('Delete contact type'),
    '#submit' => $submit + array('crm_core_contact_ui_type_delete_confirm_submit'),
  );

  return $form;
}


/**
 * Form validation callback
 */
function crm_core_contact_ui_type_delete_confirm_validate($form, &$form_state) {

  $crm_core_contact_type = $form_state['contact_type'];
  // check one more time to see if the contact type can be deleted
  $error = NULL;

  if ((bool) $crm_core_contact_type->locked) {
    $error = t('The contact type is locked');
  }

  $count = db_query("SELECT count(*) FROM {crm_core_contact} WHERE `type` = :type",
  array(':type' => $crm_core_contact_type->type))->fetchField();

  if ($count > 0) {
    $error = t('Contacts of this type exists');
  }

  if (isset($error)) {
    form_set_error('message', t('Error: you cannot delete this contact type because @msg', array('@msg' => $error)));
  }
}


/**
 * Submission callback for contact type delete
 */
function crm_core_contact_ui_type_delete_confirm_submit($form, &$form_state) {
  $crm_core_contact_type = $form_state['contact_type'];
  crm_core_contact_type_delete($crm_core_contact_type->id);

  $form_state['redirect'] = 'admin/structure/crm-core/contact-types';
  drupal_set_message(t('Contact type deleted'));

}

<?php

/*
 *
 */

use Drupal\Component\Utility\Xss;
use Drupal\Core\Form\FormStateInterface;
use Drupal\Core\Url;

function crm_core_contact_ui_form_wrapper($contact) {
  return drupal_get_form('crm_core_contact_ui_form', $contact);
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

function crm_core_contact_ui_revert_form_submit($form, FormStateInterface $form_state) {

  $contact = $form_state['crm_core_ui']['contact'];

  if (crm_core_contact_revert($contact, $form_state->getValue('vid'))) {
    drupal_set_message(t('Contact reverted.'));
  }
  $form_state->setRedirectUrl(Url::fromUri('base:crm-core/contact/' . $contact->contact_id . '/revisions'));
}

/**
 * Prepares variables for list of available contact type templates.
 *
 * Default template: crm-core-contact-ui-add-list.html.twig.
 *
 * @param array $variables
 *   An associative array containing:
 *   - content: An array of contact types.
 */
function template_preprocess_crm_core_contact_ui_add_list(&$variables) {
  $variables['types'] = array();
  if (!empty($variables['content'])) {
    foreach ($variables['content'] as $type) {
      $variables['types'][$type->id()] = array(
        'type' => $type->id(),
        'add_link' => l(t('Add @type', array('@type' => $type->label())), 'crm-core/contact/add/' . $type->id()),
        'description' => Xss::filterAdmin($type->description),
      );
    }
  }
}

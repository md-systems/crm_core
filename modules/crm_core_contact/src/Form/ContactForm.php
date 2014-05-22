<?php

/**
 * @file
 * Contains \Drupal\crm_core_contact\Form\ContactForm.
 */

namespace Drupal\crm_core_contact\Form;

use Drupal\Core\Entity\ContentEntityForm;

class ContactForm extends ContentEntityForm {

  /**
   * {@inheritdoc}
   */
  public function save(array $form, array &$form_state) {
    $contact = $this->entity;

    $status = $contact->save();

    $t_args = array('%name' => $contact->label());

    if ($status == SAVED_UPDATED) {
      drupal_set_message(t('The contact %name has been updated.', $t_args));
    }
    elseif ($status == SAVED_NEW) {
      drupal_set_message(t('The contact %name has been added.', $t_args));
      watchdog('crm_core_contact', 'Added contact %name.', $t_args, WATCHDOG_NOTICE, l(t('View'), $contact->url()));
    }

    $form_state['redirect_route']['route_name'] = 'crm_core_contact.list';
  }
}

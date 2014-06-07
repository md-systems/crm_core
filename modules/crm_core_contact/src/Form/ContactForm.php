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
      if ($contact->access('view')) {
        $form_state['redirect_route'] = array(
          'route_name' => 'crm_core_contact.view',
          'route_parameters' => array(
            'crm_core_contact' => $contact->id(),
          ),
        );
      }
      else {
        $form_state['redirect_route']['route_name'] = 'crm_core_contact.list';
      }
    }
    elseif ($status == SAVED_NEW) {
      drupal_set_message(t('The contact %name has been added.', $t_args));
      watchdog('crm_core_contact', 'Added contact %name.', $t_args, WATCHDOG_NOTICE, l(t('View'), $contact->url()));
      $form_state['redirect_route']['route_name'] = 'crm_core_contact.list';
    }
  }

  /**
   * {@inheritdoc}
   */
  protected function actions(array $form, array &$form_state) {
    $actions = parent::actions($form, $form_state);
    $actions['submit']['#value'] = $this->t('Save !contact_type', array(
      '!contact_type' => $this->entity->get('type')->entity->label(),
    ));
    return $actions;
  }
}

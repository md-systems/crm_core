<?php

/**
 * @file
 * Contains \Drupal\crm_core_contact\Form\ContactForm.
 */

namespace Drupal\crm_core_contact\Form;

use Drupal\Core\Entity\ContentEntityForm;
use Drupal\Core\Form\FormStateInterface;

class ContactForm extends ContentEntityForm {

  /**
   * {@inheritdoc}
   */
  public function save(array $form, FormStateInterface $form_state) {
    $contact = $this->entity;

    $status = $contact->save();

    $t_args = array('%name' => $contact->label(), 'link' => $contact->url());

    if ($status == SAVED_UPDATED) {
      drupal_set_message($this->t('The contact %name has been updated.', $t_args));
      if ($contact->access('view')) {
        $form_state->setRedirect('crm_core_contact.view', ['crm_core_contact' => $contact->id()]);
      }
      else {
        $form_state->setRedirect('crm_core_contact.list');
      }
    }
    elseif ($status == SAVED_NEW) {
      drupal_set_message($this->t('The contact %name has been added.', $t_args));
      \Drupal::logger('crm_core_contact')->notice('Added contact %name.', $t_args);
      $form_state->setRedirect('crm_core_contact.list');
    }
  }

  /**
   * {@inheritdoc}
   */
  protected function actions(array $form, FormStateInterface $form_state) {
    $actions = parent::actions($form, $form_state);
    $actions['submit']['#value'] = $this->t('Save !contact_type', array(
      '!contact_type' => $this->entity->get('type')->entity->label(),
    ));
    return $actions;
  }
}

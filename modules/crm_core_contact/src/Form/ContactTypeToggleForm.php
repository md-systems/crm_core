<?php

/**
 * @file
 * Contains \Drupal\crm_core_contact\Form\ContactTypeToggleForm.
 */

namespace Drupal\crm_core_contact\Form;

use Drupal\Core\Entity\EntityConfirmFormBase;
use Drupal\Core\Url;

class ContactTypeToggleForm extends EntityConfirmFormBase {

  /**
   * {@inheritdoc}
   */
  public function getQuestion() {
    $args = array(
      '%toggle' => $this->getRequest()->get('op'),
      '%type' => $this->getEntity()->label(),
    );

    $question = '';

    switch ($this->getRequest()->get('op')) {
      case 'enable':
        $question = $this->t('Are you sure you want to enable the contact type %type?', $args);
        break;

      case 'disable':
        $question = $this->t('Are you sure you want to disable the contact type %type?', $args);
        break;
    }

    return $question;
  }

  /**
   * {@inheritdoc}
   */
  public function getDescription() {
    return $this->t('When a contact type is disabled, you cannot add any contacts to this contact type. You will also not be able to search for contacts of disabled contact type.');
  }

  /**
   * {@inheritdoc}
   */
  public function getConfirmText() {
    switch ($this->getRequest()->get('op')) {
      case 'disable':
        $text = $this->t('Disable');
        break;

      default:
      case 'enable':
        $text = $this->t('Enable');
        break;
    }

    return $text;
  }

  /**
   * {@inheritdoc}
   */
  public function getCancelUrl() {
    return new Url('crm_core_contact.type_list');
  }

  /**
   * {@inheritdoc}
   */
  public function submit(array $form, array &$form_state) {
    switch ($this->getRequest()->get('op')) {

      case 'disable':
        $action = $this->t('disabled');
        $this->entity->disable()->save();
        break;

      default:
      case 'enable':
        $action = $this->t('enabled');
        $this->entity->enable()->save();
        break;
    }
    $t_args = array(
      '%name' => $this->entity->label(),
      '%toggle' => $action,
    );
    drupal_set_message($this->t('The contact type %name has been %toggle.', $t_args));

    $form_state['redirect_route'] = new Url('crm_core_contact.type_list');
  }
}

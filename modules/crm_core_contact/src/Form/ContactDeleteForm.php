<?php

/**
 * @file
 * Contains \Drupal\crm_core_contact\Form\ContactDeleteForm.
 */

namespace Drupal\crm_core_contact\Form;

use Drupal\Core\Entity\ContentEntityDeleteForm;

/**
 * The confirmation form for deleting a contact.
 */
class ContactDeleteForm extends ContentEntityDeleteForm {

  /**
   * {@inheritdoc}
   */
  protected function getDeletionMessage() {
    $entity = $this->getEntity();
    return $this->t('The contact %name (%id) has been deleted.', array(
      '%id' => $entity->id(),
      '%name' => $entity->label(),
    ));
  }

}

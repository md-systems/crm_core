<?php

/**
 * @file
 * Contains \Drupal\crm_core_activity\Form\ActivityDeleteForm.
 */

namespace Drupal\crm_core_activity\Form;

use Drupal\Core\Entity\ContentEntityDeleteForm;

/**
 * The confirmation form for deleting an activity.
 */
class ActivityDeleteForm extends ContentEntityDeleteForm {

  /**
   * {@inheritdoc}
   */
  protected function getDeletionMessage() {
    $entity = $this->getEntity();
    return $this->t('@type %title has been deleted.', array(
      '%id' => $entity->id(),
      '%title' => $entity->label(),
      '@type' => $entity->get('type')->entity->label(),
    ));
  }
}

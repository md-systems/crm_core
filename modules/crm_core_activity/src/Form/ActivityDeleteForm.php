<?php

/**
 * @file
 * Contains \Drupal\crm_core_activity\Form\ActivityDeleteForm
 */

namespace Drupal\crm_core_activity\Form;

use Drupal\Core\Entity\EntityConfirmFormBase;

class ActivityDeleteForm extends EntityConfirmFormBase {

  /**
   * {@inheritdoc}
   */
  public function getQuestion() {
    return $this->t('Are you sure you want to delete activity %title?', array(
      '%title' => $this->entity->label(),
    ));
  }

  /**
   * {@inheritdoc}
   */
  public function getCancelRoute() {
    return $this->entity->urlInfo();
  }

  /**
   * {@inheritdoc}
   */
  public function getConfirmText() {
    return $this->t('Delete');
  }

  /**
   * {@inheritdoc}
   */
  public function submit(array $form, array &$form_state) {
    $this->entity->delete();
    $t_args = array(
      '%id' => $this->entity->id(),
      '%title' => $this->entity->label(),
      '@type' => $this->entity->get('type')->entity->label(),
    );
    drupal_set_message(t('@type %title has been deleted.', $t_args));
    watchdog('crm_core_activity', 'Deleted @type %title (%id).', $t_args, WATCHDOG_NOTICE);

    $form_state['redirect_route']['route_name'] = 'crm_core_activity.list';
  }
}

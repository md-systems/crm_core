<?php

/**
 * @file
 * Contains \Drupal\crm_core_activity\Form\ActivityDeleteForm
 */

namespace Drupal\crm_core_activity\Form;

use Drupal\Core\Entity\EntityConfirmFormBase;
use Drupal\Core\Form\FormStateInterface;
use Drupal\Core\Logger\RfcLogLevel;

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
  public function getCancelUrl() {
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
  public function submitForm(array &$form, FormStateInterface $form_state) {
    $this->entity->delete();
    $t_args = array(
      '%id' => $this->entity->id(),
      '%title' => $this->entity->label(),
      '@type' => $this->entity->get('type')->entity->label(),
    );
    drupal_set_message($this->t('@type %title has been deleted.', $t_args));
    \Drupal::logger('crm_core_activity')->notice('Deleted @type %title (%id).', $t_args);

    $form_state->setRedirect('entity.crm_core_activity.collection');
  }
}

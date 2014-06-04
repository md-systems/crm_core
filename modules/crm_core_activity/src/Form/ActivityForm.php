<?php

/**
 * @file
 * Contains \Drupal\crm_core_activity\Form\ActivityForm.
 */

namespace Drupal\crm_core_activity\Form;

use Drupal\Core\Entity\ContentEntityForm;

class ActivityForm  extends ContentEntityForm {

  /**
   * {@inheritdoc}
   */
  public function validate(array $form, array &$form_state) {
    parent::validate($form, $form_state);

    /* @var \Drupal\crm_core_activity\Entity\Activity $activity */
    $activity = $this->entity;

    if ($activity->get('activity_participants')->isEmpty()) {
      $message = $this->t('Participants field should include at least one participant.');
      $this->setFormError('activity_participants', $form_state, $message);
    }
  }

  /**
   * {@inheritdoc}
   */
  public function save(array $form, array &$form_state) {
    $activity = $this->entity;

    $status = $activity->save();

    $t_args = array('%title' => $activity->label());

    if ($status == SAVED_UPDATED) {
      drupal_set_message($this->t('Activity %title edited.', $t_args));
      if ($activity->access('view')) {
        $form_state['redirect_route'] = array(
          'route_name' => 'crm_core_activity.view',
          'route_parameters' => array(
            'crm_core_activity' => $activity->id(),
          ),
        );
      }
      else {
        $form_state['redirect_route']['route_name'] = 'crm_core_contact.list';
      }
    }
    elseif ($status == SAVED_NEW) {
      drupal_set_message(t('Activity %title created.', $t_args));
      watchdog('crm_core_contact', 'Activity %title created.', $t_args, WATCHDOG_NOTICE, l(t('View'), $activity->url()));
      $form_state['redirect_route']['route_name'] = 'crm_core_contact.list';
    }

    if ($activity->access('view')) {
      $form_state['redirect_route'] = array(
        'route_name' => 'crm_core_activity.view',
        'route_parameters' => array(
          'crm_core_activity' => $activity->id(),
        ),
      );
    }
    else {
      $form_state['redirect_route']['route_name'] = 'crm_core_activity.list';
    }
  }

  /**
   * {@inheritdoc}
   */
  protected function actions(array $form, array &$form_state) {
    $actions = parent::actions($form, $form_state);
    $actions['submit']['#value'] = $this->t('Save Activity');
    return $actions;
  }
}

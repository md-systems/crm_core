<?php

/**
 * @file
 * Contains \Drupal\crm_core_activity\Form\ActivityTypeForm.
 */

namespace Drupal\crm_core_activity\Form;

use Drupal\Core\Entity\EntityForm;
use Drupal\Core\Entity\EntityTypeInterface;
use Drupal\Core\Form\FormStateInterface;

/**
 * Class ActivityTypeForm
 *
 * Form for edit activity types.
 *
 * @package Drupal\crm_core_activity\Form
 */
class ActivityTypeForm extends EntityForm {

  /**
   * {@inheritdoc}
   */
  public function form(array $form, FormStateInterface $form_state) {
    $form = parent::form($form, $form_state);

    /* @var \Drupal\crm_core_activity\Entity\ActivityType $type */
    $type = $this->entity;

    $form['name'] = array(
      '#title' => $this->t('Name'),
      '#type' => 'textfield',
      '#default_value' => $type->name,
      '#description' => $this->t('The human-readable name of this activity type. It is recommended that this name begin with a capital letter and contain only letters, numbers, and spaces. This name must be unique.'),
      '#required' => TRUE,
      '#size' => 32,
    );

    $form['type'] = array(
      '#type' => 'machine_name',
      '#default_value' => $type->id(),
      '#maxlength' => EntityTypeInterface::BUNDLE_MAX_LENGTH,
//      '#disabled' => $type->isLocked(),
      '#machine_name' => array(
        'exists' => 'Drupal\crm_core_activity\Entity\ActivityType::load',
        'source' => array('name'),
      ),
      '#description' => $this->t('A unique machine-readable name for this activity type. It must only contain lowercase letters, numbers, and underscores.'),
    );

    $form['description'] = array(
      '#title' => $this->t('Description'),
      '#type' => 'textarea',
      '#default_value' => $type->description,
      '#description' => $this->t('Describe this activity type.'),
    );

    // Primary fields section.
    $form['activity_string_container'] = array(
      '#type' => 'fieldset',
      '#title' => $this->t('Display settings'),
    );

    $form['activity_string_container']['activity_string'] = array(
      '#type' => 'textfield',
      '#title' => $this->t('Activity string'),
      '#description' => $this->t('Enter text describing the relationship between the contact and this activity. For example: Someone Somewhere registered for this activity.'),
      '#default_value' => empty($type->activity_string) ? '' : $type->activity_string,
    );

    return $form;
  }

  /**
   * {@inheritdoc}
   */
  protected function actions(array $form, FormStateInterface $form_state) {
    $actions = parent::actions($form, $form_state);
    $actions['submit']['#value'] = $this->t('Save activity type');
    $actions['delete']['#title'] = $this->t('Delete activity type');
    return $actions;
  }

  /**
   * {@inheritdoc}
   */
  public function save(array $form, FormStateInterface $form_state) {
    $type = $this->entity;

    $status = $type->save();

    $t_args = array('%name' => $type->label(), 'link' => \Drupal::url('entity.crm_core_activity_type.collection'));

    if ($status == SAVED_UPDATED) {
      drupal_set_message($this->t('The activity type %name has been updated.', $t_args));
    }
    elseif ($status == SAVED_NEW) {
      drupal_set_message($this->t('The activity type %name has been added.', $t_args));
      \Drupal::logger('crm_core_activity')->notice('Added activity type %name.', $t_args);
    }

    $form_state->setRedirect('entity.crm_core_activity_type.collection');
  }
}

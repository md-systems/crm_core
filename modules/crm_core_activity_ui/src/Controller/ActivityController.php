<?php

/**
 * @file
 * Contains \Drupal\crm_core_activity_ui\Controller\ActivityController.
 */

namespace Drupal\crm_core_activity_ui\Controller;

use Drupal\Core\Controller\ControllerBase;
use Drupal\crm_core_activity\Entity\Activity;
use Drupal\crm_core_activity\Entity\ActivityType;
use Drupal\crm_core_contact\Entity\Contact;

class ActivityController extends ControllerBase {

  /**
   * Displays add activity links for available activity types.
   *
   * Show a list of activity types that can be added.
   * If only one single activity type exits, the user will be redirected to the
   * activity add form for that one activity type.
   *
   * @return array
   *   Render array containing a list of activity types that can be added.
   */
  public function addPage() {
    $activities = array();

    // Only use activity types the user has access to.
    foreach (ActivityType::loadMultiple() as $type) {
      if ($this->entityManager()->getAccessController('crm_core_activity')->createAccess($type->type)) {
        $activities[$type->type] = $type;
      }
    }

    // Bypass the listing if only one contact type is available.
    if (count($activities) == 1) {
      $type = array_shift($activities);
      return $this->redirect('crm_core_activity.add', array('crm_core_activity_type' => $type->type));
    }

    return array(
      '#theme' => 'crm_core_activity_ui_add_list',
      '#type_entities' => $activities,
    );
  }

  /**
   * Provides the node submission form.
   *
   * @param \Drupal\crm_core_activity\Entity\ActivityType $crm_core_activity_type
   *   The activity type to add.
   * @param \Drupal\crm_core_contact\Entity\Contact $crm_core_contact
   *   (optional) The contact the activity will be assigned. If left blank, the
   *   Form will show a field to select a contact.
   * @return array
   *   A node submission form.
   */
  public function add(ActivityType $crm_core_activity_type, Contact $crm_core_contact = NULL) {

    $values = array(
      'type' => $crm_core_activity_type->id(),
    );

    if ($crm_core_contact) {
      $values['activity_participants'] = array(
        array(
          'target_id' => $crm_core_contact->id(),
        ),
      );
    }

    $activity = Activity::create($values);

    $form = $this->entityFormBuilder()->getForm($activity);

    return $form;
  }

  /**
   * The title callback for the add activity form.
   *
   * @param \Drupal\crm_core_activity\Entity\ActivityType $crm_core_activity_type
   *   The activity type.
   *
   * @return string
   *   The page title.
   */
  public function addPageTitle(ActivityType $crm_core_activity_type) {
    return $this->t('Add new Activity @name', array('@name' => $crm_core_activity_type->label()));
  }
}

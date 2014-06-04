<?php

/**
 * @file
 * Contains \Drupal\crm_core_activity_ui\Controller\ActivityController.
 */

namespace Drupal\crm_core_activity_ui\Controller;

use Drupal\Core\Controller\ControllerBase;
use Drupal\crm_core_activity\Entity\ActivityType;

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
}

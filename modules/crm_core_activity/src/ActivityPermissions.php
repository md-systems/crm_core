<?php
/**
 * @file
 * Contains \Drupal\crm_core_activity\ActivityPermissions.
 */

namespace Drupal\crm_core_activity;

use Drupal\crm_core\CRMCorePermissions;

/**
 * Builds Activity permissions.
 */
class ActivityPermissions {

  /**
   * Returns Activity permissions.
   *
   * @return array
   *   CRM Core Activity permissions.
   */
  public static function permissions() {
    $perm_builder = new CRMCorePermissions();
    return $perm_builder->entityTypePermissions('crm_core_activity');
  }

}

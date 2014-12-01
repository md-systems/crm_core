<?php
/**
 * @file
 * Contains \Drupal\crm_core_contact\ContactPermissions.
 */

namespace Drupal\crm_core_contact;

use Drupal\crm_core\CRMCorePermissions;

/**
 * Class ContactPermissions.
 */
class ContactPermissions {

  /**
   * Returns Contact permissions.
   *
   * @return array
   *   CRM Core Contact permissions.
   */
  public function permissions() {
    $perm_builder = new CRMCorePermissions();

    return $perm_builder->entityTypePermissions('crm_core_contact');
  }
}

<?php

/**
 * @file
 * Contains \Drupal\crm_core_contact\ContactTypeAccessControlHandler.
 */

namespace Drupal\crm_core_contact;

use Drupal\Core\Access\AccessResult;
use Drupal\Core\Entity\EntityAccessControlHandler;
use Drupal\Core\Entity\EntityInterface;
use Drupal\Core\Session\AccountInterface;

class ContactTypeAccessControlHandler extends EntityAccessControlHandler {

  /**
   * {@inheritdoc}
   */
  protected function checkAccess(EntityInterface $entity, $operation, $langcode, AccountInterface $account) {
    /** @var \Drupal\crm_core_contact\Entity\ContactType $entity */

    // First check permission.
    if (parent::checkAccess($entity, $operation, $langcode, $account)->isForbidden()) {
      return AccessResult::forbidden();
    }

    switch ($operation) {
      case 'enable':
        // Only disabled contact type can be enabled.
        return AccessResult::forbiddenIf($entity->status());

      case 'disable':
        // Locked contact type cannot be disabled.
        return AccessResult::allowedIf($entity->status())->andIf(AccessResult::forbiddenIf($entity->isLocked()));

      case 'delete':
        // If contact type is locked, you can't delete it.
        // If contact instance of this contact type exist, you can't delete it.
        $results = \Drupal::entityQuery('crm_core_contact')
          ->condition('type', $entity->id())
          ->execute();
        return AccessResult::allowedIf(empty($results))->andIf(AccessResult::forbiddenIf($entity->isLocked()));

      case 'edit':
        // If the contact type is locked, you can't edit it.
        return AccessResult::forbiddenIf($entity->isLocked());
    }
  }
}

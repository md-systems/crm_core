<?php

/**
 * @file
 * Contains \Drupal\crm_core_activity\ActivityTypeAccessControlHandler.
 */

namespace Drupal\crm_core_activity;

use Drupal\Core\Access\AccessResult;
use Drupal\Core\Entity\EntityAccessControlHandler;
use Drupal\Core\Entity\EntityInterface;
use Drupal\Core\Session\AccountInterface;

class ActivityTypeAccessControlHandler extends EntityAccessControlHandler {

  /**
   * {@inheritdoc}
   */
  protected function checkAccess(EntityInterface $entity, $operation, AccountInterface $account) {

    // First check drupal permission.
    if (parent::checkAccess($entity, $operation, $account)->isForbidden()) {
      return AccessResult::forbidden();
    }

    switch ($operation) {
      case 'enable':
        // Only disabled activity type can be enabled.
        return AccessResult::allowedIf(!$entity->status());

      case 'disable':
        return AccessResult::allowedIf($entity->status());

      case 'delete':
        // If activity instance of this activity type exist, you can't delete it.
        $count = \Drupal::entityQuery('crm_core_activity')
          ->condition('type', $entity->id())
          ->count()
          ->execute();

        return AccessResult::allowedIf($count == 0);

      case 'update':
        return AccessResult::allowed();
    }
  }
}

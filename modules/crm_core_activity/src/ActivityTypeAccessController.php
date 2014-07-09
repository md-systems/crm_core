<?php

/**
 * @file
 * Contains \Drupal\crm_core_activity\ActivityTypeAccessController.
 */

namespace Drupal\crm_core_activity;

use Drupal\Core\Entity\EntityAccessController;
use Drupal\Core\Entity\EntityInterface;
use Drupal\Core\Session\AccountInterface;

class ActivityTypeAccessController extends EntityAccessController {

  /**
   * {@inheritdoc}
   */
  protected function checkAccess(EntityInterface $entity, $operation, $langcode, AccountInterface $account) {

    // First check drupal permission.
    if (!parent::checkAccess($entity, $operation, $langcode, $account)) {
      return FALSE;
    }

    switch ($operation) {
      case 'enable':
        // Only disabled activity type can be enabled.
        return !$entity->status();

      case 'disable':
        return $entity->status();

      case 'delete':
        // If activity instance of this activity type exist, you can't delete it.
        $count = \Drupal::entityQuery('crm_core_activity')
          ->condition('type', $entity->id())
          ->count()
          ->execute();

        return $count == 0;

      case 'update':
        return TRUE;
    }
  }
}

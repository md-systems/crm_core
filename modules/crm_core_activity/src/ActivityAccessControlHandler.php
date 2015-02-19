<?php

/**
 * @file
 * Contains \Drupal\crm_core_activity\ActivityAccessControlHandler.
 */

namespace Drupal\crm_core_activity;

use Drupal\Core\Access\AccessResult;
use Drupal\Core\Entity\EntityAccessControlHandler;
use Drupal\Core\Entity\EntityInterface;
use Drupal\Core\Session\AccountInterface;
use Drupal\crm_core_activity\Entity\ActivityType;

class ActivityAccessControlHandler extends EntityAccessControlHandler {

  /**
   * {@inheritdoc}
   */
  protected function checkAccess(EntityInterface $entity, $operation, $langcode, AccountInterface $account) {

    switch ($operation) {
      case 'view':
        return AccessResult::allowedIfHasPermission($account, 'administer crm_core_activity entities')
          ->orIf(AccessResult::allowedIfHasPermission($account, 'view any crm_core_activity entity'))
          ->orIf(AccessResult::allowedIfHasPermission($account, 'view any crm_core_activity entity of bundle ' . $entity->bundle()));

      case 'update':
        return AccessResult::allowedIfHasPermission($account, 'administer crm_core_activity entities')
          ->orIf(AccessResult::allowedIfHasPermission($account, 'edit any crm_core_activity entity'))
          ->orIf(AccessResult::allowedIfHasPermission($account, 'edit any crm_core_activity entity of bundle ' . $entity->bundle()));

      case 'delete':
        return AccessResult::allowedIfHasPermission($account, 'administer crm_core_activity entities')
          ->orIf(AccessResult::allowedIfHasPermission($account, 'delete any crm_core_activity entity'))
          ->orIf(AccessResult::allowedIfHasPermission($account, 'delete any crm_core_activity entity of bundle ' . $entity->bundle()));

      case 'create_view':
        return AccessResult::allowedIfHasPermission($account, 'administer crm_core_activity entities')
          ->orIf(AccessResult::allowedIfHasPermission($account, 'create crm_core_activity entities'));
    }
  }

  /**
   * {@inheritdoc}
   */
  protected function checkCreateAccess(AccountInterface $account, array $context, $entity_bundle = NULL) {
    $activity_type_is_active = empty($entity_bundle);

    // Load the activity type entity.
    if (!empty($entity_bundle)) {
      /* @var \Drupal\crm_core_activity\Entity\ActivityType $activity_type_entity */
      $activity_type_entity = ActivityType::load($entity_bundle);
      $activity_type_is_active = $activity_type_entity->status();
    }

    return AccessResult::allowedIf($activity_type_is_active)
      ->andIf(AccessResult::allowedIfHasPermission($account, 'administer crm_core_activity entities')
        ->orIf(AccessResult::allowedIfHasPermission($account, 'create crm_core_activity entities'))
        ->orIf(AccessResult::allowedIfHasPermission($account, 'create crm_core_activity entities of bundle ' . $entity_bundle))
      );
  }
}

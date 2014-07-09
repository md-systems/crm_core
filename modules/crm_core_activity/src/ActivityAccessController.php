<?php

/**
 * @file
 * Contains \Drupal\crm_core_activity\ActivityAccessController.
 */

namespace Drupal\crm_core_activity;

use Drupal\Core\Entity\EntityAccessController;
use Drupal\Core\Entity\EntityInterface;
use Drupal\Core\Session\AccountInterface;
use Drupal\crm_core_activity\Entity\ActivityType;

class ActivityAccessController extends EntityAccessController {

  /**
   * {@inheritdoc}
   */
  protected function checkAccess(EntityInterface $entity, $operation, $langcode, AccountInterface $account) {

    $administer_activity = $account->hasPermission('administer crm_core_activity entities');

    switch ($operation) {
      case 'view':
        $view_any_activity = $account->hasPermission('view any crm_core_activity entity');
        $view_type_activity = $account->hasPermission('view any crm_core_activity entity of bundle ' . $entity->bundle());

        return ($administer_activity || $view_any_activity || $view_type_activity);

      case 'update':
        $edit_any_activity = $account->hasPermission('edit any crm_core_activity entity');
        $edit_type_activity = $account->hasPermission('edit any crm_core_activity entity of bundle ' . $entity->bundle());

        return ($administer_activity || $edit_any_activity || $edit_type_activity);

      case 'delete':
        $delete_any_activity = $account->hasPermission('delete any crm_core_activity entity');
        $delete_type_activity = $account->hasPermission('delete any crm_core_activity entity of bundle ' . $entity->bundle());

        return ($administer_activity || $delete_any_activity || $delete_type_activity);

      case 'create_view':
        // Any of the create permissions.
        $create_any_activity = $account->hasPermission('create crm_core_activity entities');
        return ($administer_activity || $create_any_activity);
    }
  }

  /**
   * {@inheritdoc}
   */
  protected function checkCreateAccess(AccountInterface $account, array $context, $entity_bundle = NULL) {
    $administer_activity = $account->hasPermission('administer crm_core_activity entities');
    $create_any_activity = $account->hasPermission('create crm_core_activity entities');
    $create_type_activity = $account->hasPermission('create crm_core_activity entities of bundle ' . $entity_bundle);

    $activity_type_is_active = empty($entity_bundle);

    // Load the activity type entity.
    if (!empty($entity_bundle)) {
      /* @var \Drupal\crm_core_activity\Entity\ActivityType $activity_type_entity */
      $activity_type_entity = ActivityType::load($entity_bundle);
      $activity_type_is_active = $activity_type_entity->status();
    }

    return (($administer_activity || $create_any_activity || $create_type_activity) && $activity_type_is_active);
  }
}

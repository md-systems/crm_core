<?php

/**
 * @file
 * Contains \Drupal\crm_core_contact\ContactAccessControlHandler.
 */

namespace Drupal\crm_core_contact;

use Drupal\Core\Access\AccessResult;
use Drupal\Core\Entity\EntityAccessControlHandler;
use Drupal\Core\Entity\EntityInterface;
use Drupal\Core\Session\AccountInterface;
use Drupal\crm_core_contact\Entity\ContactType;

/**
 * Class ContactAccessController.
 */
class ContactAccessControlHandler extends EntityAccessControlHandler {

  /**
   * {@inheritdoc}
   */
  protected function checkAccess(EntityInterface $entity, $operation, $langcode, AccountInterface $account) {

    $allowed_if_admin = AccessResult::allowedIfHasPermission($account, 'administer crm_core_contact entities');

    switch ($operation) {
      case 'view':
        return $allowed_if_admin->orIf(AccessResult::allowedIfHasPermissions($account, ['view any crm_core_contact entity', 'view any crm_core_contact entity of bundle ' . $entity->bundle()]));

      case 'update':
        return $allowed_if_admin->orIf(AccessResult::allowedIfHasPermissions($account, ['edit any crm_core_contact entity', 'edit any crm_core_contact entity of bundle ' . $entity->bundle()]));

      case 'delete':
        return $allowed_if_admin->orIf(AccessResult::allowedIfHasPermissions($account, ['delete any crm_core_contact entity', 'delete any crm_core_contact entity of bundle ' . $entity->bundle()]));

      case 'revert':
        // @todo: more fine grained will be adjusting dynamic permission
        // generation for reverting bundles of contact.
        return $allowed_if_admin->orIf(AccessResult::allowedIfHasPermission($account, 'revert contact record'));

      // @todo This operation should be renamed or even deleted(because we have ContactAccessControlHandler::checkCreateAccess()).
      case 'create_view':
        // Any of the create permissions.
        return $allowed_if_admin->orIf(AccessResult::allowedIfHasPermission($account, 'create crm_core_contact entities'));
    }
  }

  /**
   * {@inheritdoc}
   */
  protected function checkCreateAccess(AccountInterface $account, array $context, $entity_bundle = NULL) {
    $contact_type_is_active = empty($entity_bundle);

    // Load the contact type entity.
    if (!empty($entity_bundle)) {
      /* @var \Drupal\crm_core_contact\Entity\ContactType $contact_type_entity */
      $contact_type_entity = ContactType::load($entity_bundle);
      $contact_type_is_active = $contact_type_entity->status();
    }

    return AccessResult::allowedIf($contact_type_is_active)
      ->andIf(AccessResult::allowedIfHasPermissions($account, [
        'administer crm_core_contact entities',
        'create crm_core_contact entities',
        'create crm_core_contact entities of bundle ' . $entity_bundle,
      ], 'OR'));
  }
}

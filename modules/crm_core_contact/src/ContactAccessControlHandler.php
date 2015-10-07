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
 * Access control handler for CRM Core Contact entities.
 */
class ContactAccessControlHandler extends EntityAccessControlHandler {

  /**
   * {@inheritdoc}
   */
  protected function checkAccess(EntityInterface $entity, $operation, AccountInterface $account) {

    switch ($operation) {
      case 'view':
        return AccessResult::allowedIfHasPermissions($account, [
          'administer crm_core_contact entities',
          'view any crm_core_contact entity',
          'view any crm_core_contact entity of bundle ' . $entity->bundle(),
        ], 'OR');

      case 'update':
        return AccessResult::allowedIfHasPermissions($account, [
          'administer crm_core_contact entities',
          'edit any crm_core_contact entity',
          'edit any crm_core_contact entity of bundle ' . $entity->bundle(),
        ], 'OR');

      case 'delete':
        return AccessResult::allowedIfHasPermissions($account, [
          'administer crm_core_contact entities',
          'delete any crm_core_contact entity',
          'delete any crm_core_contact entity of bundle ' . $entity->bundle(),
        ], 'OR');

      case 'revert':
        // @todo: more fine grained will be adjusting dynamic permission
        // generation for reverting bundles of contact.
        return AccessResult::allowedIfHasPermissions($account, [
          'administer crm_core_contact entities',
          'revert contact record',
        ], 'OR');
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

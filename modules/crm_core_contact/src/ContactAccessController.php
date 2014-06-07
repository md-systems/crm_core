<?php

/**
 * @file
 * Contains \Drupal\crm_core_contact\ContactAccessController.
 */

namespace Drupal\crm_core_contact;

use Drupal\Core\Entity\EntityAccessController;
use Drupal\Core\Entity\EntityInterface;
use Drupal\Core\Session\AccountInterface;
use Drupal\crm_core_contact\Entity\ContactType;

class ContactAccessController extends EntityAccessController {

  /**
   * {@inheritdoc}
   */
  protected function checkAccess(EntityInterface $entity, $operation, $langcode, AccountInterface $account) {

    $administer_contact = $account->hasPermission('administer crm_core_contact entities');

    switch ($operation) {
      case 'view':
        $view_any_contact = $account->hasPermission('view any crm_core_contact entity');
        $view_type_contact = $account->hasPermission('view any crm_core_contact entity of bundle ' . $entity->bundle());

        return ($administer_contact || $view_any_contact || $view_type_contact);

      case 'edit':
        $edit_any_contact = $account->hasPermission('edit any crm_core_contact entity');
        $edit_type_contact = $account->hasPermission('edit any crm_core_contact entity of bundle ' . $entity->bundle());

        return ($administer_contact || $edit_any_contact || $edit_type_contact);

      case 'delete':
        $delete_any_contact = $account->hasPermission('delete any crm_core_contact entity');
        $delete_type_contact = $account->hasPermission('delete any crm_core_contact entity of bundle ' . $entity->bundle());

        return ($administer_contact || $delete_any_contact || $delete_type_contact);

      case 'revert':
        // @todo: more fine grained will be adjusting dynamic permission
        // generation for reverting bundles of contact.
        $revert_any_contact = $account->hasPermission('revert contact record');

        return ($administer_contact || $revert_any_contact);

      case 'create_view':
        // Any of the create permissions.
        $create_any_contact = $account->hasPermission('create crm_core_contact entities');
        return ($administer_contact || $create_any_contact);

      case 'create':
      default:
        return $this->createAccess($entity->bundle(), $account);
    }
  }

  /**
   * {@inheritdoc}
   */
  protected function checkCreateAccess(AccountInterface $account, array $context, $entity_bundle = NULL) {

    $administer_contact = $account->hasPermission('administer crm_core_contact entities');

    // Must be able to create contact of any type (OR) specific type
    // (AND) have an active contact type.
    // IMPORTANT, here $contact is padded in as a string of the contact type.
    $create_any_contact = $account->hasPermission('create crm_core_contact entities');
    $create_type_contact = $account->hasPermission('create crm_core_contact entities of bundle ' . $entity_bundle);

    $contact_type_is_active = empty($entity_bundle);

    // Load the contact type entity.
    if (!empty($entity_bundle)) {
      /* @var \Drupal\crm_core_contact\Entity\ContactType $contact_type_entity */
      $contact_type_entity = ContactType::load($entity_bundle);
      $contact_type_is_active = $contact_type_entity->status();
    }

    return (($administer_contact || $create_any_contact || $create_type_contact) && $contact_type_is_active);
  }
}

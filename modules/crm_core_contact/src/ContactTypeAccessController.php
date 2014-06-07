<?php

/**
 * @file
 * Contains \Drupal\crm_core_contact\ContactTypeAccessController.
 */

namespace Drupal\crm_core_contact;

use Drupal\Core\Entity\EntityAccessController;
use Drupal\Core\Entity\EntityInterface;
use Drupal\Core\Session\AccountInterface;

class ContactTypeAccessController extends EntityAccessController {

  /**
   * {@inheritdoc}
   */
  protected function checkAccess(EntityInterface $entity, $operation, $langcode, AccountInterface $account) {

    // First check drupal permission.
    if (!$account->hasPermission('administer contact types')) {
      return FALSE;
    }

    switch ($operation) {
      case 'enable':
        // Only disabled contact type can be enabled.
        return !$entity->status();

      case 'disable':
        // Locked contact type cannot be disabled.
        if ($entity->locked) {
          return FALSE;
        }
        return $entity->status();

      case 'delete':
        // If contact type is locked, you can't delete it.
        if ($entity->locked) {
          return FALSE;
        }
        // If contact instance of this contact type exist, you can't delete it.
        $results = \Drupal::entityQuery('crm_core_contact')
          ->condition('type', $entity->id())
          ->execute();

        return empty($results);

      case 'edit':
      default:
        // If the contact type is locked, you can't edit it.
        return !$entity->locked;
    }
  }

  /**
   * {@inheritdoc}
   */
  protected function checkCreateAccess(AccountInterface $account, array $context, $entity_bundle = NULL) {
    return $account->hasPermission('administer contact types');
  }
}

<?php
/**
 * @file
 * Contains \Drupal\crm_core_match\Matcher\MatcherAccessControlHandler.
 */

namespace Drupal\crm_core_match\Matcher;

use Drupal\Core\Access\AccessResult;
use Drupal\Core\Entity\EntityAccessControlHandler;
use Drupal\Core\Entity\EntityInterface;
use Drupal\Core\Session\AccountInterface;

/**
 * Controls access to matchers.
 */
class MatcherAccessControlHandler extends EntityAccessControlHandler {

  /**
   * {@inheritdoc}
   */
  protected function checkAccess(EntityInterface $entity, $operation, $langcode, AccountInterface $account) {
    /** @var \Drupal\crm_core_match\Matcher\MatcherConfigInterface $entity */
    return parent::checkAccess($entity, $operation, $langcode, $account);
    // Deny delete access.
    // ->andIf(AccessResult::allowedIf($operation != 'delete'));
  }

}

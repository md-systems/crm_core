<?php
/**
 * @file
 * Contains \Drupal\crm_core_activity\ActivityInterface.
 */

namespace Drupal\crm_core_activity;

use Drupal\Core\Entity\ContentEntityInterface;
use Drupal\Core\Entity\EntityChangedInterface;
use Drupal\crm_core_contact\ContactInterface;

/**
 * Defines methods for CRM Activity entities.
 */
interface ActivityInterface extends ContentEntityInterface, EntityChangedInterface {

  /**
   * Add a participant to the activity.
   *
   * @param \Drupal\crm_core_contact\ContactInterface $contact
   *   The contact to add as a participant.
   *
   * @return $this
   */
  public function addParticipant(ContactInterface $contact);

}

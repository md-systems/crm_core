<?php

/**
 * @file
 * Contains \Drupal\crm_core_match\MatchEngineInterface.
 */

namespace Drupal\crm_core_match;

use Drupal\crm_core_contact\Entity\Contact;

/**
 * Interface for matching engines
 *
 * CRM Core matching engines can implement this interface.
 */
interface MatchEngineInterface {

  /**
   * Finds matches for given contact.
   *
   * @param \Drupal\crm_core_contact\Entity\Contact $contact
   *   A contact entity used to pass data for identifying a match.
   *
   * @return int[]
   *   An array of entity ids for potential matches.
   */
  public function match(Contact $contact);
}

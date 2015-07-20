<?php

/**
 * @file
 * Contains \Drupal\crm_core_match\Plugin\crm_core_match\engine\MatchEngineInterface.
 */

namespace Drupal\crm_core_match\Plugin\crm_core_match\engine;

use Drupal\crm_core_contact\ContactInterface;
use Drupal\crm_core_contact\Entity\Contact;

/**
 * Interface for matching engines
 *
 * CRM Core matching engines can implement this interface.
 *
 * @todo extend PluginInspectionInterface, use PluginBase
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
  public function match(ContactInterface $contact);
}

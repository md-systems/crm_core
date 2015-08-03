<?php

/**
 * @file
 * Contains \Drupal\crm_core_match\Plugin\crm_core_match\engine\MatchEngineInterface.
 */

namespace Drupal\crm_core_match\Plugin\crm_core_match\engine;

use Drupal\Component\Plugin\PluginInspectionInterface;
use Drupal\Core\Plugin\PluginFormInterface;
use Drupal\crm_core_contact\ContactInterface;

/**
 * Interface for matching engines.
 *
 * CRM Core matching engines can implement this interface.
 */
interface MatchEngineInterface extends PluginInspectionInterface, PluginFormInterface {

  /**
   * Finds matches for given contact.
   *
   * @param \Drupal\crm_core_contact\ContactInterface $contact
   *   A contact entity used to pass data for identifying a match.
   *
   * @return int[]
   *   An array of entity ids for potential matches.
   */
  public function match(ContactInterface $contact);

}

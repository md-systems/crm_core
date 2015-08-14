<?php

/**
 * @file
 * Contains \Drupal\crm_core_match\Plugin\crm_core_match\engine\MatchEngineInterface.
 */

namespace Drupal\crm_core_match\Plugin\crm_core_match\engine;

use Drupal\Component\Plugin\ConfigurablePluginInterface;
use Drupal\Component\Plugin\PluginInspectionInterface;
use Drupal\Core\Plugin\PluginFormInterface;
use Drupal\crm_core_contact\ContactInterface;

/**
 * Interface for matching engines.
 *
 * CRM Core matching engines can implement this interface.
 */
interface MatchEngineInterface extends PluginInspectionInterface, PluginFormInterface, ConfigurablePluginInterface {

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

  /**
   * Returns a specific item of this plugin's configuration.
   *
   * @param string|array $key
   *   The key of the item to get, or an array of nested keys.
   *
   * @return mixed
   *   An item of this plugin's configuration.
   */
  public function getConfigurationItem($key);

  /**
   * Gets the rules that are matched.
   *
   * By default those are the contact type fields.
   *
   * @todo Extend with typed data definition to limit selections.
   *
   * Example data:
   * @code
   * (
   *   field_name => array(
   *     label,
   *     definition,
   * ),
   * @endcode
   *
   * @return mixed
   */
  public function getRules();

}

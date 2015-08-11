<?php
/**
 * @file
 * Contains \Drupal\crm_core_match\Matcher\MatcherConfigInterface.
 */

namespace Drupal\crm_core_match\Matcher;

use Drupal\Core\Config\Entity\ConfigEntityInterface;
use Drupal\crm_core_contact\ContactInterface;

/**
 * Interface MatcherConfigInterface.
 */
interface MatcherConfigInterface extends ConfigEntityInterface {

  /**
   * Gets the matcher plugin.
   *
   * @return \Drupal\crm_core_match\Plugin\crm_core_match\engine\MatchEngineInterface.
   *   Instantiated plugin.
   */
  public function getPlugin();

  /**
   * Gets the description.
   *
   * @return string
   *   The description of this matcher.
   */
  public function getDescription();

  /**
   * Gets plugin title.
   *
   * @return string
   *   Plugin title.
   */
  public function getPluginTitle();

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
   * Returns plugin configuration.
   *
   * @return array
   *   Configuration as an array.
   */
  public function getConfiguration();

}

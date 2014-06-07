<?php
/**
 * @file
 * Contains \Drupal\crm_core_match\MatcherInterface.
 */

namespace Drupal\crm_core_match;

interface MatcherInterface {

  /**
   * Returns the sorted array of match engines.
   *
   * @return \Drupal\crm_core_match\Plugin\crm_core_match\engine\MatchEngineInterface[]
   *   An array of match engine objects.
   */
  public function getEngines();
}

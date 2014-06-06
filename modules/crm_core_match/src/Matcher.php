<?php

/**
 * @file
 * Contains \Drupal\crm_core_match\Matcher.
 */

namespace Drupal\crm_core_match;

use Drupal\crm_core_contact\Entity\Contact;

class Matcher implements MatchEngineInterface, MatcherInterface {

  /**
   * Array of all registered match engines, keyed by ID.
   *
   * @var \Drupal\crm_core_match\MatchEngineInterface[]
   */
  protected $engines;

  /**
   * Array of all engines and their priority.
   *
   * @var array
   */
  protected $engineOrders = array();

  /**
   * Sorted list of registered engines.
   *
   * @var \Drupal\crm_core_match\MatchEngineInterface[]
   */
  protected $sortedEngines;

  /**
   * {@inheritdoc}
   */
  public function addMatchEngine($engine_id, MatchEngineInterface $engine, $priority = 0) {
    $this->engines[$engine_id] = $engine;
    $this->engineOrders[$priority][$engine_id] = $engine;
    // Force the builders to be re-sorted.
    $this->sortedEngines = NULL;
  }

  /**
   * {@inheritdoc}
   */
  public function getSortedEngines() {
    if (!isset($this->sortedEngines)) {
      // Sort the builders according to priority.
      krsort($this->engineOrders);
      // Merge nested engines from $this->engines into $this->sortedEngines.
      $this->sortedEngines = array();
      foreach ($this->engineOrders as $engines) {
        $this->sortedEngines = array_merge($this->sortedEngines, $engines);
      }
    }
    return $this->sortedEngines;
  }

  /**
   * Finds matches for given contact.
   *
   * Loops over all registered match engines and returns the aggregated matches.
   *
   * @param \Drupal\crm_core_contact\Entity\Contact $contact
   *   A contact entity used to pass data for identifying a match.
   *
   * @return int[]
   *   An array of entity ids for potential matches.
   *
   * @todo Check engine status. Skip disabled engines.
   */
  public function match(Contact $contact) {
    $ids = array();

    foreach ($this->getSortedEngines() as $engine) {
      $ids = array_merge($ids, $engine->match($contact));
    }

    return array_unique($ids);
  }
}

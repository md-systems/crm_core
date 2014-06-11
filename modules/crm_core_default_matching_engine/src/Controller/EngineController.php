<?php

/**
 * @file
 * Contains \Drupal\crm_core_default_matching_engine\Controller\EngineController.
 */

namespace Drupal\crm_core_default_matching_engine\Controller;

use Drupal\Core\Controller\ControllerBase;
use Drupal\crm_core_default_matching_engine\Entity\MatchingRule;

class EngineController extends ControllerBase {

  /**
   * Displays links to the per contact type rules config forms.
   */
  public function configPage() {
    $rules = array();

    // Only use node types the user has access to.
    foreach ($this->entityManager()->getStorage('crm_core_default_engine_rule')->loadMultiple() as $rule) {
      $rules[$rule->id()] = $rule;
    }

    // Bypass the listing if only one content type is available.
    if (count($rules) == 1) {
      $rule = array_shift($rules);
      return $this->redirect('crm_core_default_matching_engine.rule_edit', array(
        'crm_core_default_engine_rule' => $rule->id())
      );
    }

    return array(
      '#theme' => 'crm_core_default_matching_engine_config_page',
      '#matching_rule_entities' => $rules,
    );
  }

  /**
   * Gets the edit page title.
   *
   * @param \Drupal\crm_core_default_matching_engine\Entity\MatchingRule $crm_core_default_engine_rule
   *   The edited rule.
   *
   * @return string
   *   The page title.
   */
  public function editTitle(MatchingRule $crm_core_default_engine_rule) {
    return $crm_core_default_engine_rule->label();
  }
}

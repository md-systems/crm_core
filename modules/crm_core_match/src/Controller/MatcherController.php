<?php

/**
 * @file
 * Contains \Drupal\crm_core_match\Controller\MatcherController.
 */

namespace Drupal\crm_core_match\Controller;

use Drupal\Core\Controller\ControllerBase;
use Drupal\crm_core_match\Matcher\MatcherConfigInterface;

/**
 * Class MatcherController.
 */
class MatcherController extends ControllerBase {

  /**
   * Gets the edit matcher title.
   *
   * @param \Drupal\crm_core_match\Matcher\MatcherConfigInterface $crm_core_match
   *   The edited matcher.
   *
   * @return string
   *   The page title.
   */
  public function editTitle(MatcherConfigInterface $crm_core_match) {
    return $this->t('Edit %matcher matcher', array(
      '%matcher' => $crm_core_match->label(),
    ));
  }

}

<?php
/**
 * @file
 * Contains Drupal\crm_core_default_matching_engine\Plugin\crm_core\MatchField\UnsupportedField.
 */

namespace Drupal\crm_core_default_matching_engine\Plugin\crm_core\MatchField;

use Drupal\crm_core_default_matching_engine\Plugin\DefaultMatchingEngineFieldType;

class UnsupportedField extends DefaultMatchingEngineFieldType {

  public function operators() {
    return array();
  }
}

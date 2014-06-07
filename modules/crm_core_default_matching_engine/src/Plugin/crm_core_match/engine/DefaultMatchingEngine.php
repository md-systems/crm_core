<?php

/**
 * @file
 * Contains \Drupal\crm_core_default_matching_engine\Plugin\crm_core_match\engine\DefaultMatchingEngine.
 */

namespace Drupal\crm_core_default_matching_engine\Plugin\crm_core_match\engine;

use Drupal\crm_core_contact\Entity\Contact;
use Drupal\crm_core_match\Plugin\crm_core_match\engine\MatchEngineBase;

if (!defined('MATCH_DEFAULT_CHARS')) {
  define('MATCH_DEFAULT_CHARS', '3');
}


/**
 * DefaultMatchingEngine class
 *
 * Extends CrmCoreMatchEngine to provide rules for identifying duplicate
 * contacts.
 *
 * @CrmCoreMatchEngine(
 *   id = "default",
 *   title = @Translation("Default Matching Engine"),
 *   description = @Translation("This is a simple matching engine from CRM Core. Allows administrators to specify matching rules for individual contact types on a field-by-field basis."),
 *   priority = 0,
 *   settings = {
 *     "settings" = {
 *       "route" = "crm_core_default_matching_engine.config",
 *       "label" = @Translation("Configuration")
 *     }
 *   }
 * )
 */
class DefaultMatchingEngine extends MatchEngineBase {

  /**
   * {@inheritdoc}
   */
  public function match(Contact $contact) {
    $ids = array();
    $base_config = crm_core_default_matching_engine_load_contact_type_config($contact->type);
    // Check if match is enabled for this contact type.
    if ($base_config['status']) {
      $matching_rules = crm_core_default_matching_engine_load_field_config($contact->type);
      $contact_fields = field_info_instances('crm_core_contact', $contact->type);

      $results = array();
      foreach ($matching_rules as $matching_rule) {
        if (isset($contact_fields[$matching_rule->field_name])) {
          $rule_matches = array();
          $field_match_handler_class = $matching_rule->field_type . 'MatchField';
          if (class_exists($field_match_handler_class)) {
            $field_match_handler = new $field_match_handler_class();
            $rule_matches = $field_match_handler->fieldQuery($contact, $matching_rule);
          }
          foreach ($rule_matches as $matched_id) {
            $results[$matched_id][$matching_rule->mrid] = $matching_rule->score;
          }
        }
      }
      foreach ($results as $id => $rule_matches) {
        $total_score = array_sum($rule_matches);
        if ($total_score >= $base_config['threshold']) {
          $ids[] = $id;
        }
      }
    }
    return $ids;
  }
}

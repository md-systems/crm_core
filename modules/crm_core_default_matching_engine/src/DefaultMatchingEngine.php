<?php

/**
 * @file
 * Default match engine definitions.
 */

if (!defined('MATCH_DEFAULT_CHARS')) {
  define('MATCH_DEFAULT_CHARS', '3');
}

/**
 * DefaultMatchingEngine class
 *
 * Extends CrmCoreMatchEngine to provide rules for identifying duplicate contacts.
 */
class DefaultMatchingEngine extends CrmCoreMatchEngine {

  /**
   * Constructor: sets basic variables.
   */
  public function __construct() {
    $this->name = t('Default Matching Engine');
    $this->machineName = 'default_matching_engine';
    $description = 'This is a simple matching engine from CRM Core. Allows administrators to specify matching'
      . ' rules for individual contact types on a field-by-field basis.';
    $this->description = t($description);
    $this->settings = array(
      array(
        'name' => 'settings',
        'path' => 'admin/config/crm-core/match/default_match',
        'label' => t('Configuration'),
      ),
    );
  }

  /**
   * Applies logical rules for identifying matches in the database.
   *
   * Any matching engine should implement this to apply it's unique matching logic.
   * Variables are passed in by reference, so it's not necessary to return anything.
   * Accepts a list of matches and contact information to identify potential duplicates.
   *
   * @see CrmCoreMatchEngineInterface::execute()
   */
  public function execute(&$contact, &$ids = array()) {
    if ($this->status) {
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
    }
  }
}

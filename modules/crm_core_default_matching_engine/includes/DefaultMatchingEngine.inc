<?php

/**
 * @file
 * Default match engine definitions.
 */

/**
 * Interface for defining the logical operators and query criteria used to identify duplicate contacts based on
 * different field types in DefaultMatchingEngine.
 */
interface DefaultMatchingEngineFieldTypeInterface {

  /**
   * Field Renderer.
   *
   * Used for complex field types such as name.
   * Renders them into component parts for use in applying logical operators and ordering functions.
   *
   * @param array $field
   *   The field being rendered
   *
   * @param array $field_info
   *   Info of the field  being rendered
   */
  public function fieldRender($field, $field_info, &$form);

  /**
   * Operators.
   *
   * Defines the logical operators that can be used by this field type.
   * Provides any additional fields needed to capture information used in logical evaluations.
   *
   * For instance: if this was a text field, there might be 3 logical operators: EQUALS, STARTS WITH, and ENDS WITH.
   * This function should return a select list with the operator values, and a text field to be used to enter
   * something like 'first 3'.
   */
  public function operators();

  /**
   * Query.
   *
   * Used when generating queries to identify matches in the system
   */
  public function fieldQuery();
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
    $this->description = t('This is a simple matching engine from CRM Core. Allows administrators to specify matching'
      . ' rules for individual contact types on a field-by-field basis.');
    $this->settings = array();
  }

  /**
   * Applies logical rules for identifying matches in the database.
   *
   * Any matching engine should implement this to apply it's unique matching logic.
   * Variables are passed in by reference, so it's not necessary to return anything.
   *
   * Accepts a list of matches and contact information to identify potential duplicates.
   *
   * @see CrmCoreMatchEngineInterface::execute()
   */
  public function execute(&$contact, &$ids = array()) {
// @TODO we executing engine, it loads list of rules for this contact type, in weighted order executes a
// a rule queries and summing results by rules, if sum is greater then Threshold , then contact ids from rules
// queries results pushed to matches.

    $base_config = crm_core_default_matching_engine_load_contact_type_config($contact->type);
    dpm($base_config, 'Base contact type config');
    // Check if match is enabled for this contact type.
    if ($base_config['status']) {

      // This returns a list of all fields associated with the selected contact type.
      foreach (field_info_instances('crm_core_contact', $contact->type) as $field_name => $field) {

        // This returns some additional information about the field.
        $field_info = field_info_field($field_name);

        $field_type = $field_info['type'];
        // We now have enough information about the field to render a row for it. Some fields are going to have multiple
        // rows (for instance, name fields) and need to be handled a little differently.
        $field_match_handler_class = $field_type . 'MatchField';

        if (class_exists($field_match_handler_class)) {
          // Special handling for name fields.
          if ($field_type == 'name') {

          }
          $field_match_handler = new $field_match_handler_class();

          $field_rule = crm_core_default_matching_engine_load_field_config($contact->type, $field_type);
          dpm($field_rule, $field_name);
          dpm($field_type,$field_type);
        }
      }


    }
  }
}

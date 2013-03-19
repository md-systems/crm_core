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
    $this->name = 'Default Matching Engine';
    $this->machine_name = 'default_matching_engine';
    $this->description = 'This is a simple matching engine from CRM Core. Allows administrators to specify matching rules'
      . ' for individual contact types on a field-by-field basis.';
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
  public function execute(&$contactData, &$ids = array()) {
    dpm('yo yo yo');
  }
}

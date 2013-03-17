<?php 

/*
 * Interface for defining the logical operators and query criteria used to identify
 * duplicate contacts based on different field types in DefaultMatchingEngine.
 */
interface DefaultMatchingEngineFieldType {

  /**
   * Field Render
   * 
   * Used for complex field types such as name.
   * Renders them into component parts for use in applying logical 
   * operators and ordering functions.
   */
  public function fieldRender();
  
  /**
   * Operators
   * 
   * Defines the logical operators that can be used by this field type.
   * Provides any additional fields needed to capture information used 
   * in logical evaluations.
   * 
   * For instance: if this was a text field, there might be 3 logical 
   * operators: EQUALS, STARTS WITH, and ENDS WITH. This function should
   * return a select list with the operator values, and a text field
   * to be used to enter something like 'first 3'.
   */
  public function operators();
  
  /**
   * Query
   * 
   * Used when generating queries to identify matches in the system
   * 
   */
  public function fieldQuery();
}

/**
 * Class for extending DefaultMatchingEngineFieldType
 */
class CrmCoreMatchField implements DefaultMatchingEngineFieldType {
}

/**
 * DefaultMatchingEngine class
 * 
 * Extends CrmCoreMatchEngine to provide rules for identifying duplicate contacts.
 */
class DefaultMatchingEngine extends CrmCoreMatchEngine {
  
  /**
   * When processing a match, this module takes contact information being 
   * passed into CRM Core and looks for duplicate contacts using information
   * configured for each contact type.
   * 
   * @see CrmCoreMatchEngine::execute()
   */
  public function execute(&$contactData, &$ids = array()){
    dpm('yo yo yo');
  }

}




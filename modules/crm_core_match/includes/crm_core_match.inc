<?php

/**
 * @file
 * Contains the matching engine.
 */

/*
 * Interface for matching engines
 * 
 * CRM Core matching engines can implement this interface.
 * 
 */
interface CrmCoreMatchEngineInterface {

  /**
   * Constructor.
   */
  public function __construct();

  /**
   * Returns information about the matching engine to use
   * in identifying the engine
   */
  public function getEngineInfo();
  
  /**
   * This function will be executed by CRM Core Match each time 
   * the engine is executed.
   * 
   * Accepts contact data and a list of ids for identifying matches.
   * 
   * @param object $contactData: a contact object used to pass data for identifying a match
   * @param array $ids: an array of entity ids for potential matches.
   * 
   */
  public function execute(&$contactData, &$ids = array());
  
}

/**
 * Default implementation of CrmCoreMatchEngineInterface
 *
 * Safe for use by most matching engines.
 *
 */
class CrmCoreMatchEngine implements CrmCoreMatchEngineInterface {
  
  /**
   * The human readable name for the matching engine
   *
   * @var $name string 
   * 
   */
  protected $name;
  
  /**
   * The machine name used for the matching engine
   *
   * @var $machine_name string 
   * 
   */
  protected $machine_name;
  
  /**
   * A short description of what the matching engine does.
   *
   * @var $name string 
   * 
   */
  protected $description;
  
  /**
   * An array listing settings pages for the matching engine.
   *
   * @var $settings array
   * 
   */
  protected $settings;
  
  /**
   * Constructor: sets basic variables.
   */
  public function __construct() {
//    $this->name = '';
//    $this->machine_name = '';
//    $this->description = '';
//    $this->settings = array();
//@TODO: Remove this when finish with testing and uncomment above.
    $this->name = '(test) CRM Core Match Engine';
    $this->machine_name = 'test_matching_engine';
    $this->description = 'This is just a test, to see how matching engines show up in CRM Core Match. Remove this from CRM Core Match before the module is launched.';
    $this->settings = array(
      array('path' => '<front>', 'label' => 'Test Settings Page', 'name' => 'settings'),
      array('path' => '<front>', 'label' => 'Other Test Settings Page', 'name' => 'settings2'),
    );
  }  
  
  /**
   * Returns basic information about the matching engine
   * @see CrmCoreMatchEngineInterface::getEngineInfo
   */
  public function getEngineInfo() {
    return array(
      'name' => $this->name,
      'machine_name' => $this->machine_name,
      'description' => $this->description,
      'settings' => $this->settings,
    );
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
  public function execute(&$contactData, &$ids = array()){
    return FALSE;
  }
  
}


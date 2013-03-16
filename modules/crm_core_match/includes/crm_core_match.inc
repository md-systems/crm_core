<?php


dpm('yo yo yo');

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
  public function __construct($entityType);

  /**
   * Returns information about the matching engine to use
   * in identifying the engine
   */
  public function matchEngineInfo();
  
  /**
   * This function will be executed by CRM Core Match each time 
   * the engine is executed.
   * 
   * Accepts contact data and a list of ids for identifying matches.
   * 
   */
  public function execute();
  
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
  protected $name = '';
  
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
   * An object containing information used to describe the engine.
   * 
   * @var $engineInfo object
   */
  protected $engineInfo;
  
  
  /**
   * Constructor: sets basic variables.
   */
  public function __construct($engineInfo) {
    $this->name = $engineInfo->name;
    $this->machine_name = $engineInfo->machine_name;
    $this->description = $engineInfo->description;
    $this->settings = $engineInfo->settings;
  }  
  
  /**
   * Returns basic information about the matching engine
   * @see CrmCoreMatchEngineInterface::matchEngineInfo()
   */
  public function matchEngineInfo() {
    $info = new stdClass();
    $info->name = $this->name;
    $info->machine_name = $this->machine_name;
    $info->description = $this->description;
    $info->settings = $this->settings;
    return $info;
  }
  
  /**
   * Applies logical rules for identifying matches in the database.
   * 
   * Any matching engine should implement this to apply it's unique matching logic.
   * Variables are passed in by reference, so it's not necessary to return anything.
   * 
   * Accepts a list of matches and contact information to identify potential duplicates.
   * 
   * @param object $contactData: a contact object used to pass data for identifying a match
   * @param array $ids: an array of entity ids for potential matches.
   * 
   * @see CrmCoreMatchEngineInterface::execute()
   */
  public function execute(&$contactData, &$ids = array()){
    
    
    
  }
  
  
}


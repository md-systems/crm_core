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
   * Returns information about the matching engine to use in identifying the engine.
   */
  public function getEngineInfo();

  /**
   * This function will be executed by CRM Core Match each time the engine is executed.
   *
   * Accepts contact data and a list of ids for identifying matches.
   *
   * @param object $contact
   *   A contact object used to pass data for identifying a match
   * @param array $ids
   *   An array of entity ids for potential matches.
   */
  public function execute(&$contact, &$ids = array());
}

/**
 * Default implementation of CrmCoreMatchEngineInterface
 *
 * Safe for use by most matching engines.
 */
abstract class CrmCoreMatchEngine implements CrmCoreMatchEngineInterface {

  /**
   * The human readable name for the matching engine
   *
   * @var $name string
   */
  protected $name;

  /**
   * The machine name used for the matching engine
   *
   * @var $machine_name string
   */
  protected $machine_name;

  /**
   * A short description of what the matching engine does.
   *
   * @var $name string
   */
  protected $description;

  /**
   * An array listing settings pages for the matching engine.
   *
   * @var $settings array
   *
   * Example structure:
   * @code
   * $settings = array(
   *  array(
   *   'name' => 'settings', // Machine readable settings page name.
   *   'path' => '<front>', // Internal path to settings page.
   *   'label' => 'Settings page', // Untranslated label for link.
   *   ),
   * );
   * @endcode
   */
  protected $settings;

  /**
   * Constructor: sets basic variables.
   */
  public function __construct() {
    $this->name = '';
    $this->machine_name = '';
    $this->description = '';
    $this->settings = array();
  }

  /**
   * Returns basic information about the matching engine.
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
  public function execute(&$contact, &$ids = array()) {
    return FALSE;
  }
}

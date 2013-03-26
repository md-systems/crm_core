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
// @TODO: Pull all method definitions to interface.
  /**
   * Constructor.
   */
  public function __construct();

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
   * @var string $name
   */
  protected $name;

  /**
   * The machine name used for the matching engine
   *
   * @var string $machineName
   */
  protected $machineName;

  /**
   * A short description of what the matching engine does.
   *
   * @var string $description
   */
  protected $description;

  /**
   * Engine weight when applying matching.
   *
   * Configured from UI and stored in DB.
   *
   * @var int $weight
   */
  protected $weight;

  /**
   * Engine status.
   *
   * Configured from UI and stored in DB.
   *
   * @var bool $status
   */
  protected $status;

  /**
   * Engine ID.
   *
   * Stored in DB. Used for debug purposes only.
   *
   * @var int $eid
   */
  protected $eid;

  /**
   * An array listing settings pages for the matching engine.
   *
   * @var array $settings
   *
   * Example structure:
   * @code
   * $settings = array(
   *  array(
   *   'name' => 'settings', // Machine readable settings page name.
   *   'path' => '<front>', // Internal path to settings page.
   *   'label' => t('Settings page'), // Translated label for link.
   *   ),
   * );
   * @endcode
   */
  protected $settings;

  /**
   * Constructor: sets basic variables.
   *
   * Don't forget to user t() for engine name and description fields.
   */
  public function __construct() {
    $this->name = '';
    $this->machine_name = '';
    $this->description = '';
    $this->settings = array();
  }

  /**
   * Returns engine human readable name.
   *
   * @return string
   */
  public function getName() {
    return $this->name;
  }

  /**
   * Returns engine machine readable name.
   *
   * @return string
   */
  public function getMachineName() {
    return $this->machineName;
  }

  /**
   * Returns engine description.
   *
   * @return string
   */
  public function getDescription() {
    return $this->description;
  }

  /**
   * Returns engine ID.
   *
   * @return int
   */
  public function getID() {
    return $this->eid;
  }

  /**
   * Returns engine weight.
   *
   * @return int
   */
  public function getWeight() {
    return $this->weight;
  }

  /**
   * Returns engine status.
   *
   * @return bool
   */
  public function getStatus() {
    return $this->status;
  }


  /**
   * Returns engine settings.
   *
   * @return array
   */
  public function getSettings() {
    return $this->settings;
  }

  /**
   * Set engine ID.
   *
   * @param int
   */
  public function setID($eid) {
    $this->eid = $eid;
  }

  /**
   * Set engine weigth.
   *
   * @param int
   */
  public function setWeight($weight) {
    $this->weight = $weight;
  }

  /**
   * Set engine status.
   *
   * @param bool
   */
  public function setStatus($status) {
    $this->status = $status;
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

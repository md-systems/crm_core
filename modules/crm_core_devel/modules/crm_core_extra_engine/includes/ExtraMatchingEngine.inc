<?php

/**
 * @file
 * Extra match engine definitions.
 */


/**
 * Engine placeholder used for debug purposes.
 */
class ExtraMatchingEngine extends CrmCoreMatchEngine {

  /**
   * Constructor: sets basic variables.
   */
  public function __construct() {
    $this->name = t('Extra Matching Engine');
    $this->machineName = 'extra_matching_engine';
    $this->description = t('Engine placeholder used for debug purposes.');
    $this->settings = array();
  }

  /**
   * Applies logical rules for identifying matches in the database.
   *
   * @see CrmCoreMatchEngineInterface::execute()
   */
  public function execute(&$contact, &$ids = array()) {
    drupal_set_message(t('Executed @name', array('@name' => get_class($this))));
  }
}

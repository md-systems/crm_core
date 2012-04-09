<?php

/**
 * CRM Activity Entity Class.
 */
class CRMCoreActivityEntity extends Entity {
  protected function defaultLabel() {
    return $this->title;
  }

  protected function defaultUri() {
    return array(
      'path' => 'crm/activity/' . $this->identifier(),
      'options' => array(
        'absolute' => TRUE,
      ),
    );
  }
}

/**
 * Controller class for CRM Activity.
 */
class CRMCoreActivityController extends EntityAPIController {
  public function create(array $values = array()) {
    global $user;
    $values += array(
      'title' => '',
      'created' => REQUEST_TIME,
      'changed' => REQUEST_TIME,
      'uid' => $user->uid,
    );

    return parent::create($values);
  }
}

/**
 * Use a separate class for profile types so we can specify some defaults
 * modules may alter.
 */
class CRMActivityType extends Entity {
  public $type;
  public $label;
  public $weight = 0;

  public function __construct($values = array()) {
    parent::__construct($values, 'crm_core_activity_type');
  }

  function isLocked() {
    return isset($this->status) && empty($this->is_new) && (($this->status & ENTITY_IN_CODE) || ($this->status & ENTITY_FIXED));
  }
}

/**
 * Controller class for CRM Activity Type.
 */
class CRMCoreActivityTypeController extends EntityAPIControllerExportable {
  public function create(array $values = array()) {
    $values += array(
      'type' => '',
      'label' => '',
      'description' => '',
      'is_new' => TRUE,
    );

    return parent::create($values);
  }
}

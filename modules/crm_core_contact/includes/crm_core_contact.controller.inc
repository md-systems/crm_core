<?php

/**
 * CRM Contact Entity Class.
 */
class CRMCoreContactEntity extends Entity {
  protected function defaultLabel() {
    return crm_core_contact_label($this);
  }

  protected function defaultUri() {
    return array(
      'path' => 'crm/contact/' . $this->identifier(),
      'options' => array(
        'absolute' => TRUE,
      ),
    );
  }
}

/**
 * @file
 * Controller class for contacts.
 *
 * This extends the DrupalDefaultEntityController class, adding required
 * special handling for contact objects.
 */
class CRMCoreContactController extends EntityAPIController {
  public $revisionKey = 'vid';
  public $revisionTable = 'crm_core_contact_revision';

  /**
   * Create a basic contact object.
   */
  public function create(array $values = array()) {
    global $user;
    $values += array(
      'contact_id' => '',
      'vid' => '',
      'uid' => $user->uid,
      'created' => REQUEST_TIME,
      'changed' => REQUEST_TIME,
    );

    return parent::create($values);
  }
  
  /**
   * Update contact object before saving revision.
   */
  protected function saveRevision($entity) {
    $entity->log = '';

    return parent::saveRevision($entity);
  }
}

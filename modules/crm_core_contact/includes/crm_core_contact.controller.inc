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
      'path' => 'crm-core/contact/' . $this->identifier(),
      'options' => array(
        'absolute' => TRUE,
      ),
    );
  }

  /**
   * Method for de-duplicating contacts.
   *
   * Allows various modules to identify duplicate contact records through
   * hook_crm_core_contact_match. This function should implement it's
   * own contact matching scheme.
   *
   * @param CRMCoreContactEntity $entity
   *   CRM Core Contact
   *
   * @return array
   *   Array of matched contacts.
   */
  public function match(CRMCoreContactEntity $entity) {

    $checks = & drupal_static(__FUNCTION__);
    $matches = array();

    if (!isset($checks->processed)) {
      $checks = new stdClass();
      $checks->engines = module_implements('crm_core_contact_match');
      $checks->processed = 1;
    }

    // Pass in the contact and the matches array as references.
    // This will allow various matching tools to modify the contact
    // and the list of matches.
    $values = array(
      'contact' => &$entity,
      'matches' => &$matches,
    );
    foreach ($checks->engines as $module) {
      module_invoke($module, 'crm_core_contact_match', $values);
    }

    // It's up to implementing modules to handle the matching logic.
    // Most often, the match to be used should be the one
    // at the top of the stack.
    return $matches;
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
    $entity->is_new_revision = TRUE;

    return parent::saveRevision($entity);
  }

  /**
   * Updates 'changed' property on save.
   */
  public function save($entity) {
    $entity->changed = REQUEST_TIME;

    return parent::save($entity);
  }
}

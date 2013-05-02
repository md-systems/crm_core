<?php

/**
 * CRM Contact Type Entity Class.
 */
class CRMContactType extends Entity {

  public $type;
  public $label;
  public $weight = 0;
  public $primary_fields;

  public function __construct($values = array()) {
    parent::__construct($values, 'crm_core_contact_type');
  }

  function isLocked() {
    return isset($this->status) && empty($this->is_new) && (($this->status & ENTITY_IN_CODE) || ($this->status & ENTITY_FIXED));
  }
}

/**
 * @file
 * Controller class for contacts.
 *
 * This extends the DrupalDefaultEntityController class, adding required
 * special handling for contact objects.
 */
class CRMCoreContactTypeController extends EntityAPIControllerExportable {

  public function create(array $values = array()) {
    $values += array(
      'name' => '',
      'description' => '',
    );

    return parent::create($values);
  }

  /**
   * Implements EntityAPIControllerInterface.
   *
   * @param @ids array of IDs
   * @param $transaction
   *   Optionally a DatabaseTransaction object to use. Allows overrides to pass
   *   in their transaction object.
   */
  public function delete($ids, DatabaseTransaction $transaction = NULL) {
    // Delete all instances of the given type.
    $query = new EntityFieldQuery;
    $results = $query
      ->entityCondition('entity_type', 'crm_core_contact')
      ->entityCondition('bundle', $ids, 'IN')
      ->execute();

    if (!empty($results)) {
      $contacts_to_remove = array_keys($results['crm_core_contact']);
      crm_core_contact_delete_multiple($contacts_to_remove);
      watchdog('crm_core_contact', 'Delete !count contacts due to deletion of contact type.', array('!count' => count($contacts_to_remove)), WATCHDOG_INFO);
    }

    // Finally delete the type itself.
    return parent::delete($ids, $transaction);
  }

  /**
   * Overrides a method of the EntityAPIControllerExportable class.
   */
  public function save($entity, DatabaseTransaction $transaction = NULL) {
    parent::save($entity, $transaction);

    // Reset cache entry, important for usecases like install procedure.
    drupal_static_reset('crm_core_contact_types');
  }
}

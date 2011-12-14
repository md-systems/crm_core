<?php

/**
 * @file
 * Controller class for contacts.
 *
 * This extends the DrupalDefaultEntityController class, adding required
 * special handling for contact objects.
 */
class CRMCoreContactController extends DrupalDefaultEntityController {
  public $revisionKey = 'vid';
  public $revisionTable = 'crm_core_contact_revision';
  
  /**
   * Create a basic contact object.
   */
  public function create($type) {
    return (object) array(
      'contact_id' => '',
      'vid' => '',
      'type' => $type,
      'uid' => '',
      'created' => '',
      'changed' => '',
    );
  }


  /**
   * Save a contact record.
   */
  public function save($contact) {
    $transaction = db_transaction();

    try {
      global $user;
      $uid = (isset($contact->uid)) ? $contact->uid : $user->uid;

      // Determine if this is a new contact and set changed time
      $contact->is_new = empty($contact->contact_id);
      $contact->changed = REQUEST_TIME;

      // Give modules a chance to alter things
      field_attach_presave('crm_core_contact', $contact);
      module_invoke_all('crm_core_contact_presave', $contact);

      // Initialize the log field if necessary
      if (!isset($contact->log)) {
        $contact->log = '';
      }

      // If we're adding a new contact
      if (empty($contact->contact_id)) {
        $contact->created = REQUEST_TIME;

        // Save the new contact
        drupal_write_record('crm_core_contact', $contact);
        
        // Save the first revision
        $this->saveRevision($contact, $uid);

        // Save the fields attched to the contact
        field_attach_insert('crm_core_contact', $contact);

        $op = 'insert';
      }
      else {
        // Always save a new revision so that we have a log of changes to the contact
        unset($contact->vid); // Unset the vid so that a new revision will be created
        $this->saveRevision($contact, $uid);

        // Update the record and its fields
        drupal_write_record('crm_core_contact', $contact, 'contact_id');
        field_attach_update('crm_core_contact', $contact);

        $op = 'update';
      }

      // Update the vid of the contact record
      db_update('crm_core_contact')->
        fields(array('vid' => $contact->vid))->
        condition('contact_id', $contact->contact_id)->
        execute();

      module_invoke_all('crm_core_contact_' . $op, $contact);
      module_invoke_all('entity_' . $op, $contact, 'crm_core_contact');

      // rules integration
      if (module_exists('rules')) {
        rules_invoke_event('crm_core_contact_'.$op, $contact);
      }
      
      unset($contact->is_new);
      $this->resetCache();

      return $contact;
    }
    catch (Exception $e) {
      $transaction->rollback('crm contact', $e->getMessage(), array(), WATCHDOG_ERROR);
      return FALSE;
    }
  }


  /**
   * Save a revision of a contact or update an existing revision.
   */
  public function saveRevision($contact, $uid, $update = FALSE) {
    $temp_uid = $contact->uid;
    $contact->uid = $uid;

    if ($update) {
      drupal_write_record('crm_core_revision', $contact, 'vid');
    }
    else {
      drupal_write_record('crm_core_contact_revision', $contact);
    }

    $contact->uid = $temp_uid;
  }
  
  
  /**
   * Reverts contact to specific revision.
   * 
   * @param $contact
   *   The fully loaded contact object of the contact to revert.
   * @param $vid
   *   The number revision ID to revert the contact to.
   */
  public function revertContact($contact, $vid) {
    // Load the old revision
    $contact_revision = $this->load(    
      array($contact->contact_id), 
      array('vid' => $vid)
    );
    
    // Reset and collect the specific revision, if it doesn't exist set to null
    $contact_revision = (is_array($contact_revision)) ? reset($contact_revision) : NULL; 
    
    // If it's a proper contact revision, we create a new revision with the old data
    // and log that information.
    if($contact_revision) {  
      unset($contact_revision->vid);        
      $contact_revision->log = 'Reverted revision ' . $contact->vid . ' to revision ' . $vid . '.'; 
      $this->save($contact_revision);  
      return TRUE;     
    }
    
    return FALSE;
  }
  
  
  /**
   * Delete multiple contacts by contact ID.
   */
  public function delete($contact_ids) {
    if (!empty($contact_ids)) {
      $contacts = $this->load($contact_ids, array());

      // @TODO Will need to look at this again when relationships have been implemented.

      $transaction = db_transaction();

      try {
        db_delete('crm_core_contact')->condition('contact_id', $contact_ids, 'IN')->execute();

        foreach ($contacts as $contact) {
          module_invoke_all('crm_core_contact_delete', $contact);
          field_attach_delete('crm_core_contact', $contact);
          // rules integration
          if (module_exists('rules')) {
            rules_invoke_event('crm_core_contact_delete', $contact);
          }
        }
      }
      catch (Exception $e) {
        $transaction->rollback();
        watchdog_exception('crm_core_contact', $e);
        throw $e;
      }

      cache_clear_all();
      $this->resetCache();
    }

    return TRUE;
  }
}

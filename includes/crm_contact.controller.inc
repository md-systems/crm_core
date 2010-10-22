<?php

/**
 * @file
 * Controller class for contacts.
 *
 * This extends the DrupalDefaultEntityController class, adding required
 * special handling for contact objects.
 */
class CRMContactController extends DrupalDefaultEntityController {
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

      // Determine if this is a new contact and set changed time
      $contact->is_new = empty($contact->contact_id);
      $contact->changed = REQUEST_TIME;

      // Give modules a chance to alter things
      field_attach_presave('crm_contact', $contact);
      module_invoke_all('crm_contact_presave', $contact);

      // Initialize the log field if necessary
      if (!isset($contact->log)) {
        $contact->log = '';
      }

      // If we're adding a new contact
      if (empty($contact->contact_id)) {
        $contact->created = REQUEST_TIME;

        // Save the new contact
        drupal_write_record('crm_contact', $contact);
        
        // Save the first revision
        $this->saveRevision($contact, $user->uid);

        // Save the fields attched to the contact
        field_attach_insert('crm_contact', $contact);

        $op = 'insert';
      }
      else {
        // Always save a new revision so that we have a log of changes to the contact
        unset($contact->vid); // Unset the vid so that a new revision will be created
        $this->saveRevision($contact, $user->uid);

        // Update the record and its fields
        drupal_write_record('crm_contact', $contact, 'contact_id');
        field_attach_update('crm_contact', $contact);

        $op = 'update';
      }

      // Update the vid of the contact record
      db_update('crm_contact')->
        fields(array('vid' => $contact->vid))->
        condition('contact_id', $contact->contact_id)->
        execute();

      module_invoke_all('crm_contact_' . $op, $contact);
      module_invoke_all('entity_' . $op, $contact, 'crm_contact');

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
      drupal_write_record('crm_contact_revision', $contact, 'vid');
    }
    else {
      drupal_write_record('crm_contact_revision', $contact);
    }

    $contact->uid = $temp_uid;
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
        db_delete('crm_contact')->condition('contact_id', $contact_ids, 'IN')->execute();

        foreach ($contacts as $contact) {
          module_invoke_all('crm_contact_delete', $contact);
          field_attach_delete('crm_contact', $contact);
        }
      }
      catch (Exception $e) {
        $transaction->rollback();
        watchdog_exception('crm_contact', $e);
        throw $e;
      }

      cache_clear_all();
      $this->resetCache();
    }

    return TRUE;
  }
}

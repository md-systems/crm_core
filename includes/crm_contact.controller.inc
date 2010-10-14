<?php

/**
 * @file
 * Controller class for contacts.
 *
 * This extends the DrupalDefaultEntityController class, adding required
 * special handling for contact objects.
 */
class CRMContactController extends DrupalDefaultEntityController {

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

  public function save($contact) {
    $transaction = db_transaction();

    try {
      $contact->changed = REQUEST_TIME;

      // Give modules a chance to alter things
      field_attach_presave('crm_contact', $contact);
      module_invoke_all('crm_contact_presave', $contact);

      // If we're adding a new contact
      if (empty($contact->contact_id)) {
        $contact->created = REQUEST_TIME;

        // Save the new contact and fields
        drupal_write_record('crm_contact', $contact);
        field_attach_insert('crm_contact', $contact);

        $op = 'insert';
      }
      else {
        // Update the record and its fields
        drupal_write_record('crm_contact', $contact);
        field_attach_update('crm_contact', $contact);

        $op = 'update';
      }

      module_invoke_all('crm_contact_' . $op, $contact);
      module_invoke_all('entity_' . $op, $contact, 'crm_contact');

      return $contact;
    }
    catch (Exception $e) {
      $transaction->rollback('crm contact', $e->getMessage(), array(), WATCHDOG_ERROR);
      return FALSE;
    }
  }

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

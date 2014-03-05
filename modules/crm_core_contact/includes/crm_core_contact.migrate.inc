<?php

class MigrateDestinationCRMCoreContact extends MigrateDestinationEntity {
  static public function getKeySchema() {
    return array(
      'contact_id' => array(
        'type' => 'int',
        'unsigned' => TRUE,
        'description' => 'The primary identifer for a contact.',
      ),
    );
  }

  /**
   * Return an options array for contact destinations.
   *
   * @param string $language
   *  Default language for contacts created via this destination class.
   * @param string $text_format
   *  Default text format for contacts created via this destination class.
   */
  static public function options($language, $text_format) {
    return compact('language', 'text_format');
  }

  public function __construct($bundle, array $options = array()) {
    parent::__construct('crm_core_contact', $bundle, $options);
  }

  /**
   * Returns a list of fields available to be mapped for the contact type (bundle)
   *
   * @param Migration $migration
   *  Optionally, the migration containing this destination.
   * @return array
   *  Keys: machine names of the fields (to be passed to addFieldMapping)
   *  Values: Human-friendly descriptions of the fields.
   */
  public function fields($migration = NULL) {
    $fields = array();
    // First the core (contact table) properties
    $fields['contact_id'] = t('CRM Core Contact: Existing Contact ID');
    $fields['created'] = t('Created timestamp');
    $fields['changed'] = t('Modified timestamp');
    $fields['uid'] = t('Authored by (uid)');
    $fields += migrate_handler_invoke_all('Entity', 'fields', $this->entityType, $this->bundle, $migration);
    return $fields;
  }

  /**
   * Delete a batch of contacts at once.
   *
   * @param $contact_ids
   *  Array of contact IDs to be deleted.
   */
  public function bulkRollback(array $contact_ids) {
    migrate_instrument_start('crm_core_contact_delete_multiple');
    $this->prepareRollback($contact_ids);
    crm_core_contact_delete_multiple($contact_ids);
    $this->completeRollback($contact_ids);
    migrate_instrument_stop('crm_core_contact_delete_multiple');
  }

  /**
   * Import a single contact.
   *
   * @param $contact
   *  Contact object to build. Prefilled with any fields mapped in the Migration.
   * @param $row
   *  Raw source data object - passed through to prepare/complete handlers.
   * @return array
   *  Array of key fields (contact_id only in this case) of the contact that was saved if
   *  successful. FALSE on failure.
   */
  public function import(stdClass $contact, stdClass $row) {
    $migration = Migration::currentMigration();

    if (isset($row->migrate_map_destid1)) {
      // This is an update to an existing contact.
      if (isset($contact->contact_id)) {
        if ($contact->contact_id != $row->migrate_map_destid1) {
          $string = "Incoming contact_id !contact_id and map destination contact_id !destid1 don't match";
          $params = array(
            '!contact_id' => $contact->contact_id,
            '!destid1' => $row->migrate_map_destid1,
          );
          throw new MigrateException(t($string), $params);
        }
      }
      else {
        $contact->contact_id = $row->migrate_map_destid1;
      }
    }

    if ($migration->getSystemOfRecord() == Migration::DESTINATION) {
      if (!isset($contact->contact_id)) {
        throw new MigrateException(t('System-of-record is DESTINATION, but no destination contact_id provided'));
      }
      $old_contact = crm_core_contact_load($contact->contact_id);
      if (empty($old_contact)) {
        $string = 'System-of-record is DESTINATION, but contact !contact_id does not exist';
        $params = array('!contact_id' => $contact->contact_id);
        throw new MigrateException(t($string), $params);
      }
    }

    if (!isset($contact->type)) {
      // Default the type to our designated destination bundle (by doing this
      // conditionally, we permit some flexibility in terms of implementing
      // migrations which can affect more than one type).
      $contact->type = $this->bundle;
    }

    // Set default properties.
    if ($migration->getSystemOfRecord() == Migration::SOURCE) {
      if (!isset($contact->created)) {
        $contact->created = REQUEST_TIME;
      }
    }

    $this->prepare($contact, $row);
    if (isset($contact->contact_id)) {
      $updating = TRUE;
    }
    else {
      $updating = FALSE;
    }

    migrate_instrument_start('contact_save');
    crm_core_contact_save($contact);
    migrate_instrument_stop('contact_save');

    if (isset($contact->contact_id)) {
      if ($updating) {
        $this->numUpdated++;
      }
      else {
        $this->numCreated++;
      }
      $return = array($contact->contact_id);
    }
    else {
      $return = FALSE;
    }

    $this->complete($contact, $row);
    return $return;
  }
}

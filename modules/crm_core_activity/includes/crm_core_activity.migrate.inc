<?php

class MigrateDestinationCRMCoreActivity extends MigrateDestinationEntity {
  static public function getKeySchema() {
    return array(
      'activity_id' => array(
        'type' => 'int',
        'unsigned' => TRUE,
        'description' => 'The primary identifer for an activity.',
      ),
    );
  }

  /**
   * Return an options array for activity destinations.
   *
   * @param string $language
   *  Default language for activities created via this destination class.
   * @param string $text_format
   *  Default text format for activities created via this destination class.
   */
  static public function options($language, $text_format) {
    return compact('language', 'text_format');
  }

  public function __construct($bundle, array $options = array()) {
    parent::__construct('crm_core_activity', $bundle, $options);
  }

  /**
   * Returns a list of fields available to be mapped for the activity type (bundle)
   *
   * @param Migration $migration
   *  Optionally, the migration containing this destination.
   * @return array
   *  Keys: machine names of the fields (to be passed to addFieldMapping)
   *  Values: Human-friendly descriptions of the fields.
   */
  public function fields($migration = NULL) {
    $fields = array();
    // First the core (activity table) properties
    $fields['activity_id'] = t('CRM Core Activity: Existing Activity ID');
    $fields['created'] = t('Created timestamp');
    $fields['changed'] = t('Modified timestamp');
    $fields['uid'] = t('Authored by (uid)');
    $fields += migrate_handler_invoke_all('Entity', 'fields', $this->entityType, $this->bundle, $migration);
    return $fields;
  }

  /**
   * Delete a batch of activities at once.
   *
   * @param $activity_ids
   *  Array of activity IDs to be deleted.
   */
  public function bulkRollback(array $activity_ids) {
    migrate_instrument_start('crm_core_activity_delete_multiple');
    $this->prepareRollback($activity_ids);
    crm_core_activity_delete_multiple($activity_ids);
    $this->completeRollback($activity_ids);
    migrate_instrument_stop('crm_core_activity_delete_multiple');
  }

  /**
   * Import a single activity.
   *
   * @param $activity
   *  Activity object to build. Prefilled with any fields mapped in the Migration.
   * @param $row
   *  Raw source data object - passed through to prepare/complete handlers.
   * @return array
   *  Array of key fields (activity_id only in this case) of the activity that was saved if
   *  successful. FALSE on failure.
   */
  public function import(stdClass $activity, stdClass $row) {
    $migration = Migration::currentMigration();

    if (isset($row->migrate_map_destid1)) {
      // This is an update to an existing contact.
      if (isset($activity->activity_id)) {
        if ($activity->activity_id != $row->migrate_map_destid1) {
          $string = "Incoming activity_id !activity_id and map destination activity_id !destid1 don't match";
          $params = array(
            '!activity_id' => $activity->activity_id,
            '!destid1' => $row->migrate_map_destid1,
          );
          throw new MigrateException(t($string), $params);
        }
      }
      else {
        $activity->activity_id = $row->migrate_map_destid1;
      }
    }

    if ($migration->getSystemOfRecord() == Migration::DESTINATION) {
      if (!isset($activity->activity_id)) {
        throw new MigrateException(t('System-of-record is DESTINATION, but no destination activity_id provided'));
      }
      $old_activity = crm_core_activity_load($activity->activity_id);
      if (empty($old_activity)) {
        $string = 'System-of-record is DESTINATION, but activity !activity_id does not exist';
        $params = array('!activity_id' => $activity->activity_id);
        throw new MigrateException(t($string), $params);
      }
    }

    if (!isset($activity->type)) {
      // Default the type to our designated destination bundle (by doing this
      // conditionally, we permit some flexibility in terms of implementing
      // migrations which can affect more than one type).
      $activity->type = $this->bundle;
    }

    // Set default properties.
    if ($migration->getSystemOfRecord() == Migration::SOURCE) {
      if (!isset($activity->created)) {
        $activity->created = REQUEST_TIME;
      }
    }

    $this->prepare($activity, $row);
    if (isset($activity->activity_id)) {
      $updating = TRUE;
    }
    else {
      $updating = FALSE;
    }

    migrate_instrument_start('activity_save');
    crm_core_activity_save($activity);
    migrate_instrument_stop('activity_save');

    if (isset($activity->activity_id)) {
      if ($updating) {
        $this->numUpdated++;
      }
      else {
        $this->numCreated++;
      }
      $return = array($activity->activity_id);
    }
    else {
      $return = FALSE;
    }

    $this->complete($activity, $row);
    return $return;
  }
}

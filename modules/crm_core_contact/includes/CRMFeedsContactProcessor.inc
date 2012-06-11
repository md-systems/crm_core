<?php

/**
 * @file
 * Class definition of CRMFeedsContactProcessor.
 */

/**
 * Creates contacts from feed items.
 */
class CRMFeedsContactProcessor extends FeedsProcessor {
  /**
   * Define entity type.
   */
  public function entityType() {
    return 'crm_core_contact';
  }

  /**
   * Implements parent::entityInfo().
   */
  protected function entityInfo() {
    $info = parent::entityInfo();
    $info['label plural'] = t('Contacts');
    return $info;
  }

  /**
   * Creates a new contact in memory and returns it.
   */
  protected function newEntity(FeedsSource $source) {
    $contact = new stdClass();
    $contact->type = $this->config['contact_type'];
    $contact->changed = REQUEST_TIME;
    $contact->created = REQUEST_TIME;
    $contact->language = LANGUAGE_NONE;
    $contact->log = t('Imported by feeds.');
    $contact->uid = $this->config['author'];
    return $contact;
  }

  /**
   * Loads an existing contact.
   *
   * If the update existing method is not FEEDS_UPDATE_EXISTING, only the contact
   * table will be loaded, foregoing the crm_core_contact_load API for better performance.
   */
  protected function entityLoad(FeedsSource $source, $contact_id) {
    if ($this->config['update_existing'] == FEEDS_UPDATE_EXISTING) {
      $contact = crm_core_contact_load($contact_id, NULL, TRUE);
    }
    else {
      // We're replacing the existing contact. Only save the absolutely necessary.
      $contact = db_query("SELECT created, contact_id, vid, type FROM {crm_core_contact} WHERE contact_id = :contact_id", array(':contact_id' => $contact_id))->fetchObject();
      $contact->uid = $this->config['author'];
    }
    // Populate properties that are set by contact_object_prepare().
    if ($this->config['update_existing'] == FEEDS_UPDATE_EXISTING) {
      $contact->log = 'Updated by CRMFeedsContactProcessor';
    }
    else {
      $contact->log = 'Replaced by CRMFeedsContactProcessor';
    }
    return $contact;
  }

  /**
   * Save a contact.
   */
  public function entitySave($entity) {
    crm_core_contact_save($entity);
  }

  /**
   * Delete a series of contacts.
   */
  protected function entityDeleteMultiple($contact_ids) {
    crm_core_contact_delete_multiple($contact_ids);
  }

  /**
   * Implement expire().
   *
   * @todo: move to processor stage?
   */
  public function expire($time = NULL) {
    if ($time === NULL) {
      $time = $this->expiryTime();
    }
    if ($time == FEEDS_EXPIRE_NEVER) {
      return;
    }
    $count = $this->getLimit();
    $contacts = db_query_range("SELECT c.contact_id FROM {crm_core_contact} c JOIN {feeds_item} fi ON fi.entity_type = 'crm_core_contact' AND c.contact_id = fi.entity_id WHERE fi.id = :id AND c.created < :created", 0, $count, array(':id' => $this->id, ':created' => REQUEST_TIME - $time));
    $contact_ids = array();
    foreach ($contacts as $contact) {
      $contact_ids[$contact->contact_id] = $contact->contact_id;
    }
    $this->entityDeleteMultiple($contact_ids);
    if (db_query_range("SELECT 1 FROM {crm_core_contact} c JOIN {feeds_item} fi ON fi.entity_type = 'crm_core_contact' AND c.contact_id = fi.entity_id WHERE fi.id = :id AND c.created < :created", 0, 1, array(':id' => $this->id, ':created' => REQUEST_TIME - $time))->fetchField()) {
      return FEEDS_BATCH_ACTIVE;
    }
    return FEEDS_BATCH_COMPLETE;
  }

  /**
   * Return expiry time.
   */
  public function expiryTime() {
    return $this->config['expire'];
  }

  /**
   * Override parent::configDefaults().
   */
  public function configDefaults() {
    $types = crm_core_contact_type_get_name();
    $type = key($types);
    return array(
      'contact_type' => $type,
      'expire' => FEEDS_EXPIRE_NEVER,
      'author' => 0,
    ) + parent::configDefaults();
  }

  /**
   * Override parent::configForm().
   */
  public function configForm(&$form_state) {
    $types = crm_core_contact_type_get_name();
    array_walk($types, 'check_plain');
    $form = parent::configForm($form_state);
    $form['contact_type'] = array(
      '#type' => 'select',
      '#title' => t('Contact type'),
      '#description' => t('Select the type of contacts to be created.'),
      '#options' => $types,
      '#default_value' => $this->config['contact_type'],
    );
    $author = user_load($this->config['author']);
    $form['author'] = array(
      '#type' => 'textfield',
      '#title' => t('Author'),
      '#description' => t('Select the author of the contacts to be created - leave empty to assign "anonymous".'),
      '#autocomplete_path' => 'user/autocomplete',
      '#default_value' => empty($author->name) ?  'anonymous' : check_plain($author->name),
    );
    $period = drupal_map_assoc(array(FEEDS_EXPIRE_NEVER, 3600, 10800, 21600, 43200, 86400, 259200, 604800, 2592000, 2592000 * 3, 2592000 * 6, 31536000), 'feeds_format_expire');
    $form['expire'] = array(
      '#type' => 'select',
      '#title' => t('Expire contacts'),
      '#options' => $period,
      '#description' => t('Select after how much time contacts should be deleted. The contact\'s published date will be used for determining the contact\'s age, see Mapping settings.'),
      '#default_value' => $this->config['expire'],
    );
    $form['update_existing']['#options'] = array(
      FEEDS_SKIP_EXISTING => 'Do not update existing contacts',
      FEEDS_REPLACE_EXISTING => 'Replace existing contacts',
      FEEDS_UPDATE_EXISTING => 'Update existing contacts (slower than replacing them)',
    );
    return $form;
  }

  /**
   * Override parent::configFormValidate().
   */
  public function configFormValidate(&$values) {
    if ($author = user_load_by_name($values['author'])) {
      $values['author'] = $author->uid;
    }
    else {
      $values['author'] = 0;
    }
  }

  /**
   * Reschedule if expiry time changes.
   */
  public function configFormSubmit(&$values) {
    if ($this->config['expire'] != $values['expire']) {
      feeds_reschedule($this->id);
    }
    parent::configFormSubmit($values);
  }

  /**
   * Override setTargetElement to operate on a target item that is a contact.
   */
  public function setTargetElement(FeedsSource $source, $target_contact, $target_element, $value) {
    switch ($target_element) {
      case 'created':
        $target_contact->created = feeds_to_unixtime($value, REQUEST_TIME);
        break;
      case 'feeds_source':
        // Get the class of the feed contact importer's fetcher and set the source
        // property. See feeds_contact_update() how $contact->feeds gets stored.
        if ($id = feeds_get_importer_id($this->config['contact_type'])) {
          $class = get_class(feeds_importer($id)->fetcher);
          $target_contact->feeds[$class]['source'] = $value;
          // This effectively suppresses 'import on submission' feature.
          // See feeds_contact_insert().
          $target_contact->feeds['suppress_import'] = TRUE;
        }
        break;
      default:
        parent::setTargetElement($source, $target_contact, $target_element, $value);
        break;
    }
  }

  /**
   * Return available mapping targets.
   */
  public function getMappingTargets() {
    $types = crm_core_contact_types();
    $type = $types[$this->config['contact_type']];
    $targets = parent::getMappingTargets();

    $targets += array(
      'contact_id' => array(
        'name' => t('Contact ID'),
        'description' => t('The contact_id of the contact. NOTE: use this feature with care, contact ids are usually assigned by Drupal.'),
        'optional_unique' => TRUE,
      ),
      'uid' => array(
        'name' => t('User ID'),
        'description' => t('The Drupal user ID of the contact author.'),
      ),
      'created' => array(
        'name' => t('Created date'),
        'description' => t('The UNIX time when a contact has been created.'),
      ),
    );

    // If the target content type is a Feed contact, expose its source field.
    if ($id = feeds_get_importer_id($this->config['contact_type'])) {
      $name = feeds_importer($id)->config['name'];
      $targets['feeds_source'] = array(
        'name' => t('Feed source'),
        'description' => t('The contact type created by this processor is a Feed Node, it represents a source itself. Depending on the fetcher selected on the importer "@importer", this field is expected to be for example a URL or a path to a file.', array('@importer' => $name)),
        'optional_unique' => TRUE,
      );
    }

    // Let other modules expose mapping targets.
    self::loadMappers();
    feeds_alter('feeds_processor_targets', $targets, 'crm_core_contact', $this->config['contact_type']);

    return $targets;
  }

  /**
   * Get contact_id of an existing feed item contact if available.
   */
  protected function existingEntityId(FeedsSource $source, FeedsParserResult $result) {
    if ($contact_id = parent::existingEntityId($source, $result)) {
      return $contact_id;
    }

    // Iterate through all unique targets and test whether they do already
    // exist in the database.
    foreach ($this->uniqueTargets($source, $result) as $target => $value) {
      switch ($target) {
        case 'contact_id':
          $contact_id = db_query("SELECT contact_id FROM {crm_core_contact} WHERE contact_id = :contact_id", array(':contact_id' => $value))->fetchField();
          break;
        case 'feeds_source':
          if ($id = feeds_get_importer_id($this->config['contact_type'])) {
            $contact_id = db_query("SELECT fs.feed_contact_id FROM {crm_core_contact} c JOIN {feeds_source} fs ON c.contact_id = fs.feed_contact_id WHERE fs.id = :id AND fs.source = :source", array(':id' => $id, ':source' => $value))->fetchField();
          }
          break;
      }
      if ($contact_id) {
        // Return with the first contact_id found.
        return $contact_id;
      }
    }
    return 0;
  }
}

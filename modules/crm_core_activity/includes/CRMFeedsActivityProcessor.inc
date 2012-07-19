<?php

/**
 * @file
 * Class definition of CRMFeedsActivityProcessor.
 */

/**
 * Creates activities from feed items.
 */
class CRMFeedsActivityProcessor extends FeedsProcessor {
  /**
   * Define entity type.
   */
  public function entityType() {
    return 'crm_core_activity';
  }

  /**
   * Implements parent::entityInfo().
   */
  protected function entityInfo() {
    $info = parent::entityInfo();
    $info['label plural'] = t('Activities');
    return $info;
  }

  /**
   * Creates a new activity in memory and returns it.
   */
  protected function newEntity(FeedsSource $source) {
    $activity = new stdClass();
    $activity->type = $this->config['activity_type'];
    $activity->changed = REQUEST_TIME;
    $activity->created = REQUEST_TIME;
    $activity->language = LANGUAGE_NONE;
    $activity->log = t('Imported by feeds.');
    $activity->uid = $this->config['author'];
    return $activity;
  }

  /**
   * Loads an existing activity.
   *
   * If the update existing method is not FEEDS_UPDATE_EXISTING, only the activity
   * table will be loaded, foregoing the crm_core_activity_load API for better performance.
   */
  protected function entityLoad(FeedsSource $source, $activity_id) {
    if ($this->config['update_existing'] == FEEDS_UPDATE_EXISTING) {
      $activity = crm_core_activity_load($activity_id, TRUE);
    }
    else {
      // We're replacing the existing activity. Only save the absolutely necessary.
      $activity = db_query("SELECT created, activity_id, type FROM {crm_core_activity} WHERE activity_id = :activity_id", array(':activity_id' => $activity_id))->fetchObject();
      $activity->uid = $this->config['author'];
    }
    // Populate properties that are set by activity_object_prepare().
    if ($this->config['update_existing'] == FEEDS_UPDATE_EXISTING) {
      $activity->log = 'Updated by CRMFeedsActivityProcessor';
    }
    else {
      $activity->log = 'Replaced by CRMFeedsActivityProcessor';
    }
    return $activity;
  }

  /**
   * Save a activity.
   */
  public function entitySave($entity) {
    crm_core_activity_save($entity);
  }

  /**
   * Delete a series of activitys.
   */
  protected function entityDeleteMultiple($activity_ids) {
    crm_core_activity_delete_multiple($activity_ids);
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
    $activitys = db_query_range("SELECT a.activity_id FROM {crm_core_activity} a JOIN {feeds_item} fi ON fi.entity_type = 'crm_core_activity' AND a.activity_id = fi.entity_id WHERE fi.id = :id AND c.created < :created", 0, $count, array(':id' => $this->id, ':created' => REQUEST_TIME - $time));
    $activity_ids = array();
    foreach ($activitys as $activity) {
      $activity_ids[$activity->activity_id] = $activity->activity_id;
    }
    $this->entityDeleteMultiple($activity_ids);
    if (db_query_range("SELECT 1 FROM {crm_core_activity} a JOIN {feeds_item} fi ON fi.entity_type = 'crm_core_activity' AND a.activity_id = fi.entity_id WHERE fi.id = :id AND a.created < :created", 0, 1, array(':id' => $this->id, ':created' => REQUEST_TIME - $time))->fetchField()) {
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
    $types = crm_core_activity_type_get_name();
    $type = key($types);
    return array(
      'activity_type' => $type,
      'expire' => FEEDS_EXPIRE_NEVER,
      'author' => 0,
    ) + parent::configDefaults();
  }

  /**
   * Override parent::configForm().
   */
  public function configForm(&$form_state) {
    $types = crm_core_activity_type_get_name();
    array_walk($types, 'check_plain');
    $form = parent::configForm($form_state);
    $form['activity_type'] = array(
      '#type' => 'select',
      '#title' => t('Activity type'),
      '#description' => t('Select the type of activities to be created.'),
      '#options' => $types,
      '#default_value' => $this->config['activity_type'],
    );
    $author = user_load($this->config['author']);
    $form['author'] = array(
      '#type' => 'textfield',
      '#title' => t('Author'),
      '#description' => t('Select the author of the activities to be created - leave empty to assign "anonymous".'),
      '#autocomplete_path' => 'user/autocomplete',
      '#default_value' => empty($author->name) ?  'anonymous' : check_plain($author->name),
    );
    $period = drupal_map_assoc(array(FEEDS_EXPIRE_NEVER, 3600, 10800, 21600, 43200, 86400, 259200, 604800, 2592000, 2592000 * 3, 2592000 * 6, 31536000), 'feeds_format_expire');
    $form['expire'] = array(
      '#type' => 'select',
      '#title' => t('Expire activities'),
      '#options' => $period,
      '#description' => t('Select after how much time activities should be deleted. The activity\'s published date will be used for determining the activity\'s age, see Mapping settings.'),
      '#default_value' => $this->config['expire'],
    );
    $form['update_existing']['#options'] = array(
      FEEDS_SKIP_EXISTING => 'Do not update existing activities',
      FEEDS_REPLACE_EXISTING => 'Replace existing activities',
      FEEDS_UPDATE_EXISTING => 'Update existing activities (slower than replacing them)',
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
   * Override setTargetElement to operate on a target item that is a activity.
   */
  public function setTargetElement(FeedsSource $source, $target_activity, $target_element, $value) {
    switch ($target_element) {
      case 'created':
        $target_activity->created = feeds_to_unixtime($value, REQUEST_TIME);
        break;
      case 'feeds_source':
        // Get the class of the feed activity importer's fetcher and set the source
        // property. See feeds_activity_update() how $activity->feeds gets stored.
        if ($id = feeds_get_importer_id($this->config['activity_type'])) {
          $class = get_class(feeds_importer($id)->fetcher);
          $target_activity->feeds[$class]['source'] = $value;
          // This effectively suppresses 'import on submission' feature.
          // See feeds_activity_insert().
          $target_activity->feeds['suppress_import'] = TRUE;
        }
        break;
      default:
        parent::setTargetElement($source, $target_activity, $target_element, $value);
        break;
    }
  }

  /**
   * Return available mapping targets.
   */
  public function getMappingTargets() {
    $types = crm_core_activity_types();
    $type = $types[$this->config['activity_type']];
    $targets = parent::getMappingTargets();

    $targets += array(
      'title' => array(
        'name' => t('Title'),
        'description' => t('The title of the CRM Activity.'),
        'optional_unique' => TRUE,
      ),
      'activity_id' => array(
        'name' => t('Activity ID'),
        'description' => t('The activity_id of the activity. NOTE: use this feature with care, activity ids are usually assigned by Drupal.'),
        'optional_unique' => TRUE,
      ),
      'uid' => array(
        'name' => t('User ID'),
        'description' => t('The Drupal user ID of the activity author.'),
      ),
      'created' => array(
        'name' => t('Created date'),
        'description' => t('The UNIX time when a activity has been created.'),
      ),
    );

    // If the target content type is a Feed activity, expose its source field.
    if ($id = feeds_get_importer_id($this->config['activity_type'])) {
      $name = feeds_importer($id)->config['name'];
      $targets['feeds_source'] = array(
        'name' => t('Feed source'),
        'description' => t('The activity type created by this processor is a Feed Node, it represents a source itself. Depending on the fetcher selected on the importer "@importer", this field is expected to be for example a URL or a path to a file.', array('@importer' => $name)),
        'optional_unique' => TRUE,
      );
    }

    // Let other modules expose mapping targets.
    self::loadMappers();
    feeds_alter('feeds_processor_targets', $targets, 'crm_core_activity', $this->config['activity_type']);

    return $targets;
  }

  /**
   * Get activity_id of an existing feed item activity if available.
   */
  protected function existingEntityId(FeedsSource $source, FeedsParserResult $result) {
    if ($activity_id = parent::existingEntityId($source, $result)) {
      return $activity_id;
    }

    // Iterate through all unique targets and test whether they do already
    // exist in the database.
    foreach ($this->uniqueTargets($source, $result) as $target => $value) {
      switch ($target) {
        case 'activity_id':
          $activity_id = db_query("SELECT activity_id FROM {crm_core_activity} WHERE activity_id = :activity_id", array(':activity_id' => $value))->fetchField();
          break;
        case 'feeds_source':
          if ($id = feeds_get_importer_id($this->config['activity_type'])) {
            $activity_id = db_query("SELECT fs.feed_activity_id FROM {crm_core_activity} c JOIN {feeds_source} fs ON c.activity_id = fs.feed_activity_id WHERE fs.id = :id AND fs.source = :source", array(':id' => $id, ':source' => $value))->fetchField();
          }
          break;
      }
      if ($activity_id) {
        // Return with the first activity_id found.
        return $activity_id;
      }
    }
    return 0;
  }
}

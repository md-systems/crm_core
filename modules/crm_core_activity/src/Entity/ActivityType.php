<?php
/**
 * @file
 * Contains \Drupal\crm_core_activity\Entity\ActivityType.
 */

namespace Drupal\crm_core_activity\Entity;

use Drupal\Core\Config\Entity\ConfigEntityBase;
use Drupal\Core\Entity\EntityInterface;
use Drupal\Core\Entity\EntityStorageInterface;
use Drupal\field\Entity\FieldConfig;
use Drupal\field\Entity\FieldInstanceConfig;

/**
 * CRM Activity Type Entity Class.
 *
 * @ConfigEntityType(
 *   id = "crm_core_activity_type",
 *   label = @Translation("CRM Core Activity type"),
 *   bundle_of = "crm_core_activity",
 *   config_prefix = "type",
 *   controllers = {
 *     "access" = "Drupal\crm_core_activity\ActivityTypeAccessController",
 *     "form" = {
 *       "default" = "Drupal\crm_core_activity\Form\ActivityTypeForm",
 *       "delete" = "Drupal\crm_core_activity\Form\ActivityTypeDeleteForm",
 *       "toggle" = "Drupal\crm_core_activity\Form\ActivityTypeToggleForm",
 *     },
 *     "list_builder" = "Drupal\crm_core_activity\ActivityTypeListBuilder",
 *   },
 *   entity_keys = {
 *     "id" = "type",
 *     "label" = "name",
 *   },
 *   links = {
 *     "canonical" = "crm_core_activity.type_edit",
 *     "add-form" = "crm_core_activity.type_add",
 *     "edit-form" = "crm_core_activity.type_edit",
 *     "delete-form" = "crm_core_activity.type_delete",
 *     "enable" = "crm_core_activity.type_enable",
 *     "disable" = "crm_core_activity.type_disable",
 *   }
 * )
 */
class ActivityType extends ConfigEntityBase {

  /**
   * The machine-readable name of this type.
   *
   * @var string
   */
  public $type = '';

  /**
   * The human-readable name of this type.
   *
   * @var string
   */
  public $name = '';

  /**
   * A brief description of this type.
   *
   * @var string
   */
  public $description = '';

  /**
   * Text describing the relationship between the contact and this activity.
   *
   * @var string
   */
  public $activity_string;

  /**
   * Overrides Entity::__construct().
   */
  public function __construct($values = array()) {
    parent::__construct($values, 'crm_core_activity_type');
  }

  /**
   * Overrides Entity::id().
   */
  public function id() {
    return $this->type;
  }

  /**
   * {@inheritdoc}
   *
   * @todo This does not scale.
   *
   * Deleting a activity type with thousands of activities records associated
   * will run into execution timeout.
   */
  public static function preDelete(EntityStorageInterface $storage, array $entities) {
    parent::preDelete($storage, $entities);

    $ids = array_map(function(EntityInterface $entity){
      return $entity->id();
    }, $entities);

    // Delete all instances of the given type.
    $results = \Drupal::entityQuery('crm_core_activity')
      ->condition('type', $ids, 'IN')
      ->execute();

    if (!empty($results)) {
      $activities = Activity::loadMultiple($results);
      \Drupal::entityManager()->getStorage('crm_core_activity')->delete($activities);
      // @todo Handle singular and plural.
      watchdog('crm_core_activity', 'Delete !count activities due to deletion of activity type.', array('!count' => count($results)), WATCHDOG_INFO);
    }
  }

  /**
   * {@inheritdoc}
   */
  public function postSave(EntityStorageInterface $storage, $update = TRUE) {
    parent::postSave($storage, $update);

    if (!$update) {
      $this->ensureParticipantField();
      $this->ensureDateField();
      $this->ensureNotesField();
    }
  }

  /**
   * Adds a participant field for an activity type.
   *
   * @todo Check field and instance settings.
   */
  protected function ensureParticipantField() {
    $field_name = 'activity_participants';

    $field = FieldConfig::loadByName('crm_core_activity', $field_name);
    $instance = FieldInstanceConfig::loadByName('crm_core_activity', $this->id(), $field_name);

    if (empty($field)) {
      entity_create('field_config', array(
        'name' => $field_name,
        'type' => 'entity_reference',
        'entity_type' => 'crm_core_activity',
        'cardinality' => -1,
        'settings' => array(
          'target_type' => 'crm_core_contact',
          'handler' => 'base',
          'handler_submit' => 'Change handler',
          'handler_settings' => array('target_bundles' => array()),
        ),
      ))->save();
    }

    if (empty($instance)) {
      entity_create('field_instance_config', array(
        'field_name' => $field_name,
        'entity_type' => 'crm_core_activity',
        'bundle' => $this->id(),
        'label' => t('Participants'),
        'settings' => array(
          'required' => TRUE,
          'user_register_form' => FALSE,
        ),
      ))->save();

      // Assign widget settings for the 'default' form mode.
      entity_get_form_display('crm_core_activity', $this->id(), 'default')
        ->setComponent('activity_participants', array(
          'type' => 'entityreference_autocomplete_tags',
          'module' => 'entityreference',
          'active' => 1,
          'settings' => array(
            'match_operator' => 'CONTAINS',
            'size' => 60,
            'path' => '',
          ),
        ))
        ->save();

      // Assign display settings for the 'default' and 'teaser' view modes.
      entity_get_display('crm_core_activity', $this->id(), 'default')
        ->setComponent('activity_participants', array(
          'label' => 'above',
          'module' => 'entityreference',
          'settings' => array(
            'link' => 1,
          ),
          'type' => 'entityreference_label',
          'weight' => '0',
        ))
        ->save();
    }
  }

  /**
   * Adds a date field for an activity type.
   *
   * @todo Check field and instance settings.
   */
  protected function ensureDateField() {
    $field_name = 'activity_participants';

    $field = FieldConfig::loadByName('crm_core_activity', $field_name);
    $instance = FieldInstanceConfig::loadByName('crm_core_activity', $this->id(), $field_name);

    if (empty($field)) {
      $field = entity_create('field_config', array(
        'name' => $field_name,
        'type' => 'datetime',
        'entity_type' => 'crm_core_activity',
        'active' => TRUE,
        'translatable' => FALSE,
        // Allow admin to change settings of this field as for
        // example meeting might need end date.
        'locked' => FALSE,
        'cardinality' => 1,
        'settings' => array(
          'repeat' => 0,
          'granularity' => array(
            'month' => 'month',
            'day' => 'day',
            'hour' => 'hour',
            'minute' => 'minute',
            'year' => 'year',
            'second' => 0,
          ),
          'tz_handling' => 'site',
          'timezone_db' => 'UTC',
          'todate' => '',
        ),
      ));
      $field->save();
    }

    if (empty($instance)) {
      $instance = entity_create('field_instance_config', array(
        'field_name' => $field_name,
        'entity_type' => 'crm_core_activity',
        'bundle' => $this->id(),
        'label' => t('Date'),
        'required' => FALSE,
        'settings' => array(
          'default_value' => 'now',
          'default_value_code' => '',
          'default_value2' => 'blank',
          'default_value_code2' => '',
          'user_register_form' => FALSE,
        ),
      ));
      $instance->save();

      // Assign widget settings for the 'default' form mode.
      entity_get_form_display('crm_core_activity', $this->id(), 'default')
        ->setComponent('activity_participants', array(
          'type' => 'datetime_default',
          'active' => 1,
          'settings' => array(
            'input_format' => 'm/d/Y - H:i:s',
            'input_format_custom' => '',
            'year_range' => '-3:+3',
            'increment' => '15',
            'label_position' => 'above',
            'text_parts' => array(),
            'repeat_collapsed' => 0,
          ),
        ))
        ->save();

      // Assign display settings for the 'default' and 'teaser' view modes.
      entity_get_display('crm_core_activity', $this->id(), 'default')
        ->setComponent('activity_participants', array(
          'label' => 'above',
          'settings' => array(
            'format_type' => 'long',
            'show_repeat_rule' => 'show',
            'multiple_number' => '',
            'multiple_from' => '',
            'multiple_to' => '',
            'fromto' => 'both',
          ),
          'type' => 'datetime_default',
          'weight' => 1,
        ))
        ->save();
    }
  }

  /**
   * Adds a note field for an activity type.
   *
   * @todo Check field and instance settings.
   */
  protected function ensureNotesField() {
    $field_name = 'activity_notes';

    $field = FieldConfig::loadByName('crm_core_activity', $field_name);
    $instance = FieldInstanceConfig::loadByName('crm_core_activity', $this->id(), $field_name);

    if (empty($field)) {
      $field = entity_create('field_config', array(
        'name' => $field_name,
        'type' => 'text_long',
        'entity_type' => 'crm_core_activity',
        'active' => TRUE,
        'translatable' => FALSE,
        'locked' => TRUE,
        'cardinality' => 1,
        'settings' => array(
          'repeat' => 0,
          'granularity' => array(
            'month' => 'month',
            'day' => 'day',
            'hour' => 'hour',
            'minute' => 'minute',
            'year' => 'year',
            'second' => 0,
          ),
          'tz_handling' => 'site',
          'timezone_db' => 'UTC',
          'todate' => '',
        ),
      ));
      $field->save();
    }

    if (empty($instance)) {
      $instance = entity_create('field_instance_config', array(
        'field_name' => $field_name,
        'entity_type' => 'crm_core_activity',
        'bundle' => $this->id(),
        'label' => t('Notes'),
        'required' => FALSE,
        'settings' => array(
          'text_processing' => '0',
          'user_register_form' => FALSE,
        ),
      ));
      $instance->save();

      // Assign widget settings for the 'default' form mode.
      entity_get_form_display('crm_core_activity', $this->id(), 'default')
        ->setComponent('activity_participants', array(
          'type' => 'text_textarea',
          'active' => 1,
          'weight' => 2,
          'settings' => array(
            'rows' => 5,
          ),
        ))
        ->save();

      // Assign display settings for the 'default' and 'teaser' view modes.
      entity_get_display('crm_core_activity', $this->id(), 'default')
        ->setComponent('activity_participants', array(
          'label' => 'above',
          'settings' => array(),
          'type' => 'text_default',
          'weight' => 2,
        ))
        ->save();
    }
  }
}

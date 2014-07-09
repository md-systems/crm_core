<?php
/**
 * @file
 * Contains \Drupal\crm_core_activity\Entity\ActivityType.
 */

namespace Drupal\crm_core_activity\Entity;

use Drupal\Core\Config\Entity\ConfigEntityBundleBase;
use Drupal\Core\Entity\EntityInterface;
use Drupal\Core\Entity\EntityStorageInterface;
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
 *   admin_permission = "administer activity types",
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
class ActivityType extends ConfigEntityBundleBase {

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

    $instance = FieldInstanceConfig::loadByName('crm_core_activity', $this->id(), $field_name);

    if (empty($instance)) {
      entity_create('field_instance_config', array(
        'field_name' => $field_name,
        'entity_type' => 'crm_core_activity',
        'bundle' => $this->id(),
        'label' => t('Participants'),
        'required' => TRUE,
        'settings' => array(
          'handler' => 'default',
        ),
      ))->save();

      // Assign widget settings for the 'default' form mode.
      entity_get_form_display('crm_core_activity', $this->id(), 'default')
        ->setComponent($field_name, array(
          'type' => 'entity_reference_autocomplete',
          'settings' => array(
            'match_operator' => 'CONTAINS',
          ),
        ))
        ->save();

      // Assign display settings for the 'default' and 'teaser' view modes.
      entity_get_display('crm_core_activity', $this->id(), 'default')
        ->setComponent($field_name, array(
          'label' => 'above',
          'settings' => array(
            'link' => TRUE,
          ),
          'type' => 'entity_reference_label',
          'weight' => 0,
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
    $field_name = 'activity_date';

    $instance = FieldInstanceConfig::loadByName('crm_core_activity', $this->id(), $field_name);

    if (empty($instance)) {
      $instance = entity_create('field_instance_config', array(
        'field_name' => $field_name,
        'entity_type' => 'crm_core_activity',
        'bundle' => $this->id(),
        'label' => t('Date'),
        'required' => FALSE,
        'default_value' => array(
          'default_date' => 'now',
        ),
      ));
      $instance->save();

      // Assign widget settings for the 'default' form mode.
      entity_get_form_display('crm_core_activity', $this->id(), 'default')
        ->setComponent($field_name, array(
          'type' => 'datetime_default',
          'weight' => 2,
        ))
        ->save();

      // Assign display settings for the 'default' and 'teaser' view modes.
      entity_get_display('crm_core_activity', $this->id(), 'default')
        ->setComponent($field_name, array(
          'label' => 'above',
          'settings' => array(
            'format_type' => 'long',
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

    $instance = FieldInstanceConfig::loadByName('crm_core_activity', $this->id(), $field_name);

    if (empty($instance)) {
      $instance = entity_create('field_instance_config', array(
        'field_name' => $field_name,
        'entity_type' => 'crm_core_activity',
        'bundle' => $this->id(),
        'label' => t('Notes'),
        'required' => FALSE,
      ));
      $instance->save();

      // Assign widget settings for the 'default' form mode.
      entity_get_form_display('crm_core_activity', $this->id(), 'default')
        ->setComponent($field_name, array(
          'type' => 'text_textarea',
          'weight' => 3,
          'settings' => array(
            'rows' => 5,
          ),
        ))
        ->save();

      // Assign display settings for the 'default' and 'teaser' view modes.
      entity_get_display('crm_core_activity', $this->id(), 'default')
        ->setComponent($field_name, array(
          'label' => 'above',
          'type' => 'text_default',
          'weight' => 2,
        ))
        ->save();
    }
  }
}

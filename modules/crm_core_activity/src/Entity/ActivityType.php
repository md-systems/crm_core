<?php
/**
 * @file
 * Contains \Drupal\crm_core_activity\Entity\ActivityType.
 */

namespace Drupal\crm_core_activity\Entity;

use Drupal\Core\Config\Entity\ConfigEntityBundleBase;
use Drupal\Core\Entity\EntityInterface;
use Drupal\Core\Entity\EntityStorageInterface;
use Drupal\field\Entity\FieldConfig;

/**
 * CRM Activity Type Entity Class.
 *
 * @ConfigEntityType(
 *   id = "crm_core_activity_type",
 *   label = @Translation("CRM Core Activity type"),
 *   bundle_of = "crm_core_activity",
 *   config_prefix = "type",
 *   handlers = {
 *     "access" = "Drupal\crm_core_activity\ActivityTypeAccessControlHandler",
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
 *   config_export = {
 *     "name",
 *     "type",
 *     "description",
 *     "activity_string",
 *   },
 *   links = {
 *     "canonical" = "admin/structure/crm-core/activity-types/{crm_core_activity_type}",
 *     "add-form" = "admin/structure/crm-core/activity-types/add",
 *     "edit-form" = "admin/structure/crm-core/activity-types/{crm_core_activity_type}",
 *     "delete-form" = "admin/structure/crm-core/activity-types/{crm_core_activity_type}/delete",
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
   * {@inheritdoc}
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
      \Drupal::logger('crm_core_activity')->info('Delete !count activities due to deletion of activity type.', array('!count' => count($results)));
    }
  }

}

<?php
/**
 * @file
 * Contains \Drupal\crm_core_contact\Entity\ContactType.
 */

namespace Drupal\crm_core_contact\Entity;

use Drupal\Component\Utility\NestedArray;
use Drupal\Core\Config\Entity\ConfigEntityBundleBase;
use Drupal\Core\Entity\EntityInterface;
use Drupal\Core\Entity\EntityStorageInterface;
use Drupal\field\Entity\FieldInstanceConfig;

/**
 * CRM Contact Type Entity Class.
 *
 * @ConfigEntityType(
 *   id = "crm_core_contact_type",
 *   label = @Translation("CRM Core Contact type"),
 *   bundle_of = "crm_core_contact",
 *   config_prefix = "type",
 *   controllers = {
 *     "access" = "Drupal\crm_core_contact\ContactTypeAccessController",
 *     "form" = {
 *       "default" = "Drupal\crm_core_contact\Form\ContactTypeForm",
 *       "delete" = "Drupal\crm_core_contact\Form\ContactTypeDeleteForm",
 *       "toggle" = "Drupal\crm_core_contact\Form\ContactTypeToggleForm",
 *     },
 *     "list_builder" = "Drupal\crm_core_contact\ContactTypeListBuilder",
 *   },
 *   admin_permission = "administer contact types",
 *   entity_keys = {
 *     "id" = "type",
 *     "label" = "name",
 *     "status" = "disabled",
 *   },
 *   links = {
 *     "canonical" = "crm_core_contact.type_edit",
 *     "add-form" = "crm_core_contact.type_add",
 *     "edit-form" = "crm_core_contact.type_edit",
 *     "delete-form" = "crm_core_contact.type_delete",
 *     "enable" = "crm_core_contact.type_enable",
 *     "disable" = "crm_core_contact.type_disable",
 *   }
 * )
 */
class ContactType extends ConfigEntityBundleBase {

  /**
   * The machine-readable name of this type.
   *
   * @var string
   */
  public $type;

  /**
   * The human-readable name of this type.
   *
   * @var string
   */
  public $name;

  /**
   * A brief description of this type.
   *
   * @var string
   */
  public $description;

  /**
   * Whether or not this type is locked.
   *
   * A boolean indicating whether this type is locked or not, locked contact
   * type cannot be edited or disabled/deleted.
   *
   * @var boolean
   */
  public $locked;

  /**
   * Primary fields.
   *
   * An array of key-value pairs, where key is the primary field type and value
   * is real field name used for this type.
   *
   * @var array
   */
  public $primary_fields;

  /**
   * {@inheritdoc}
   */
  public function id() {
    return $this->type;
  }

  /**
   * Gets the lock status.
   *
   * The entity is considered locked if the entity is enabled an not new.
   *
   * @return bool
   *   TRUE if locked, FALSE otherwise.
   */
  public function isLocked() {
    return isset($this->status) && !$this->isNew();
  }

  /**
   * {@inheritdoc}
   */
  public static function preCreate(EntityStorageInterface $storage, array &$values) {
    parent::preCreate($storage, $values);

    // Ensure default values are set.
    $values += array(
      'locked' => FALSE,
    );
  }

  /**
   * {@inheritdoc}
   *
   * @todo This does not scale.
   *
   * Deleting a contact type with thousands of contact records associated will
   * run into execution timeout.
   */
  public static function preDelete(EntityStorageInterface $storage, array $entities) {
    parent::preDelete($storage, $entities);

    $ids = array_map(function(EntityInterface $entity){
      return $entity->id();
    }, $entities);

    // Delete all instances of the given type.
    $results = \Drupal::entityQuery('crm_core_contact')
      ->condition('type', $ids, 'IN')
      ->execute();

    if (!empty($results)) {
      $contacts = Contact::loadMultiple($results);
      \Drupal::entityManager()->getStorage('crm_core_contact')->delete($contacts);
      watchdog('crm_core_contact', 'Delete !count contacts due to deletion of contact type.', array('!count' => count($results)), WATCHDOG_INFO);
    }
  }

  /**
   * Loads all enabled Contact Types.
   *
   * @return \Drupal\crm_core_contact\Entity\ContactType[]
   *   An array of contact types indexed by their IDs.
   */
  public static function loadActive() {
    $ids = \Drupal::entityQuery('crm_core_contact')
      ->condition('status', TRUE)
      ->execute();

    return ContactType::loadMultiple($ids);
  }
}

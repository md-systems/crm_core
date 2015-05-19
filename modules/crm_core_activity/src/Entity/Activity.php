<?php
/**
 * @file
 * Contains Drupal\crm_core_activity\Entity\Activity.
 */

namespace Drupal\crm_core_activity\Entity;

use Drupal\Core\Entity\ContentEntityBase;
use Drupal\Core\Entity\EntityChangedTrait;
use Drupal\Core\Entity\EntityStorageInterface;
use Drupal\Core\Entity\EntityTypeInterface;
use Drupal\Core\Field\BaseFieldDefinition;
use Drupal\crm_core_activity\ActivityInterface;
use Drupal\crm_core_contact\ContactInterface;

/**
 * CRM Activity Entity Class.
 *
 * @ContentEntityType(
 *   id = "crm_core_activity",
 *   label = @Translation("CRM Core Activity"),
 *   bundle_label = @Translation("Activity type"),
 *   label_callback = "Drupal\crm_core_activity\Entity\Activity::defaultLabel",
 *   handlers = {
 *     "access" = "Drupal\crm_core_activity\ActivityAccessControlHandler",
 *     "form" = {
 *       "default" = "Drupal\crm_core_activity\Form\ActivityForm",
 *       "delete" = "Drupal\crm_core_activity\Form\ActivityDeleteForm",
 *     },
 *     "list_builder" = "Drupal\crm_core_activity\ActivityListBuilder",
 *     "view_builder" = "Drupal\Core\Entity\EntityViewBuilder",
 *   },
 *   base_table = "crm_core_activity",
 *   revision_table = "crm_core_activity_revision",
 *   fieldable = TRUE,
 *   entity_keys = {
 *     "id" = "activity_id",
 *     "revision" = "revision_id",
 *     "bundle" = "type",
 *     "uuid" = "uuid",
 *     "label" = "title",
 *     "user" = "uid",
 *   },
 *   bundle_entity_type = "crm_core_activity_type",
 *   permission_granularity = "bundle",
 *   permission_labels = {
 *     "singular" = @Translation("Activity"),
 *     "plural" = @Translation("Activities"),
 *   },
 *   links = {
 *     "canonical" = "crm-core/activity/{crm_core_activity}",
 *     "delete-form" = "crm-core/activity/{crm_core_activity}/delete",
 *     "edit-form" = "crm-core/activity/{crm_core_activity}/edit",
 *     "version-history" = "crm_core_activity.revision_list",
 *     "admin-form" = "crm_core_activity.type_edit"
 *   }
 * )
 *
 * @todo Add Views support.
 * @todo Replace list builder with a view.
 */
class Activity extends ContentEntityBase implements ActivityInterface {

  use EntityChangedTrait;

  /**
   * {@inheritdoc}
   */
  public static function baseFieldDefinitions(EntityTypeInterface $entity_type) {
    $fields = array();

    $fields['activity_id'] = BaseFieldDefinition::create('integer')
      ->setLabel(t('Activity ID'))
      ->setDescription(t('The activity ID.'))
      ->setReadOnly(TRUE)
      ->setSetting('unsigned', TRUE);

    $fields['uuid'] = BaseFieldDefinition::create('uuid')
      ->setLabel(t('UUID'))
      ->setDescription(t('The node UUID.'))
      ->setReadOnly(TRUE);

    $fields['revision_id'] = BaseFieldDefinition::create('integer')
      ->setLabel(t('Revision ID'))
      ->setDescription(t('The activity revision ID.'))
      ->setReadOnly(TRUE)
      ->setSetting('unsigned', TRUE);

    // @todo Update once https://drupal.org/node/1979260 is done.
    $fields['uid'] = BaseFieldDefinition::create('entity_reference')
      ->setLabel(t('Owner'))
      ->setDescription(t('The user that created the activity.'))
      ->setRevisionable(TRUE)
      ->setSettings(array(
        'target_type' => 'user',
        'default_value' => 0,
      ));

    $fields['type'] = BaseFieldDefinition::create('entity_reference')
      ->setLabel(t('Type'))
      ->setDescription(t('The activity type.'))
      ->setSetting('target_type', 'crm_core_activity_type')
      ->setReadOnly(TRUE);

    $fields['title'] = BaseFieldDefinition::create('string')
      ->setLabel(t('Title'))
      ->setDescription(t('The title of this activity.'))
      ->setRequired(TRUE)
      ->setRevisionable(TRUE)
      ->setSetting('default_value', '')
      ->setSetting('max_length', 255)
      ->setDisplayOptions('view', array(
        'label' => 'hidden',
        'type' => 'string',
        'weight' => -5,
      ))
      ->setDisplayOptions('form', array(
        'type' => 'text_textfield',
        'weight' => -5,
      ))
      ->setDisplayConfigurable('form', TRUE);

    $fields['created'] = BaseFieldDefinition::create('created')
      ->setLabel(t('Created'))
      ->setDescription(t('The time that the activity was created.'))
      ->setRevisionable(TRUE)
      ->setDisplayOptions('form', array(
        'type' => 'number',
        'weight' => -5,
      ))
      ->setDisplayConfigurable('form', TRUE);;

    $fields['changed'] = BaseFieldDefinition::create('changed')
      ->setLabel(t('Changed'))
      ->setDescription(t('The time that the activity was last edited.'))
      ->setRevisionable(TRUE);

    // @todo Check settings.
    $fields['activity_participants'] = BaseFieldDefinition::create('entity_reference')
      ->setLabel(t('Participants'))
      ->setSetting('target_type', 'crm_core_contact')
      ->setCardinality(-1)
      ->setRequired(TRUE)
      ->setDisplayOptions('view', [
        'label' => 'above',
        'settings' => [
          'link' => TRUE,
        ],
        'type' => 'entity_reference_label',
        'weight' => 0,
      ])
      ->setDisplayOptions('form', [
        'type' => 'entity_reference_autocomplete',
        'settings' => [
          'match_operator' => 'CONTAINS',
        ],
        'weight' => 0,
      ]);

    // @todo Check settings.
    $fields['activity_date'] = BaseFieldDefinition::create('datetime')
      ->setLabel(t('Date'))
      ->setDefaultValue(['default_date' => 'now'])
      ->setDisplayOptions('view', [
        'label' => 'above',
        'settings' => [
          'format_type' => 'long',
        ],
        'type' => 'datetime_default',
        'weight' => 1,
      ])
      ->setDisplayOptions('form', [
        'type' => 'datetime_default',
        'weight' => 2,
      ]);

    // @todo Check settings.
    $fields['activity_notes'] = BaseFieldDefinition::create('text_long')
      ->setLabel(t('Notes'))
      ->setDisplayOptions('view', [
        'label' => 'above',
        'type' => 'text_default',
        'weight' => 2,
      ])
      ->setDisplayOptions('form', [
        'type' => 'text_textarea',
        'weight' => 3,
        'settings' => [
          'rows' => 5,
        ],
      ]);

    return $fields;
  }

  /**
   * {@inheritdoc}
   *
   * @todo Remove once https://drupal.org/node/1979260 is done.
   */
  public static function preCreate(EntityStorageInterface $storage, array &$values) {
    parent::preCreate($storage, $values);

    $account = \Drupal::currentUser();

    // Set user id of contact owner.
    $values += array(
      'uid' => $account->id(),
    );
  }

  /**
   * {@inheritdoc}
   */
  public function preSaveRevision(EntityStorageInterface $storage, \stdClass $record) {
    parent::preSaveRevision($storage, $record);

    $account = \Drupal::currentUser();
    $record->uid = $account->id();
  }

  /**
   * {@inheritdoc}
   */
  public function addParticipant(ContactInterface $contact) {
    $this->get('activity_participants')->appendItem($contact);
  }

  /**
   * {@inheritdoc}
   */
  public function getChangedTime() {
    return $this->changed;
  }
}

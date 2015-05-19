<?php
/**
 * @file
 * Contains Drupal\crm_core_contact\Entity\Contact.
 */

namespace Drupal\crm_core_contact\Entity;

use Drupal\Core\Entity\ContentEntityBase;
use Drupal\Core\Entity\EntityChangedTrait;
use Drupal\Core\Entity\EntityStorageInterface;
use Drupal\Core\Entity\EntityTypeInterface;
use Drupal\Core\Field\BaseFieldDefinition;
use Drupal\crm_core_contact\ContactInterface;

/**
 * CRM Contact Entity Class.
 *
 * @ContentEntityType(
 *   id = "crm_core_contact",
 *   label = @Translation("CRM Core Contact"),
 *   bundle_label = @Translation("Contact type"),
 *   label_callback = "Drupal\crm_core_contact\Entity\Contact::labelCallback",
 *   handlers = {
 *     "access" = "Drupal\crm_core_contact\ContactAccessControlHandler",
 *     "form" = {
 *       "default" = "Drupal\crm_core_contact\Form\ContactForm",
 *       "delete" = "Drupal\crm_core_contact\Form\ContactDeleteForm",
 *     },
 *     "view_builder" = "Drupal\Core\Entity\EntityViewBuilder",
 *     "list_builder" = "Drupal\crm_core_contact\ContactListBuilder",
 *   },
 *   base_table = "crm_core_contact",
 *   revision_table = "crm_core_contact_revision",
 *   fieldable = TRUE,
 *   entity_keys = {
 *     "id" = "contact_id",
 *     "revision" = "vid",
 *     "bundle" = "type",
 *     "uuid" = "uuid",
 *     "user" = "uid",
 *   },
 *   bundle_entity_type = "crm_core_contact_type",
 *   permission_granularity = "bundle",
 *   permission_labels = {
 *     "singular" = @Translation("Contact"),
 *     "plural" = @Translation("Contacts"),
 *   },
 *   links = {
 *     "canonical" = "crm-core/contact/{crm_core_contact}",
 *     "collection" = "crm-core/contact",
 *     "edit_form" = "crm-core/contact/{crm_core_contact}/edit",
 *     "delete_form" = "crm-core/contact/{crm_core_contact}/delete"
 *   }
 * )
 *
 * @todo Add Views support.
 */
class Contact extends ContentEntityBase implements ContactInterface {

  use EntityChangedTrait;

  /**
   * {@inheritdoc}
   */
  public static function baseFieldDefinitions(EntityTypeInterface $entity_type) {
    $fields = array();

    $fields['contact_id'] = BaseFieldDefinition::create('integer')
      ->setLabel(t('Contact ID'))
      ->setDescription(t('The contact ID.'))
      ->setReadOnly(TRUE)
      ->setSetting('unsigned', TRUE);

    $fields['uuid'] = BaseFieldDefinition::create('uuid')
      ->setLabel(t('UUID'))
      ->setDescription(t('The contact UUID.'))
      ->setReadOnly(TRUE);

    $fields['vid'] = BaseFieldDefinition::create('integer')
      ->setLabel(t('Revision ID'))
      ->setDescription(t('The contact revision ID.'))
      ->setReadOnly(TRUE)
      ->setSetting('unsigned', TRUE);

    $fields['type'] = BaseFieldDefinition::create('entity_reference')
      ->setLabel(t('Type'))
      ->setDescription(t('The contact type.'))
      ->setSetting('target_type', 'crm_core_contact_type')
      ->setReadOnly(TRUE);

    $fields['created'] = BaseFieldDefinition::create('created')
      ->setLabel(t('Created'))
      ->setDescription(t('The time that the contact was created.'))
      ->setRevisionable(TRUE)
      ->setDisplayOptions('form', array(
        'type' => 'number',
        'weight' => -5,
      ))
      ->setDisplayConfigurable('form', TRUE);;

    $fields['changed'] = BaseFieldDefinition::create('changed')
      ->setLabel(t('Changed'))
      ->setDescription(t('The time that the contact was last edited.'))
      ->setRevisionable(TRUE);

    // @todo Update once https://drupal.org/node/1979260 is done.
    $fields['uid'] = BaseFieldDefinition::create('entity_reference')
      ->setLabel(t('Owner'))
      ->setDescription(t('The user that is the contact owner.'))
      ->setRevisionable(TRUE)
      ->setSettings(array(
        'target_type' => 'user',
        'default_value' => 0,
      ));

    // @todo Make this a name field once it gets available.
    $fields['name'] = BaseFieldDefinition::create('string')
      ->setLabel(t('Name'))
      ->setDisplayOptions('form', array(
        'type' => 'text_textfield',
        'weight' => 0,
      ))
      ->setDisplayOptions('view', array(
        'label' => 'hidden',
        'type' => 'string',
        'weight' => 0,
      ));

    $fields['revision_log'] = BaseFieldDefinition::create('string_long')
      ->setLabel(t('Revision log message'))
      ->setDescription(t('The log entry explaining the changes in this revision.'))
      ->setRevisionable(TRUE)
      ->setTranslatable(TRUE)
      ->setDisplayOptions('form', array(
          'type' => 'text_textarea',
          'weight' => 25,
          'settings' => array(
            'rows' => 4,
          ),
        ));

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

    if (!isset($record->revision_log)) {
      $record->revision_log = '';
    }

    $account = \Drupal::currentUser();
    $record->uid = $account->id();
  }

  /**
   * Gets the primary address.
   *
   * @return \Drupal\Core\Field\FieldItemListInterface|\Drupal\Core\TypedData\TypedDataInterface
   *   The address property object.
   */
  public function getPrimaryAddress() {
    return $this->getPrimaryField('address');
  }

  /**
   * Gets the primary email.
   *
   * @return \Drupal\Core\Field\FieldItemListInterface|\Drupal\Core\TypedData\TypedDataInterface
   *   The email property object.
   */
  public function getPrimaryEmail() {
    return $this->getPrimaryField('email');
  }

  /**
   * Gets the primary phone.
   *
   * @return \Drupal\Core\Field\FieldItemListInterface|\Drupal\Core\TypedData\TypedDataInterface
   *   The phone property object.
   */
  public function getPrimaryPhone() {
    return $this->getPrimaryField('phone');
  }

  /**
   * Gets the primary field.
   *
   * @param string $field
   *   The primary field name.
   *
   * @return \Drupal\Core\Field\FieldItemListInterface|\Drupal\Core\TypedData\TypedDataInterface
   *   The primary field property object.
   *
   * @throws \InvalidArgumentException
   *   If no primary field is configured.
   *   If the configured primary field does not exist.
   */
  public function getPrimaryField($field) {
    $type = $this->get('type')->entity;
    $name = empty($type->primary_fields[$field]) ? '' : $type->primary_fields[$field];
    return $this->get($name);
  }

  /**
   * Returns the label of the contact.
   *
   * @param \Drupal\crm_core_contact\Entity\Contact $entity
   *   The Contact entity.
   *
   * @return string
   *   Contact label.
   */
  public static function labelCallback(Contact $entity) {
    // @todo Replace with the value of the contact_name field, when name module will be available.
    $label = $entity->get('name')->value;
    \Drupal::moduleHandler()->alter('crm_core_contact_label', $label, $entity);

    return $label;
  }

  /**
   * {@inheritdoc}
   */
  public function getChangedTime() {
    return $this->changed;
  }

  /**
   * {@inheritdoc}
   */
  public function label() {
    $label = parent::label();
    if (empty($label)) {
      $label = t('Nameless #@id', ['@id' => $this->id()]);
    }
    return $label;
  }

}

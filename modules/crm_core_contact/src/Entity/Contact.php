<?php
/**
 * @file
 * Contains Drupal\crm_core_contact\Entity\Contact.
 */

namespace Drupal\crm_core_contact\Entity;

use Drupal\Core\Entity\ContentEntityBase;
use Drupal\Core\Entity\EntityStorageInterface;
use Drupal\Core\Entity\EntityTypeInterface;
use Drupal\Core\Field\FieldDefinition;

/**
 * CRM Contact Entity Class.
 *
 * @ContentEntityType(
 *   id = "crm_core_contact",
 *   label = @Translation("CRM Core Contact"),
 *   bundle_label = @Translation("Contact type"),
 *   label_callback = "Drupal\crm_core_contact\Entity\Contact::defaultLabel",
 *   controllers = {
 *     "access" = "Drupal\crm_core_contact\ContactAccessController",
 *     "form" = {
 *       "default" = "Drupal\crm_core_contact\Form\ContactForm",
 *       "delete" = "Drupal\crm_core_contact\Form\ContactDeleteForm",
 *     },
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
 *     "canonical" = "crm_core_contact.view",
 *     "delete-form" = "crm_core_contact.delete_confirm",
 *     "edit-form" = "crm_core_contact.edit",
 *     "version-history" = "crm_core_contact.revision_list",
 *     "admin-form" = "crm_core_contact.type_edit"
 *   }
 * )
 *
 * @todo Add Views support.
 * @todo Replace list builder with a view.
 */
class Contact extends ContentEntityBase {

  /**
   * Label callback.
   *
   * @param \Drupal\crm_core_contact\Entity\Contact $contact
   *   The contact entity object.
   *
   * @return string
   *   Raw formatted string. This should be run through check_plain().
   */
  public static function defaultLabel(Contact $contact) {
    // Check whether bundle type label function exists.
    // This is needed if we want to have different labels per contact type.
    // For example Individual contact's label is person's Name.
    // But for Organization -- organization's name.
    $function = 'crm_core_contact_' . $contact->bundle() . '_label';
    if (function_exists($function)) {
      return $function($contact);
    }

    // @todo Use rendered field
    // The Drupal 7 version of CRM Core returns the rendered value of the name
    // field. Restore that behaviour once the name field module is used again.
    return $contact->get('contact_name')->value;
  }

  /**
   * Method for de-duplicating contacts.
   *
   * Allows various modules to identify duplicate contact records through
   * hook_crm_core_contact_match. This function should implement it's
   * own contact matching scheme.
   *
   * @param Contact $entity
   *   CRM Core Contact
   *
   * @return array
   *   Array of matched contacts.
   */
  public function match(Contact $entity) {

    $checks = & drupal_static(__FUNCTION__);
    $matches = array();

    if (!isset($checks->processed)) {
      $checks = new stdClass();
      $checks->engines = module_implements('crm_core_contact_match');
      $checks->processed = 1;
    }

    // Pass in the contact and the matches array as references.
    // This will allow various matching tools to modify the contact
    // and the list of matches.
    $values = array(
      'contact' => &$entity,
      'matches' => &$matches,
    );
    foreach ($checks->engines as $module) {
      module_invoke($module, 'crm_core_contact_match', $values);
    }

    // It's up to implementing modules to handle the matching logic.
    // Most often, the match to be used should be the one
    // at the top of the stack.
    return $matches;
  }

  /**
   * {@inheritdoc}
   */
  public static function baseFieldDefinitions(EntityTypeInterface $entity_type) {
    $fields = array();

    $fields['contact_id'] = FieldDefinition::create('integer')
      ->setLabel(t('Contact ID'))
      ->setDescription(t('The contact ID.'))
      ->setReadOnly(TRUE)
      ->setSetting('unsigned', TRUE);

    $fields['uuid'] = FieldDefinition::create('uuid')
      ->setLabel(t('UUID'))
      ->setDescription(t('The node UUID.'))
      ->setReadOnly(TRUE);

    $fields['vid'] = FieldDefinition::create('integer')
      ->setLabel(t('Revision ID'))
      ->setDescription(t('The contact revision ID.'))
      ->setReadOnly(TRUE)
      ->setSetting('unsigned', TRUE);

    $fields['type'] = FieldDefinition::create('entity_reference')
      ->setLabel(t('Type'))
      ->setDescription(t('The contact type.'))
      ->setSetting('target_type', 'crm_core_contact_type')
      ->setReadOnly(TRUE);

    $fields['created'] = FieldDefinition::create('created')
      ->setLabel(t('Created'))
      ->setDescription(t('The time that the contact was created.'))
      ->setRevisionable(TRUE)
      ->setDisplayOptions('form', array(
        'type' => 'integer',
        'weight' => -5,
      ))
      ->setDisplayConfigurable('form', TRUE);;

    $fields['changed'] = FieldDefinition::create('changed')
      ->setLabel(t('Changed'))
      ->setDescription(t('The time that the contact was last edited.'))
      ->setRevisionable(TRUE);

    // @todo Update once https://drupal.org/node/1979260 is done.
    $fields['uid'] = FieldDefinition::create('entity_reference')
      ->setLabel(t('Owner'))
      ->setDescription(t('The user that is the contact owner.'))
      ->setRevisionable(TRUE)
      ->setSettings(array(
        'target_type' => 'user',
        'default_value' => 0,
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

    if (!isset($record->log)) {
      $record->log = '';
    }

    $account = \Drupal::currentUser();
    $record->uid = $account->id();
  }

  /**
   * Constructs a new contact object, without saving it.
   *
   * @param array $values
   *   (optional) An array of values to set, keyed by property name. If the
   *   entity type has bundles, the bundle key has to be specified.
   *
   * @return \Drupal\crm_core_contact\Entity\Contact
   *   The entity object.
   *
   * @todo Review once https://drupal.org/node/2096899 got committed.
   */
  public static function create(array $values = array()) {
    return \Drupal::entityManager()->getStorage('crm_core_contact')->create($values);
  }
}

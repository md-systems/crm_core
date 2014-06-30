<?php
/**
 * @file
 * Contains \Drupal\crm_core_contact\Tests\ContactCRUDTest.
 */

namespace Drupal\crm_core_contact\Tests;

use Drupal\crm_core_contact\Entity\Contact;
use Drupal\crm_core_contact\Entity\ContactType;
use Drupal\simpletest\KernelTestBase;

/**
 * Tests CRUD of the entities in crm_core_contact.
 */
class ContactCRUDTest extends KernelTestBase {

  /**
   * Modules to enable.
   *
   * @var array
   */
  public static $modules = array(
    'entity',
    'field',
    'text',
    'user',
    'crm_core',
    'crm_core_contact',
  );

  /**
   * Gets the test information.
   */
  public static function getInfo() {
    return array(
      'name' => 'Contact CRUD',
      'description' => 'Tests create, read, update and delete of contacts and contact types.',
      'group' => 'CRM Core',
    );
  }

  /**
   * {@inheritdoc}
   */
  protected function setUp() {
    parent::setUp();

    $this->installConfig(array('field'));
    $this->installEntitySchema('crm_core_contact');
  }

  /**
   * Tests CRUD of contact types.
   */
  public function testContactType() {
    $type = 'dog';

    // Create.
    $contact_type = ContactType::create(array('type' => $type));
    $this->assertTrue(isset($contact_type->type) && $contact_type->type == $type, t('New contact type type exists.'));
    // @todo Check if this still must be the case.
//    $this->assertTrue($contact_type->locked, t('New contact type has locked set to TRUE.'));

    // crm_core_contact_type_save().
    $contact_type->name = $this->randomName();
    $contact_type->description = $this->randomString();
    $this->assertTrue($contact_type->save(), t('Contact type saved.'));

    // Load.
    $contact_type_load = ContactType::load($type);
    $this->assertEqual($contact_type->type, $contact_type_load->type, t('Loaded contact type has same type.'));
    $this->assertEqual($contact_type->name, $contact_type_load->name, t('Loaded contact type has same name.'));
    $this->assertEqual($contact_type->description, $contact_type_load->description, t('Loaded contact type has same description.'));
    $uuid = $contact_type_load->uuid();
    $this->assertTrue(!empty($uuid), 'Loaded contact type has uuid.');

    // Delete.
    $contact_type_load->delete();
    // Avoid static cache.
    $contact_type_load = ContactType::load($type);
    $this->assertTrue(empty($contact_type_load), t('Contact type deleted.'));
  }

  /**
   * Tests CRUD of contacts.
   *
   * @todo Check if working once https://drupal.org/node/2239969 got committed.
   */
  public function testContact() {
    $this->installEntitySchema('user');

    $type = ContactType::create(array('type' => 'test'));
    $type->save();

    // Create.
    $contact = Contact::create(array('type' => $type->type));
    $this->assertTrue($contact->save(), t('Contact saved.'));

    // Load.
    $contact_load = Contact::load($contact->id());
    $uuid = $contact_load->uuid();
    $this->assertTrue(!empty($uuid), 'Loaded contact has uuid.');

    // Delete.
    $contact->delete();
    // Avoid static cache.
    $contact_load = Contact::load($contact->id());
    $this->assertTrue(empty($contact_load), t('Contact deleted.'));
  }
}

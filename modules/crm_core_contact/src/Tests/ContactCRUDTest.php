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
 * Tests CRUD operations for the CRM Core Contact entity.
 *
 * @group crm_core
 */
class ContactCRUDTest extends KernelTestBase {

  /**
   * Modules to enable.
   *
   * @var array
   */
  public static $modules = array(
    'field',
    'text',
    'user',
    'crm_core',
    'crm_core_contact',
  );

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
    $this->assertTrue(isset($contact_type->type) && $contact_type->type == $type, 'New contact type type exists.');
    // @todo Check if this still must be the case.
//    $this->assertTrue($contact_type->locked, t('New contact type has locked set to TRUE.'));
    $contact_type->name = $this->randomMachineName();
    $contact_type->description = $this->randomString();
    $this->assertEqual(SAVED_NEW, $contact_type->save(), 'Contact type saved.');

    // Load.
    $contact_type_load = ContactType::load($type);
    $this->assertEqual($contact_type->type, $contact_type_load->type, 'Loaded contact type has same type.');
    $this->assertEqual($contact_type->name, $contact_type_load->name, 'Loaded contact type has same name.');
    $this->assertEqual($contact_type->description, $contact_type_load->description, 'Loaded contact type has same description.');
    $uuid = $contact_type_load->uuid();
    $this->assertTrue(!empty($uuid), 'Loaded contact type has uuid.');

    // Delete.
    $contact_type_load->delete();
    $contact_type_load = ContactType::load($type);
    $this->assertNull($contact_type_load, 'Contact type deleted.');
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
    $this->assertEqual(SAVED_NEW, $contact->save(), 'Contact saved.');

    // Load.
    $contact_load = Contact::load($contact->id());
    $uuid = $contact_load->uuid();
    $this->assertTrue(!empty($uuid), 'Loaded contact has uuid.');

    // Delete.
    $contact->delete();
    $contact_load = Contact::load($contact->id());
    $this->assertNull($contact_load, 'Contact deleted.');
  }
}

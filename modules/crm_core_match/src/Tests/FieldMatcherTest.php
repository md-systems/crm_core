<?php
/**
 * @file
 * Contains \Drupal\crm_core_match\Tests\FieldMatcherTest.
 */

namespace Drupal\crm_core_match\Tests;

use Drupal\crm_core_contact\Entity\Contact;
use Drupal\field\Entity\FieldConfig;
use Drupal\field\Entity\FieldStorageConfig;
use Drupal\simpletest\KernelTestBase;

/**
 * Tests the field matchers of the default matching engine.
 *
 * @group crm_core
 */
class FieldMatcherTest extends KernelTestBase {

  /**
   * {@inheritdoc}
   */
  public static $modules = array(
    'user',
    'field',
    'text',
    'crm_core_contact',
    'crm_core_match',
  );

  /**
   * The mocked match field plugin manager.
   *
   * @var \Drupal\Component\Plugin\PluginManagerInterface
   */
  protected $pluginManager;

  /**
   * {@inheritdoc}
   */
  protected function setUp() {
    parent::setUp();

    $this->installConfig(['crm_core_contact']);
    $this->installEntitySchema('crm_core_contact');

    $this->pluginManager = $this->container->get('plugin.manager.crm_core_match.match_field');
  }

  /**
   * Test the unsupported field.
   */
  public function testUnsupported() {
    $config = array(
      'value' => array(
        'operator' => '',
      ),
    );
    $contact_needle = Contact::create(array('type' => 'individual'));
    $contact_needle->save();

    $config['field'] = $contact_needle->getFieldDefinition('uuid');
    /* @var \Drupal\crm_core_match\Plugin\crm_core_match\field\FieldHandlerInterface $unsupported */
    $unsupported = $this->pluginManager->createInstance('unsupported', $config);

    $ids = $unsupported->match($contact_needle);
    $this->assertTrue(empty($ids), 'Empty result for unsupported match');
  }

  /**
   * Test the text field.
   */
  public function testName() {
    $config = array(
      'value' => array(
        'operator' => '=',
        'score' => 42,
      ),
    );
    /** @var Contact $contact_needle */
    $contact_needle = Contact::create(array('type' => 'individual'));
    $contact_needle->set('name', 'Mr Gimeno Boomer');
    $contact_needle->save();
    /** @var Contact $contact_match */
    $contact_match = Contact::create(array('type' => 'individual'));
    $contact_match->set('name', 'Gimeno Boomer, Mr');
    $contact_match->save();
    /** @var Contact $contact_match2 */
    $contact_match2 = Contact::create(array('type' => 'individual'));
    $contact_match2->set('name', 'Rodrigo Boomer, Mr');
    $contact_match2->save();

    $config['field'] = $contact_needle->getFieldDefinition('name');
    /* @var \Drupal\crm_core_match\Plugin\crm_core_match\field\FieldHandlerInterface $text */
    $text = $this->pluginManager->createInstance('name', $config);

    $ids = $text->match($contact_needle);
    $this->assertTrue(array_key_exists($contact_match->id(), $ids), 'Text match returns expected match');
    $this->assertTrue(array_key_exists($contact_match2->id(), $ids), 'Text match returns expected match');
    $this->assertEqual(42, $ids[$contact_match->id()]['name.value'], 'Got expected match score');
    $this->assertEqual(21, $ids[$contact_match2->id()]['name.value'], 'Got expected match score');
  }

  /**
   * Test the text field.
   */
  public function testText() {
    $config = array(
      'value' => array(
        'operator' => '=',
        'score' => 42,
      ),
    );
    $contact_needle = Contact::create(array('type' => 'individual'));
    $contact_needle->set('name', 'Boomer');
    $contact_needle->save();
    $contact_match = Contact::create(array('type' => 'individual'));
    $contact_match->set('name', 'Boomer');
    $contact_match->save();

    $config['field'] = $contact_needle->getFieldDefinition('name');
    /* @var \Drupal\crm_core_match\Plugin\crm_core_match\field\FieldHandlerInterface $text */
    $text = $this->pluginManager->createInstance('text', $config);

    $ids = $text->match($contact_needle);
    $this->assertTrue(array_key_exists($contact_match->id(), $ids), 'Text match returns expected match');
    $this->assertEqual(42, $ids[$contact_match->id()]['name.value'], 'Got expected match score');
  }

  /**
   * Test the email field.
   */
  public function testEmail() {
    FieldStorageConfig::create(array(
      'entity_type' => 'crm_core_contact',
      'type' => 'email',
      'field_name' => 'contact_mail',
    ))->save();
    FieldConfig::create(array(
      'field_name' => 'contact_mail',
      'entity_type' => 'crm_core_contact',
      'bundle' => 'individual',
      'label' => t('Email'),
      'required' => FALSE,
    ))->save();

    $config = array(
      'value' => array(
        'operator' => '=',
        'score' => 42,
      ),
    );
    $contact_needle = Contact::create(array('type' => 'individual'));
    $contact_needle->set('contact_mail', 'boomer@example.com');
    $contact_needle->save();
    $contact_match = Contact::create(array('type' => 'individual'));
    $contact_match->set('contact_mail', 'boomer@example.com');
    $contact_match->save();

    $config['field'] = $contact_needle->getFieldDefinition('contact_mail');
    /* @var \Drupal\crm_core_match\Plugin\crm_core_match\field\FieldHandlerInterface $text */
    $text = $this->pluginManager->createInstance('email', $config);

    $ids = $text->match($contact_needle);
    $this->assertTrue(array_key_exists($contact_match->id(), $ids), 'Text match returns expected match');
    $this->assertEqual(42, $ids[$contact_match->id()]['contact_mail.value'], 'Got expected match score');
  }
}

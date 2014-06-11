<?php
/**
 * @file
 * Contains \Drupal\crm_core_default_matching_engine\Tests\FieldMatcherTest.
 */

namespace Drupal\crm_core_default_matching_engine\Tests;

use Drupal\crm_core_contact\Entity\Contact;
use Drupal\crm_core_contact\Entity\ContactType;
use Drupal\simpletest\KernelTestBase;
use Drupal\simpletest\WebTestBase;

class FieldMatcherTest extends WebTestBase {

  /**
   * {@inheritdoc}
   */
  public static $modules = array(
    'entity',
    'user',
    'field',
    'text',
    'crm_core_contact',
    'crm_core_default_matching_engine',
  );

  /**
   * The mocked match field plugin manager.
   *
   * @var \Drupal\crm_core_default_matching_engine\Plugin\MatchFieldPluginManager
   */
  protected $pluginManager;

  /**
   * {@inheritdoc}
   */
  public static function getInfo() {
    return array(
      'name' => 'Default Engine field matcher',
      'description' => 'Tests the field matcher of the default matching engine.',
      'group' => 'CRM Core',
    );
  }

  /**
   * {@inheritdoc}
   */
  protected function setUp() {
    parent::setUp();

    $this->pluginManager = $this->container->get('plugin.manager.crm_core_match.match_field');
  }

  /**
   * Test the unsupported field.
   */
  public function testUnsupported() {
    $config = array(
      'value' => array(
        'operator' => 'equals',
      ),
    );
    $contact_needle = Contact::create(array('type' => 'individual'));
    $contact_needle->save();

    $config['field'] = $contact_needle->getFieldDefinition('uuid');
    /* @var \Drupal\crm_core_default_matching_engine\Plugin\crm_core\MatchField\MatchFieldInterface $unsupported */
    $unsupported = $this->pluginManager->createInstance('unsupported', $config);

    $ids = $unsupported->match($contact_needle);
    $this->assertTrue(empty($ids), 'Empty result for unsupported match');
  }

  /**
   * Test the unsupported field.
   */
  public function testText() {
    $config = array(
      'value' => array(
        'operator' => 'equals',
        'score' => 42,
      ),
    );
    $contact_needle = Contact::create(array('type' => 'individual'));
    $contact_needle->set('contact_name', 'Boomer');
    $contact_needle->save();
    $contact_match = Contact::create(array('type' => 'individual'));
    $contact_match->set('contact_name', 'Boomer');
    $contact_match->save();

    $config['field'] = $contact_needle->getFieldDefinition('contact_name');
    /* @var \Drupal\crm_core_default_matching_engine\Plugin\crm_core\MatchField\MatchFieldInterface $text */
    $text = $this->pluginManager->createInstance('text', $config);

    $ids = $text->match($contact_needle);
    $this->assertTrue(array_key_exists($contact_match->id(), $ids), 'Text match returns expected match');
    $this->assertEqual(42, $ids[$contact_match->id()]['contact_name.value'], 'Got expected match score');
  }
}

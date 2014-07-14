<?php
/**
 * @file
 * Contains \Drupal\crm_core_default_matching_engine\Tests\DefaultEngineTest.
 */

namespace Drupal\crm_core_default_matching_engine\Tests;

use Drupal\crm_core_default_matching_engine\Plugin\crm_core_match\engine\DefaultMatchingEngine;
use Drupal\Tests\UnitTestCase;

/**
 * Tests the default matching engine.
 *
 * @group crm_core
 */
class DefaultEngineTest extends UnitTestCase {

  /**
   * The mocked match field plugin manager.
   *
   * @var \Drupal\crm_core_default_matching_engine\Plugin\FieldHandlerPluginManager|\PHPUnit_Framework_MockObject_MockObject
   */
  protected $pluginManager;


  /**
   * The mocked entity manager service.
   *
   * @var \Drupal\Core\Entity\EntityManagerInterface|\PHPUnit_Framework_MockObject_MockObject
   */
  protected $entityManager;

  /**
   * A mocked contact entity used to get matches.
   *
   * @var \Drupal\crm_core_contact\ContactInterface|\PHPUnit_Framework_MockObject_MockObject
   */
  protected $contact;

  /**
   * @var \Drupal\crm_core_default_matching_engine\Entity\MatchingRule|\PHPUnit_Framework_MockObject_MockObject
   */
  protected $matchingRule;

  /**
   * The tested engine.
   *
   * @var \Drupal\crm_core_default_matching_engine\Plugin\crm_core_match\engine\DefaultMatchingEngine
   */
  protected $engine;

  /**
   * A mocked field definition.
   *
   * @var \Drupal\Core\Field\FieldDefinitionInterface
   */
  protected $field;

  /**
   * A mocked match field handler.
   *
   * @var \Drupal\crm_core_default_matching_engine\Plugin\crm_core_match\field\FieldHandlerInterface
   */
  protected $matchHandler;

  /**
   * {@inheritdoc}
   */
  protected function setUp() {
    $this->pluginManager = $this->getMockBuilder('Drupal\crm_core_default_matching_engine\Plugin\FieldHandlerPluginManager')
      ->disableOriginalConstructor()
      ->getMock();

    $this->entityManager = $this->getMockBuilder('\Drupal\Core\Entity\EntityManagerInterface')
      ->disableOriginalConstructor()
      ->getMock();

    $this->contact = $this->getMockBuilder('Drupal\crm_core_contact\Entity\Contact')
      ->disableOriginalConstructor()
      ->getMock();
    $this->contact->expects($this->any())
      ->method('bundle')
      ->will($this->returnValue('dogs'));

    $this->matchingRule = $this->getMockBuilder('Drupal\crm_core_default_matching_engine\Entity\MatchingRule')
      ->disableOriginalConstructor()
      ->getMock();
    $this->matchingRule->status = TRUE;
    $this->matchingRule->rules = array(
      'foo' => array(),
    );

    $storage = $this->getMock('Drupal\Core\Entity\EntityStorageInterface');
    $storage->expects($this->any())
      ->method('load')
      ->with('dogs')
      ->will($this->returnValue($this->matchingRule));

    $this->entityManager->expects($this->any())
      ->method('getStorage')
      ->will($this->returnValue($storage));

    $this->field = $this->getMock('Drupal\Core\Field\FieldDefinitionInterface');

    $this->matchHandler = $this->getMock('Drupal\crm_core_default_matching_engine\Plugin\crm_core_match\field\FieldHandlerInterface');
    $this->matchHandler->expects($this->any())
      ->method('getPropertyNames')
      ->will($this->returnValue(array('value')));

    $this->engine = new DefaultMatchingEngine(array(), 'default', array(), $this->pluginManager, $this->entityManager);
  }

  /**
   * Tests the match method.
   */
  public function testMatch() {
    $this->field->expects($this->any())
      ->method('getType')
      ->will($this->returnValue('crap'));

    $this->contact->expects($this->any())
      ->method('getFieldDefinitions')
      ->will($this->returnValue(array(
        'foo' => $this->field,
      )));

    $this->pluginManager->expects($this->any())
      ->method('hasDefinition')
      ->will($this->returnValue(TRUE));

    $this->matchHandler->expects($this->any())
      ->method('match')
      ->will($this->returnValue(array(
        '42' => array(
          'value' => 100,
        ),
      )));

    $this->pluginManager->expects($this->once())
      ->method('createInstance')
      ->will($this->returnValue($this->matchHandler));

    $ids = $this->engine->match($this->contact);
    $this->assertEquals(array(42), $ids);
  }
}

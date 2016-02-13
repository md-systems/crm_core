<?php
/**
 * @file
 * Contains \Drupal\Tests\crm_core_match\Unit\DefaultEngineTest.
 */

namespace Drupal\Tests\crm_core_match\Unit;

use Drupal\crm_core_match\Plugin\crm_core_match\engine\DefaultMatchingEngine;
use Drupal\Tests\UnitTestCase;

/**
 * Tests the default matching engine.
 *
 * @coversDefaultClass \Drupal\crm_core_match\Plugin\crm_core_match\engine\DefaultMatchingEngine
 *
 * @group crm_core
 */
class DefaultEngineTest extends UnitTestCase {

  /**
   * The mocked match field plugin manager.
   *
   * @var \Drupal\Core\Entity\EntityManagerInterface|\PHPUnit_Framework_MockObject_MockObject
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
   * @var \Drupal\crm_core_contact\Entity\Contact|\PHPUnit_Framework_MockObject_MockObject
   */
  protected $contact;

  /**
   * A mocked matcher.
   *
   * @var \Drupal\crm_core_match\Entity\Matcher|\PHPUnit_Framework_MockObject_MockObject
   */
  protected $matcher;

  /**
   * The tested engine.
   *
   * @var \Drupal\crm_core_match\Plugin\crm_core_match\engine\DefaultMatchingEngine
   */
  protected $engine;

  /**
   * A mocked field definition.
   *
   * @var \Drupal\Core\Field\FieldDefinitionInterface|\PHPUnit_Framework_MockObject_MockObject
   */
  protected $field;

  /**
   * A mocked match field handler.
   *
   * @var \Drupal\crm_core_match\Plugin\crm_core_match\field\FieldHandlerInterface|\PHPUnit_Framework_MockObject_MockObject
   */
  protected $matchHandler;

  /**
   * {@inheritdoc}
   */
  protected function setUp() {
    $this->pluginManager = $this->getMock('\Drupal\Core\Entity\EntityManagerInterface');

    $this->entityManager = $this->getMockBuilder('\Drupal\Core\Entity\EntityManagerInterface')
      ->disableOriginalConstructor()
      ->getMock();

    $this->contact = $this->getMockBuilder('Drupal\crm_core_contact\Entity\Contact')
      ->disableOriginalConstructor()
      ->getMock();
    $this->contact->expects($this->any())
      ->method('bundle')
      ->will($this->returnValue('dogs'));

    $this->matcher = $this->getMockBuilder('Drupal\crm_core_match\Entity\Matcher')
      ->disableOriginalConstructor()
      ->getMock();
    $this->matcher->expects($this->any())
      ->method('status')
      ->will($this->returnValue(TRUE));

    $storage = $this->getMock('Drupal\Core\Entity\EntityStorageInterface');
    $storage->expects($this->any())
      ->method('load')
      ->with('dogs')
      ->will($this->returnValue($this->matcher));

    $this->entityManager->expects($this->any())
      ->method('getStorage')
      ->will($this->returnValue($storage));

    $this->field = $this->getMock('Drupal\Core\Field\FieldDefinitionInterface');

    $this->matchHandler = $this->getMock('Drupal\crm_core_match\Plugin\crm_core_match\field\FieldHandlerInterface');
    $this->matchHandler->expects($this->any())
      ->method('getPropertyNames')
      ->will($this->returnValue(array('value')));

    $this->engine = new DefaultMatchingEngine([
      'rules' => ['foo' => [], 'bar' => []],
      'threshold' => 50,
    ], 'default', array(), $this->pluginManager, $this->entityManager);
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
    $this->assertEquals([42], $ids);
  }

  /**
   * Tests the match method with multiple fields.
   */
  public function testMultipleMatch() {
    $this->field->expects($this->any())
      ->method('getType')
      ->will($this->returnValue('crap'));

    $this->contact->expects($this->any())
      ->method('getFieldDefinitions')
      ->will($this->returnValue(array(
        'foo' => $this->field,
        'bar' => $this->field,
      )));

    $this->pluginManager->expects($this->any())
      ->method('hasDefinition')
      ->will($this->returnValue(TRUE));

    $this->matchHandler->expects($this->any())
      ->method('match')
      ->will($this->returnValue(array(
        '42' => array(
          'value' => 40,
        ),
      )))
      ->will($this->returnValue(array(
        '42' => array(
          'value' => 40,
        ),
        '30' => array(
          'value' => 40,
        ),
      )));

    $this->pluginManager->expects($this->any())
      ->method('createInstance')
      ->will($this->returnValue($this->matchHandler));

    $ids = $this->engine->match($this->contact);
    $this->assertEquals([42], $ids);
  }

}

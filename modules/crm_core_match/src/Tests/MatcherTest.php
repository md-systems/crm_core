<?php

/**
 * @file
 * Contains \Drupal\crm_core_match\Tests\MatcherTest.
 */

namespace Drupal\crm_core_match\Tests;

use Drupal\crm_core_match\Matcher;
use Drupal\Tests\UnitTestCase;

/**
 * Tests the matcher.
 */
class MatcherTest extends UnitTestCase {

  /**
   * The tested matcher.
   *
   * @var \Drupal\crm_core_match\Matcher
   */
  protected $matcher;
  /**
   * A set mocked match engines keyed by id.
   *
   * @var \Drupal\crm_core_match\MatchEngineInterface[]|\PHPUnit_Framework_MockObject_MockObject[]
   */
  protected $engine = array();

  /**
   * A contact entity used to get matches.
   *
   * @var \Drupal\crm_core_contact\Entity\Contact
   */
  protected $contact;

  /**
   * A mocked instance of the engine plugin manager.
   *
   * @var \Drupal\crm_core_match\Plugin\MatchEnginePluginManager|\PHPUnit_Framework_MockObject_MockObject
   */
  protected $pluginManager;

  /**
   * A mocked instance of the config.
   *
   * @var \Drupal\Core\Config\Config|\PHPUnit_Framework_MockObject_MockObject
   */
  protected $config;

  /**
   * {@inheritdoc}
   */
  public static function getInfo() {
    return array(
      'name' => 'Matcher',
      'description' => 'Tests the matcher.',
      'group' => 'CRM Core',
    );
  }

  /**
   * {@inheritdoc}
   */
  protected function setUp() {
    $this->engine['a'] = $this->getMock('Drupal\crm_core_match\Plugin\crm_core_match\engine\MatchEngineInterface');
    $this->engine['b'] = $this->getMock('Drupal\crm_core_match\Plugin\crm_core_match\engine\MatchEngineInterface');
    $this->engine['c'] = $this->getMock('Drupal\crm_core_match\Plugin\crm_core_match\engine\MatchEngineInterface');

    $this->pluginManager = $this->getMockBuilder('Drupal\crm_core_match\Plugin\MatchEnginePluginManager')
      ->disableOriginalConstructor()
      ->getMock();

    $this->config = $this->getMockBuilder('\Drupal\Core\Config\Config')
      ->disableOriginalConstructor()
      ->getMock();

    $this->matcher = new Matcher($this->pluginManager, $this->config);

    $this->contact = $this->getMockBuilder('Drupal\crm_core_contact\Entity\Contact')
      ->disableOriginalConstructor()
      ->getMock();
  }

  /**
   * Tests the sorting of engines.
   */
  public function testEngineSort() {
    $engine_config = $this->getMockBuilder('\Drupal\Core\Config\Config')
      ->disableOriginalConstructor()
      ->getMock();

    $this->config->expects($this->once())
      ->method('get')
      ->with('engines')
      ->will($this->returnValue($engine_config));

    $engine_config->expects($this->exactly(3))
      ->method('get')
      ->will($this->returnValue(TRUE));

    $definitions = array(
      'a' => array('priority' => 5),
      'b' => array('priority' => 11),
      'c' => array('priority' => -1),
    );
    $this->pluginManager->expects($this->once())
      ->method('getDefinitions')
      ->will($this->returnValue($definitions));

    $this->pluginManager->expects($this->at(1))
      ->method('createInstance')
      ->with('a', $definitions['a'])
      ->will($this->returnValue($this->engine['a']));

    $this->pluginManager->expects($this->at(2))
      ->method('createInstance')
      ->with('b', $definitions['b'])
      ->will($this->returnValue($this->engine['b']));

    $this->pluginManager->expects($this->at(3))
      ->method('createInstance')
      ->with('c', $definitions['c'])
      ->will($this->returnValue($this->engine['c']));

    $engines = $this->matcher->getEngines();

    $this->assertTrue(is_array($engines));
    $this->assertTrue(count($engines) == 3);
    $this->assertEquals($this->engine['b'], array_shift($engines));
    $this->assertEquals($this->engine['a'], array_shift($engines));
    $this->assertEquals($this->engine['c'], array_shift($engines));
  }

  /**
   * Tests the execution of match engines.
   */
  public function testEngineExecution() {
    $engine_config = $this->getMockBuilder('\Drupal\Core\Config\Config')
      ->disableOriginalConstructor()
      ->getMock();

    $this->config->expects($this->once())
      ->method('get')
      ->with('engines')
      ->will($this->returnValue($engine_config));

    $engine_config->expects($this->exactly(2))
      ->method('get')
      ->will($this->returnValue(TRUE));

    $definitions = array(
      'a' => array('priority' => 5),
      'b' => array('priority' => 11),
    );
    $this->pluginManager->expects($this->once())
      ->method('getDefinitions')
      ->will($this->returnValue($definitions));

    $this->pluginManager->expects($this->at(1))
      ->method('createInstance')
      ->with('a', $definitions['a'])
      ->will($this->returnValue($this->engine['a']));

    $this->pluginManager->expects($this->at(2))
      ->method('createInstance')
      ->with('b', $definitions['b'])
      ->will($this->returnValue($this->engine['b']));

    $this->engine['a']->expects($this->once())
      ->method('match')
      ->with($this->contact)
      ->will($this->returnValue(array(1, 2, 3, 5, 8, 13)));

    $this->engine['b']->expects($this->once())
      ->method('match')
      ->with($this->contact)
      ->will($this->returnValue(array(3, 8, 21, 34)));

    $ids = $this->matcher->match($this->contact);
    $ids = array_values($ids);
    sort($ids);
    $this->assertEquals(array(1, 2, 3, 5, 8, 13, 21, 34), $ids);
  }

  /**
   * Tests disabled engines are not executed.
   */
  public function testDisabledEngines() {
    $engine_config = $this->getMockBuilder('\Drupal\Core\Config\Config')
      ->disableOriginalConstructor()
      ->getMock();

    $this->config->expects($this->once())
      ->method('get')
      ->with('engines')
      ->will($this->returnValue($engine_config));

    $engine_config->expects($this->exactly(2))
      ->method('get')
      ->will($this->returnValueMap(array(
        array('a.status', TRUE),
        array('b.status', FALSE),
      )));

    $definitions = array(
      'a' => array('priority' => 5),
      'b' => array('priority' => 11),
    );
    $this->pluginManager->expects($this->once())
      ->method('getDefinitions')
      ->will($this->returnValue($definitions));

    $this->pluginManager->expects($this->once())
      ->method('createInstance')
      ->with('a', $definitions['a'])
      ->will($this->returnValue($this->engine['a']));

    $this->engine['a']->expects($this->once())
      ->method('match')
      ->with($this->contact)
      ->will($this->returnValue(array()));

    $this->engine['b']->expects($this->never())
      ->method('match');

    $this->assertEquals(1, count($this->matcher->getEngines()));

    $this->matcher->match($this->contact);
  }
}

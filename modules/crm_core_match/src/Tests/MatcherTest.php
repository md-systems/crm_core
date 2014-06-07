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

    $this->matcher = new Matcher();

    $this->contact = $this->getMockBuilder('Drupal\crm_core_contact\Entity\Contact')
      ->disableOriginalConstructor()
      ->getMock();
  }

  /**
   * Tests the sorting of engines.
   */
  public function testEngineSort() {
    $this->matcher->addMatchEngine('a', $this->engine['a'], 5);
    $this->matcher->addMatchEngine('b', $this->engine['b'], 11);
    $this->matcher->addMatchEngine('c', $this->engine['c'], -1);

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
    $this->matcher->addMatchEngine('a', $this->engine['a']);
    $this->matcher->addMatchEngine('b', $this->engine['b']);

    $this->engine['a']->expects($this->once())
      ->method('match')
      ->with($this->contact)
      ->will($this->returnValue(array(1, 2, 3, 5, 8, 13)));

    $this->engine['b']->expects($this->once())
      ->method('match')
      ->with($this->contact)
      ->will($this->returnValue(array(3, 8, 21, 34)));

    $ids = $this->matcher->match($this->contact);
    $this->assertArrayEquals(array(1, 2, 3, 5, 8, 13, 21, 34), array_values($ids));
  }
}

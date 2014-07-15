<?php
/**
 * @file
 * Contains \Drupal\crm_core_collect_contact\Tests\EventSubscriberTest.
 */

namespace Drupal\crm_core_collect_contact\Tests;

use Drupal\crm_core_collect\CollectEvent;
use Drupal\crm_core_collect_contact\EventSubscriber;
use Drupal\Tests\UnitTestCase;

/**
 * Tests the processing of contact form submissions.
 *
 * @group crm_core
 */
class EventSubscriberTest extends UnitTestCase {

  /**
   * The tested event subscriber.
   *
   * @var \Drupal\crm_core_collect_contact\EventSubscriber
   */
  protected $subscriber;

  /**
   * The entity manager service.
   *
   * @var \Drupal\Core\Entity\EntityManagerInterface|\PHPUnit_Framework_MockObject_MockObject
   */
  protected $entityManager;

  /**
   * The mocked contact storage.
   *
   * @var \Drupal\Core\Entity\EntityStorageInterface|\PHPUnit_Framework_MockObject_MockObject
   */
  protected $contactStorage;

  /**
   * The mocked activity storage.
   *
   * @var \Drupal\Core\Entity\EntityStorageInterface|\PHPUnit_Framework_MockObject_MockObject
   */
  protected $activityStorage;

  /**
   * The mocked logger channel.
   *
   * @var \Drupal\Core\Logger\LoggerChannelInterface|\PHPUnit_Framework_MockObject_MockObject
   */
  protected $logger;

  /**
   * The mocked contact matcher.
   *
   * @var \Drupal\crm_core_match\MatcherInterface|\PHPUnit_Framework_MockObject_MockObject
   */
  protected $matcher;

  /**
   * The mocked data container.
   *
   * @var \Drupal\collect\CollectContainerInterface|\PHPUnit_Framework_MockObject_MockObject
   */
  protected $submission;

  /**
   * Test json data.
   *
   * @var string
   */
  protected $json;

  /**
   * The decoded json test data.
   *
   * @var array
   */
  protected $data;

  /**
   * {@inheritdoc}
   */
  protected function setUp() {
    parent::setUp();

    $this->json = file_get_contents(__DIR__ . '/fixture.json');
    $this->data = json_decode($this->json, TRUE);

    $this->entityManager = $this->getMock('Drupal\Core\Entity\EntityManagerInterface');
    $this->contactStorage = $this->getMock('Drupal\Core\Entity\EntityStorageInterface');
    $this->activityStorage = $this->getMock('Drupal\Core\Entity\EntityStorageInterface');
    $this->logger = $this->getMock('Drupal\Core\Logger\LoggerChannelInterface');
    $this->matcher = $this->getMock('Drupal\crm_core_match\MatcherInterface');
    $this->submission = $this->getMock('Drupal\collect\CollectContainerInterface');

    $this->subscriber = new EventSubscriber($this->entityManager, $this->logger, $this->matcher);

    $this->entityManager->expects($this->any())
      ->method('getStorage')
      ->will($this->returnValueMap(array(
        array('crm_core_contact', $this->contactStorage),
        array('crm_core_activity', $this->activityStorage),
      )));

    $this->submission->expects($this->any())
      ->method('getSchemaUri')
      ->will($this->returnValue('https://drupal.org/project/collect_client/contact'));

    $this->submission->expects($this->any())
      ->method('getType')
      ->will($this->returnValue('application/json'));

  }

  /**
   * Tests the announced event subscription..
   */
  public function testGetSubscribedEvents() {
    $expected_subscription = array(
      'crm_core_collect.process' => 'process',
    );
    $subscription = EventSubscriber::getSubscribedEvents();

    $this->assertArrayEquals($expected_subscription, $subscription, 'Got subscription to event crm_core_collect.process to method process.');
  }

  /**
   * Tests the processing of an unknown mime type.
   */
  public function testProcessUnknownType() {
    $event = new CollectEvent($this->submission);

    $this->submission->expects($this->any())
      ->method('getType')
      ->will($this->returnValue('application/pdf'));

    $this->logger->expects($this->once())
      ->method('notice')
      ->with('Unsupported MIME type {type} when processing submission with scheme {schema}', array(
        'type' => 'application/pdf',
        'schema' => 'https://drupal.org/project/collect_client/contact',
      ));
    $this->subscriber->process($event);
  }

  /**
   * Tests the processing with a user present in the received data.
   */
  public function testProcessWithUser() {
    $event = new CollectEvent($this->submission);
    $contact = $this->getMockBuilder('Drupal\crm_core_contact\Entity\Contact')
      ->disableOriginalConstructor()
      ->getMock();
    $activity = $this->getMockBuilder('Drupal\crm_core_activity\Entity\Activity')
      ->disableOriginalConstructor()
      ->getMock();

    $this->submission->expects($this->any())
      ->method('getData')
      ->will($this->returnValue($this->json));

    $this->contactStorage->expects($this->once())
      ->method('create')
      ->with(array(
        'type' => 'individual',
        'contact_remote_id' => 'http://example.com/user/1',
        'name' => 'dapibus',
        'contact_mail' => array(
          'dapibus@example.com',
          'dapibus@example.com',
        ),
      ))
      ->will($this->returnValue($contact));

    $contact->expects($this->once())
      ->method('save');

    $this->activityStorage->expects($this->once())
      ->method('create')
      ->with(array(
        'type' => 'contact',
        'title' => $this->data['values']['subject'][0]['value'],
        'activity_notes' => $this->data['values']['message'][0]['value'],
        'activity_participants' => $contact,
        'activity_submission' => $this->submission,
      ))
      ->will($this->returnValue($activity));

    $activity->expects($this->once())
      ->method('save');

    $this->subscriber->process($event);
  }

  /**
   * Tests the processing with no user present in the received data.
   */
  public function testProcessWithoutUser() {
    $data = $this->data;
    $data['user'] = NULL;

    $event = new CollectEvent($this->submission);
    $contact = $this->getMockBuilder('Drupal\crm_core_contact\Entity\Contact')
      ->disableOriginalConstructor()
      ->getMock();
    $activity = $this->getMockBuilder('Drupal\crm_core_activity\Entity\Activity')
      ->disableOriginalConstructor()
      ->getMock();

    $this->submission->expects($this->any())
      ->method('getData')
      ->will($this->returnValue(json_encode($data)));

    $this->contactStorage->expects($this->once())
      ->method('create')
      ->with(array(
        'type' => 'individual',
        'name' => 'Ullamcorper Fermentum',
        'contact_mail' => array(
          'ullamcorper@example.com',
        ),
      ))
      ->will($this->returnValue($contact));

    $contact->expects($this->once())
      ->method('save');

    $this->activityStorage->expects($this->once())
      ->method('create')
      ->will($this->returnValue($activity));

    $activity->expects($this->once())
      ->method('save');

    $this->subscriber->process($event);
  }

  /**
   * Tests the processing with a user present in the received data and a match.
   */
  public function testProcessWithUserAndUserMatch() {
    $event = new CollectEvent($this->submission);
    $contact = $this->getMockBuilder('Drupal\crm_core_contact\Entity\Contact')
      ->disableOriginalConstructor()
      ->getMock();
    $contact_match = $this->getMockBuilder('Drupal\crm_core_contact\Entity\Contact')
      ->disableOriginalConstructor()
      ->getMock();
    $activity = $this->getMockBuilder('Drupal\crm_core_activity\Entity\Activity')
      ->disableOriginalConstructor()
      ->getMock();

    $this->submission->expects($this->any())
      ->method('getData')
      ->will($this->returnValue($this->json));

    $this->contactStorage->expects($this->once())
      ->method('create')
      ->with(array(
        'type' => 'individual',
        'contact_remote_id' => 'http://example.com/user/1',
        'name' => 'dapibus',
        'contact_mail' => array(
          'dapibus@example.com',
          'dapibus@example.com',
        ),
      ))
      ->will($this->returnValue($contact));

    $this->matcher->expects($this->once())
      ->method('match')
      ->with($contact)
      ->will($this->returnValue(array(42)));

    $this->contactStorage->expects($this->once())
      ->method('load')
      ->with(42)
      ->will($this->returnValue($contact_match));

    $contact->expects($this->any())
      ->method('get')
      ->will($this->returnValueMap(array(
        array('contact_remote_id', (object) array('value' => 'http://example.com/user/42')),
        array('name', (object) array('value' => 'Amet Dolor')),
        array('contact_mail', (object) array('value' => 'amet@example.com')),
      )));
    $contact_match->expects($this->never())
      ->method('set');

    $contact_match->expects($this->never())
      ->method('save');

    $this->activityStorage->expects($this->once())
      ->method('create')
      ->with(array(
        'type' => 'contact',
        'title' => $this->data['values']['subject'][0]['value'],
        'activity_notes' => $this->data['values']['message'][0]['value'],
        'activity_participants' => $contact_match,
        'activity_submission' => $this->submission,
      ))
      ->will($this->returnValue($activity));

    $activity->expects($this->once())
      ->method('save');

    $this->subscriber->process($event);
  }

  /**
   * Tests the processing with no user present in the received data and a match.
   */
  public function testProcessWithoutUserAndUserMatch() {
    $data = $this->data;
    $data['user'] = NULL;

    $event = new CollectEvent($this->submission);
    $contact = $this->getMockBuilder('Drupal\crm_core_contact\Entity\Contact')
      ->disableOriginalConstructor()
      ->getMock();
    $contact_match = $this->getMockBuilder('Drupal\crm_core_contact\Entity\Contact')
      ->disableOriginalConstructor()
      ->getMock();
    $activity = $this->getMockBuilder('Drupal\crm_core_activity\Entity\Activity')
      ->disableOriginalConstructor()
      ->getMock();

    $this->submission->expects($this->any())
      ->method('getSchemaUri')
      ->will($this->returnValue('https://drupal.org/project/collect_client/contact'));

    $this->submission->expects($this->any())
      ->method('getType')
      ->will($this->returnValue('application/json'));

    $this->submission->expects($this->any())
      ->method('getData')
      ->will($this->returnValue(json_encode($data)));

    $this->contactStorage->expects($this->once())
      ->method('create')
      ->with(array(
        'type' => 'individual',
        'name' => 'Ullamcorper Fermentum',
        'contact_mail' => array(
          'ullamcorper@example.com',
        ),
      ))
      ->will($this->returnValue($contact));

    $this->matcher->expects($this->once())
      ->method('match')
      ->with($contact)
      ->will($this->returnValue(array(42)));

    $this->contactStorage->expects($this->once())
      ->method('load')
      ->with(42)
      ->will($this->returnValue($contact_match));

    $contact->expects($this->any())
      ->method('get')
      ->will($this->returnValueMap(array(
        array('contact_remote_id', (object) array('value' => NULL)),
        array('name', (object) array('value' => 'Amet Dolor')),
        array('contact_mail', (object) array('value' => 'amet@example.com')),
      )));

    $contact_match->expects($this->never())
      ->method('set');

    $contact_match->expects($this->never())
      ->method('save');

    $this->activityStorage->expects($this->once())
      ->method('create')
      ->with(array(
        'type' => 'contact',
        'title' => $this->data['values']['subject'][0]['value'],
        'activity_notes' => $this->data['values']['message'][0]['value'],
        'activity_participants' => $contact_match,
        'activity_submission' => $this->submission,
      ))
      ->will($this->returnValue($activity));

    $activity->expects($this->once())
      ->method('save');

    $this->subscriber->process($event);
  }
}

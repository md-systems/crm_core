<?php
/**
 * @file
 * Contains \Drupal\crm_core_collect_contact\Tests\EventProcessingTest.
 */

namespace Drupal\crm_core_collect_contact\Tests;

use Drupal\collect\CollectContainerInterface;
use Drupal\collect\Entity\Container;
use Drupal\crm_core_activity\Entity\Activity;
use Drupal\crm_core_collect\CollectEvent;
use Drupal\crm_core_contact\Entity\Contact;
use Drupal\crm_core_default_matching_engine\Entity\MatchingRule;
use Drupal\simpletest\KernelTestBase;

/**
 * Tests the processing of contact form submission on integration level.
 *
 * @group crm_core
 */
class EventProcessingTest extends KernelTestBase {

  /**
   * {@inheritdoc}
   */
  public static $modules = array(
    'user',
    'serialization',
    'rest',
    'hal',
    'collect',
    'field',
    'text',
    'datetime',
    'entity',
    'filter',
    'crm_core_contact',
    'crm_core_activity',
    'crm_core_match',
    'crm_core_default_matching_engine',
    'crm_core_collect_contact',
  );

  /**
   * Test json data.
   *
   * @var string
   */
  protected $json;

  /**
   * The submission to process.
   *
   * @var \Drupal\collect\CollectContainerInterface
   */
  protected $submission;

  /**
   * An existing contact.
   *
   * @var \Drupal\crm_core_contact\Entity\Contact
   */
  protected $contact;

  /**
   * {@inheritdoc}
   */
  protected function setUp() {
    parent::setUp();

    $this->installEntitySchema('collect_container');
    $this->installEntitySchema('crm_core_contact');
    $this->installEntitySchema('crm_core_activity');
    $this->installEntitySchema('user');

    $this->installConfig(array(
      'crm_core_contact',
      'crm_core_activity',
      'crm_core_collect_contact',
    ));

    \Drupal::config('crm_core_match.engines')->set('default', array('status' => TRUE));
    /* @var \Drupal\crm_core_default_matching_engine\Entity\MatchingRule $rule */
    $rule = MatchingRule::load('individual');
    $rule->setStatus(TRUE);
    $rule->threshold = 8;
    $rule->rules = array(
      'contact_remote_id' => array(
        'value' => array(
          'status' => '1',
          'operator' => '=',
          'options' => '',
          'score' => '10',
          'weight' => '0',
        ),
      ),
      'contact_mail' => array(
        'value' => array(
          'status' => '1',
          'operator' => '=',
          'options' => '',
          'score' => '9',
          'weight' => '0',
        ),
      ),
    );
    $rule->save();

    $this->json = file_get_contents(__DIR__ . '/fixture.json');
    $this->submission = Container::create(array(
      'origin_uri' => 'http://localhost/entity/message/feedback/2494b3ba-158b-4066-9833-510bd72c82eb',
      'schema_uri' => 'https://drupal.org/project/collect_client/contact',
      'date' => REQUEST_TIME,
      'data' => array(
        'data' => $this->json,
        'type' => 'application/json',
      ),
    ));
    $this->submission->save();

    $this->contact = Contact::create(array(
      'type' => 'individual',
      'name' => 'Aenean Risus',
      'contact_mail' => 'anean@example.com',
    ));
    $this->contact->save();
  }

  /**
   * Tests submission containing a user that does not match an existing contact.
   */
  public function testWithUserWithoutMatch() {
    $this->triggerProcessing($this->submission);

    $contact = Contact::load(2);
    $this->assertNewUserContact($contact);

    $activity = Activity::load(1);
    $this->assertActivity($activity, $contact);
  }

  /**
   * Tests submission containing a user that does match an existing contact.
   */
  public function testWithUserWithMatch() {
    $this->contact->set('contact_remote_id', 'http://example.com/user/1');
    $this->contact->save();

    $this->triggerProcessing($this->submission);

    $contact = Contact::load(2);
    $this->assertNull($contact, 'No new contact was created.');

    $contact = Contact::load(1);
    $this->assertMatchContact($contact);
    $activity = Activity::load(1);
    $this->assertActivity($activity, $contact);
  }

  /**
   * Tests submission not containing a user not matching an existing contact.
   */
  public function testWithoutUserWithoutMatch() {
    $this->unsetUser();

    $this->triggerProcessing($this->submission);

    $contact = Contact::load(2);
    $this->assertNewMessageContact($contact);
    $activity = Activity::load(1);
    $this->assertActivity($activity, $contact);
  }

  /**
   * Tests submission not containing a user that does match an existing contact.
   */
  public function testWithoutUserWithMatch() {
    $this->unsetUser();
    $data = json_decode($this->json, TRUE);
    $data['values']['mail'][0]['value'] = 'anean@example.com';
    $this->json = json_encode($data);
    $this->submission->setData($this->json);

    $this->triggerProcessing($this->submission);

    $contact = Contact::load(2);
    $this->assertNull($contact, 'No new contact was created.');

    $contact = Contact::load(1);
    $this->assertMatchContact($contact);
    $activity = Activity::load(1);
    $this->assertActivity($activity, $contact);
  }

  /**
   * Asserts an activity.
   *
   * An activity was created with values from the submission and linked to the
   * container and contact
   *
   * @param Activity $activity
   *   The activity to assert.
   * @param Contact $contact
   *   The contact expected to be linked with the activity.
   */
  protected function assertActivity(Activity $activity = NULL, Contact $contact = NULL) {
    $this->assertNotNull($activity, 'New activity was created.');
    if ($activity) {
      $this->assertEqual('Aenean lacinia bibendum nulla sed consectetur', $activity->get('title')->value, 'Found expected activity title');
      $data = json_decode($this->json, TRUE);
      $this->assertEqual($data['values']['message'][0]['value'], $activity->get('activity_notes')->value, 'Found expected activity title');
      if ($contact) {
        $this->assertEqual($contact->id(), $activity->get('activity_participants')->target_id, 'Activity was assigned to the expected contact');
      }
      $this->assertEqual($this->submission->id(), $activity->get('activity_submission')->target_id, 'Activity was linked to the container record');
    }
  }

  /**
   * Asserts a new user contact.
   *
   * A new contact was created with values from the user section of the
   * submission.
   *
   * @param Contact $contact
   *   The contact to assert.
   */
  protected function assertNewUserContact(Contact $contact = NULL) {
    $this->assertNotNull($contact, 'New contact was created.');
    if ($contact) {
      $this->assertEqual('dapibus', $contact->get('name')->value, 'Contact is named \'dapibus\'');
      $this->assertEqual('dapibus@example.com', $contact->get('contact_mail')->value, 'Contact mail is \'dapibus@example.com\'');
      $this->assertEqual('http://example.com/user/1', $contact->get('contact_remote_id')->value, 'Remote identifier is \'http://example.com/user/1\'');
    }
  }

  /**
   * Asserts a new message contact.
   *
   * A new contact was created with values from the message part of the
   * submission.
   *
   * @param Contact $contact
   *   The contact to assert.
   */
  protected function assertNewMessageContact(Contact $contact = NULL) {
    $this->assertNotNull($contact, 'New contact was created.');
    if ($contact) {
      $this->assertEqual('Ullamcorper Fermentum', $contact->get('name')->value, 'Contact is named \'Ullamcorper Fermentum\'');
      $this->assertEqual('ullamcorper@example.com', $contact->get('contact_mail')->value, 'Contact mail is \'ullamcorper@example.com\'');
      $this->assertNull($contact->get('contact_remote_id')->value, 'Remote identifier is undefined');
    }
  }

  /**
   * Asserts the value of a contact identified as a match.
   *
   * The existing contact was not overwritten.
   *
   * @param Contact $contact
   *   The contact to assert.
   */
  protected function assertMatchContact(Contact $contact) {
    $this->assertEqual('Aenean Risus', $contact->get('name')->value, 'Contact is named \'Aenean Risus\'');
    $this->assertEqual('anean@example.com', $contact->get('contact_mail')->value, 'Contact mail is \'anean@example.com\'');
  }

  /**
   * Triggers processing.
   *
   * @param \Drupal\collect\CollectContainerInterface $submission
   *   The submission to be processed.
   */
  protected function triggerProcessing(CollectContainerInterface $submission) {
    $event = new CollectEvent($submission);
    /* @var \Symfony\Component\EventDispatcher\EventDispatcherInterface $dispatcher */
    $dispatcher = $this->container->get('event_dispatcher');
    $dispatcher->dispatch(CollectEvent::NAME, $event);
  }

  /**
   * Sets the user to null in the submission data.
   */
  protected function unsetUser() {
    $data = json_decode($this->json, TRUE);
    $data['user'] = NULL;
    $this->json = json_encode($data);
    $this->submission->setData($this->json);
  }
}

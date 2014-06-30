<?php
/**
 * @file
 * Contains \Drupal\crm_core_collect_contact\EventSubscriber.
 */

namespace Drupal\crm_core_collect_contact;

use Drupal\Core\Entity\EntityManagerInterface;
use Drupal\Core\Logger\LoggerChannelInterface;
use Drupal\crm_core_collect\CollectEvent;
use Drupal\crm_core_match\MatcherInterface;
use Symfony\Component\EventDispatcher\EventSubscriberInterface;

class EventSubscriber implements EventSubscriberInterface {

  /**
   * The schema uri for contact submissions.
   */
  const SCHEMA_URI = 'https://drupal.org/project/collect_client/contact';

  /**
   * The entity manager service.
   *
   * @var \Drupal\Core\Entity\EntityManagerInterface
   */
  protected $entityManager;

  /**
   * The logger channel.
   *
   * @var \Drupal\Core\Logger\LoggerChannelInterface
   */
  protected $logger;

  /**
   * The contact matcher.
   *
   * @var \Drupal\crm_core_match\MatcherInterface
   */
  protected $matcher;

  /**
   * Constructs an EventSubscriberObject.
   *
   * @param \Drupal\Core\Entity\EntityManagerInterface $entity_manager
   *   The entity manager.
   * @param \Drupal\Core\Logger\LoggerChannelInterface $logger
   *   The logger chanel.
   * @param \Drupal\crm_core_match\MatcherInterface $matcher
   *   The contact matcher.
   */
  public function __construct(EntityManagerInterface $entity_manager, LoggerChannelInterface $logger, MatcherInterface $matcher) {
    $this->entityManager = $entity_manager;
    $this->logger = $logger;
    $this->matcher = $matcher;
  }

  /**
   * Processes a CollectEvent.
   *
   * @param \Drupal\crm_core_collect\CollectEvent $event
   *   The collect event containing the collected data.
   */
  public function process(CollectEvent $event) {
    $submission = $event->getSubmission();
    if ($submission->getSchemaUri() == self::SCHEMA_URI) {

      switch ($submission->getType()) {
        case 'application/json':
          $data = json_decode($submission->getData(), TRUE);

          $contact = $this->getContact($data);
          $matches = $this->matcher->match($contact);
          if (empty($matches)) {
            $contact->save();
          }
          else {
            // The first match is the one with the highest score.
            $match = $this->entityManager->getStorage('crm_core_contact')->load($matches[0]);
            $contact = $match;
            // @todo Queue multiple matches for manual moderation.
          }

          $activity = $this->entityManager->getStorage('crm_core_activity')->create(array(
            'type' => 'contact',
            'title' => $data['values']['subject'][0]['value'],
            'activity_notes' => $data['values']['message'][0]['value'],
            'activity_participants' => $contact,
            'activity_submission' => $submission,
          ));
          $activity->save();

          break;

        default:
          $this->logger->notice('Unsupported MIME type {type} when processing submission with scheme {schema}', array(
            'type' => $submission->getType(),
            'schema' => $submission->getSchemaUri(),
          ));
      }
    }
  }

  /**
   * Gets a contact from the submission data.
   *
   * Takes the values from the user if present and from the message otherwise.
   *
   * @param array $data
   *   The submission data.
   *
   * @return \Drupal\crm_core_contact\Entity\Contact
   *   The extracted contact entity.
   */
  protected function getContact(array $data) {
    $contact = array(
      'type' => 'individual',
    );

    if (empty($data['user'])) {
      $contact['contact_mail'][] = $data['values']['mail'][0]['value'];
      $contact['contact_name'] = $data['values']['name'][0]['value'];
    }
    else {
      $contact['contact_mail'][] = $data['user']['mail'][0]['value'];
      $contact['contact_mail'][] = $data['user']['init'][0]['value'];
      $contact['contact_name'] = $data['user']['name'][0]['value'];
      $contact['contact_remote_id'] = $data['user']['_links']['self']['href'];
    }

    return $this->entityManager->getStorage('crm_core_contact')->create($contact);

  }

  /**
   * {@inheritdoc}
   */
  public static function getSubscribedEvents() {
    return array(CollectEvent::NAME => 'process');
  }
}

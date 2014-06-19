<?php
/**
 * @file
 * Contains \Drupal\crm_core_collect\CollectEvent.
 */

namespace Drupal\crm_core_collect;

use Symfony\Component\EventDispatcher\Event;

class CollectEvent extends Event {

  /**
   * The queue name.
   *
   * The name of the queue used for processing submissions.
   */
  const QUEUE = 'crm_core_collect';

  /**
   * The processing event name.
   */
  const NAME = 'crm_core_collect.process';

  /**
   * @var \Drupal\collect\CollectContainerInterface
   */
  protected $submission;

  /**
   * Constructs a CollectEvent object.
   *
   * @param \Drupal\collect\CollectContainerInterface $submission
   *   The submission to process.
   */
  public function __construct($submission) {
    $this->submission = $submission;
  }

  /**
   * Get the submission to process.
   *
   * @return \Drupal\collect\CollectContainerInterface
   *   The collect container entity containing the submission data.
   */
  public function getSubmission() {
    return $this->submission;
  }
}

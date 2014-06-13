<?php

/**
 * @file
 * Contains \Drupal\crm_core_submission\Subscriber.
 *
 * @todo Move to a multipart module similar to hal.
 */

namespace Drupal\crm_core_submission;

use Symfony\Component\HttpKernel\KernelEvents;
use Symfony\Component\HttpKernel\Event\GetResponseEvent;
use Symfony\Component\EventDispatcher\EventSubscriberInterface;

/**
 * Subscribes to the kernel request event to add multipart media types.
 */
class Subscriber implements EventSubscriberInterface {

  /**
   * Registers multipart formats with the Request class.
   *
   * @param \Symfony\Component\HttpKernel\Event\GetResponseEvent $event
   *   The event to process.
   */
  public function onKernelRequest(GetResponseEvent $event) {
    $request = $event->getRequest();
    $request->setFormat('multipart_mixed', 'multipart/mixed');
  }

  /**
   * Registers the methods in this class that should be listeners.
   *
   * @return array
   *   An array of event listener definitions.
   */
  public static function getSubscribedEvents() {
    $events[KernelEvents::REQUEST][] = array('onKernelRequest', 40);
    return $events;
  }

}

<?php
/**
 * @file
 * Contains \Drupal\crm_core_collect\Plugin\rest\resource\CollectResource.
 */

namespace Drupal\crm_core_collect\Plugin\rest\resource;

use Drupal\collect\CollectContainerInterface;
use Drupal\Core\Entity\EntityManagerInterface;
use Drupal\Core\Queue\QueueInterface;
use Drupal\Core\Routing\UrlGeneratorInterface;
use Drupal\crm_core_collect\CollectEvent;
use Drupal\rest\Plugin\ResourceBase;
use Drupal\rest\ResourceResponse;
use Symfony\Cmf\Component\Routing\RouteObjectInterface;
use Symfony\Component\DependencyInjection\ContainerInterface;
use Symfony\Component\HttpFoundation\Request;
use Symfony\Component\HttpFoundation\Response;
use Symfony\Component\Serializer\Normalizer\NormalizerInterface;

/**
 * Provides a resource for collecting crm submissions.
 *
 * @RestResource(
 *   id = "crm_core_collect",
 *   label = @Translation("CRM Core Collect"),
 *   serialization_class = "Drupal\collect\Entity\Container",
 *   uri_paths = {
 *     "canonical" = "/crm-core/api/v1/submissions/{uuid}",
 *     "http://drupal.org/link-relations/create" = "/crm-core/api/v1/submissions"
 *   }
 * )
 */
class CollectResource extends ResourceBase {


  /**
   * The serializer service.
   *
   * @var \Symfony\Component\Serializer\Serializer
   */
  protected $serializer;

  /**
   * The url generator service.
   *
   * @var \Drupal\Core\Routing\UrlGeneratorInterface
   */
  protected $urlGenerator;

  /**
   * The entity manager service.
   *
   * @var \Drupal\Core\Entity\EntityManagerInterface
   */
  protected $entityManager;

  /**
   * The submission processing queue.
   *
   * @var \Drupal\Core\Queue\QueueInterface
   */
  protected $queue;

  /**
   * Constructs a Drupal\rest\Plugin\ResourceBase object.
   *
   * @param \Symfony\Component\Serializer\Normalizer\NormalizerInterface $serializer
   *   The serializer service.
   * @param \Drupal\Core\Routing\UrlGeneratorInterface $url_generator
   *   The url generator service.
   * @param \Drupal\Core\Entity\EntityManagerInterface $entity_manager
   *   The entity manager service.
   * @param \Drupal\Core\Queue\QueueInterface $queue
   *   The queue for processing submissions.
   * @param array $configuration
   *   A configuration array containing information about the plugin instance.
   * @param string $plugin_id
   *   The plugin_id for the plugin instance.
   * @param mixed $plugin_definition
   *   The plugin implementation definition.
   * @param array $serializer_formats
   *   The available serialization formats.
   */
  public function __construct(NormalizerInterface $serializer, UrlGeneratorInterface $url_generator, EntityManagerInterface $entity_manager, QueueInterface $queue, array $configuration, $plugin_id, $plugin_definition, array $serializer_formats) {
    parent::__construct($configuration, $plugin_id, $plugin_definition, $serializer_formats);

    $this->serializer = $serializer;
    $this->urlGenerator = $url_generator;
    $this->entityManager = $entity_manager;
    $this->queue = $queue;
  }

  /**
   * {@inheritdoc}
   */
  public static function create(ContainerInterface $container, array $configuration, $plugin_id, $plugin_definition) {
    return new static(
      $container->get('serializer'),
      $container->get('url_generator'),
      $container->get('entity.manager'),
      $container->get('queue')->get(CollectEvent::QUEUE),
      $configuration,
      $plugin_id,
      $plugin_definition,
      $container->getParameter('serializer.formats')
    );
  }

  /**
   * Gets a submission object.
   *
   * @param string $uuid
   *   The uuid of the object to return.
   * @param CollectContainerInterface $submission
   *   Just an empty placeholder.
   *   The RequestHandler passes the unserialized value as second argument. GET
   *   request usually doe not contain any request body, so I most probably will
   *   be empty.
   * @param Request $request
   *   The request object.
   *
   * @return ResourceResponse
   *   The resource response.
   */
  public function get($uuid, CollectContainerInterface $submission = NULL, Request $request = NULL) {

    $submission = $this->entityManager->loadEntityByUuid('collect_container', $uuid);

    // Rewire the the self link to this resource if the format is hal_json.
    //
    // The ContentEntityNormalizer uses the canonical url to build the self
    // link. The collect container entity does not have a canonical link and or
    // it would point to somewhere else.
    $format = $request->attributes->get(RouteObjectInterface::ROUTE_OBJECT)->getRequirement('_format') ?: 'json';
    $content = $this->serializer->normalize($submission, $format);
    if ($format == 'hal_json') {
      $content['_links']['self']['href'] = $this->getCanonicalUrl($submission);
    }
    return new ResourceResponse($content);
  }

  /**
   * Responds to submission POST requests and echos the data.
   *
   * @param \Drupal\collect\CollectContainerInterface $submission
   *   The submission..
   *
   * @return \Drupal\rest\ResourceResponse
   *   The HTTP response object.
   */
  public function post(CollectContainerInterface $submission) {
    $submission->save();
    $this->queue->createItem($submission);
    return new ResourceResponse('', Response::HTTP_CREATED, array(
      'Location' => $this->getCanonicalUrl($submission),
    ));
  }

  /**
   * Gets the canonical service url for a submission.
   *
   * @param CollectContainerInterface $submission
   *   The submission object.
   *
   * @return string
   *   The canonical service url.
   *
   * @todo Make this work if the route for the json format is missing.
   */
  protected function getCanonicalUrl(CollectContainerInterface $submission) {
    return $this->urlGenerator->generate('rest.crm_core_collect.GET.json', array(
      'uuid' => $submission->uuid(),
    ), TRUE);
  }
}

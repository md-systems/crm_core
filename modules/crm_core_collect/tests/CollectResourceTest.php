<?php
/**
 * @file
 * Contains \Drupal\crm_core_collect\Tests\CollectResourceTest.
 */

namespace Drupal\crm_core_collect\Tests;

use Drupal\crm_core_collect\Plugin\rest\resource\CollectResource;
use Drupal\Tests\UnitTestCase;
use Symfony\Cmf\Component\Routing\RouteObjectInterface;
use Symfony\Component\DependencyInjection\ParameterBag\ParameterBag;

/**
 * Tests the CRM Core Collect REST resource.
 *
 * @group crm_core_collect
 */
class CollectResourceTest extends UnitTestCase {

  /**
   * The tested resource.
   *
   * @var \Drupal\crm_core_collect\Plugin\rest\resource\CollectResource
   */
  protected $resource;

  /**
   * The mocked serializer service.
   *
   * @var \Symfony\Component\Serializer\Normalizer\NormalizerInterface|\PHPUnit_Framework_MockObject_MockObject
   */
  protected $serializer;

  /**
   * The mocked url generator service.
   *
   * @var \Drupal\Core\Routing\UrlGeneratorInterface|\PHPUnit_Framework_MockObject_MockObject
   */
  protected $urlGenerator;

  /**
   * The mocked entity manager service.
   *
   * @var \Drupal\Core\Entity\EntityManagerInterface|\PHPUnit_Framework_MockObject_MockObject
   */
  protected $entityManager;

  /**
   * The mocked queue for processing submissions.
   *
   * @var \Drupal\Core\Queue\QueueInterface|\PHPUnit_Framework_MockObject_MockObject
   */
  protected $queue;

  /**
   * {@inheritdoc}
   */
  protected function setUp() {
    parent::setUp();

    $this->serializer = $this->getMock('Symfony\Component\Serializer\Normalizer\NormalizerInterface');
    $this->urlGenerator = $this->getMock('Drupal\Core\Routing\UrlGeneratorInterface');
    $this->entityManager = $this->getMock('Drupal\Core\Entity\EntityManagerInterface');
    $this->queue = $this->getMock('Drupal\Core\Queue\QueueInterface');

    $this->resource = new CollectResource($this->serializer, $this->urlGenerator, $this->entityManager, $this->queue, array(), 'crm_core_collect', array(), array());
  }

  /**
   * Tests GET of a exiting submission.
   */
  public function testGet() {
    $uuid = 'd18cf563-f20f-4e3c-ad7e-01818f1baeac';
    $url = 'http://example.com/crm-core/api/v1/submissions/d18cf563-f20f-4e3c-ad7e-01818f1baeac';
    $request = $this->getMock('Symfony\Component\HttpFoundation\Request');
    $route = $this->getMockBuilder('Symfony\Component\Routing\Route')
      ->disableOriginalConstructor()
      ->getMock();
    $request->attributes = new ParameterBag(array(
      RouteObjectInterface::ROUTE_OBJECT => $route,
    ));
    $container = $this->getMockBuilder('Drupal\collect\Entity\Container')
      ->disableOriginalConstructor()
      ->getMock();

    $this->entityManager->expects($this->once())
      ->method('loadEntityByUuid')
      ->with('collect_container', $uuid)
      ->will($this->returnValue($container));

    $route->expects($this->any())
      ->method('getRequirement')
      ->with('_format')
      ->will($this->returnValue('hal_json'));

    $this->serializer->expects($this->once())
      ->method('normalize')
      ->with($container, 'hal_json')
      ->will($this->returnValue(array()));

    $this->urlGenerator->expects($this->once())
      ->method('generate')
      ->will($this->returnValue($url));

    $result = $this->resource->get($uuid, NULL, $request);
    $data = $result->getResponseData();
    $this->assertEquals($data['_links']['self']['href'], $url);
  }

  /**
   * Tests the submission of new records.
   */
  public function testPost() {
    $container = $this->getMockBuilder('Drupal\collect\Entity\Container')
      ->disableOriginalConstructor()
      ->getMock();
    $location = 'http://example.com/crm-core/api/v1/submissions/d18cf563-f20f-4e3c-ad7e-01818f1baeac';

    $container->expects($this->once())
      ->method('save');

    $this->queue->expects($this->once())
      ->method('createItem')
      ->with($container);

    $this->urlGenerator->expects($this->once())
      ->method('generate')
      ->will($this->returnValue($location));

    $result = $this->resource->post($container);
    $this->assertEquals($result->getStatusCode(), 201);
    $this->assertEquals($result->headers->get('Location'), $location);
  }
}

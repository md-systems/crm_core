<?php

/**
 * @file
 * Contains \Drupal\crm_core_activity\ActivityListBuilder.
 */

namespace Drupal\crm_core_activity;

use Drupal\Core\Datetime\Date;
use Drupal\Core\Entity\EntityInterface;
use Drupal\Core\Entity\EntityListBuilder;
use Drupal\Core\Entity\EntityStorageInterface;
use Drupal\Core\Entity\EntityTypeInterface;
use Symfony\Component\DependencyInjection\ContainerInterface;

class ActivityListBuilder extends EntityListBuilder {

  /**
   * The date service.
   *
   * @var \Drupal\Core\Datetime\Date
   */
  protected $dateService;

  /**
   * Constructs a new NodeListBuilder object.
   *
   * @param \Drupal\Core\Entity\EntityTypeInterface $entity_type
   *   The entity type definition.
   * @param \Drupal\Core\Entity\EntityStorageInterface $storage
   *   The entity storage class.
   * @param \Drupal\Core\Datetime\Date $date_service
   *   The date service.
   */
  public function __construct(EntityTypeInterface $entity_type, EntityStorageInterface $storage, Date $date_service) {
    parent::__construct($entity_type, $storage);

    $this->dateService = $date_service;
  }

  /**
   * {@inheritdoc}
   */
  public static function createInstance(ContainerInterface $container, EntityTypeInterface $entity_type) {
    return new static(
      $entity_type,
      $container->get('entity.manager')->getStorage($entity_type->id()),
      $container->get('date')
    );
  }

  /**
   * {@inheritdoc}
   */
  public function buildHeader() {
    $header = array();

    $header['date'] = $this->t('Activity Date');
    $header['title'] = $this->t('Title');
    $header['type'] = $this->t('Activity type');

    return $header + parent::buildHeader();
  }

  /**
   * {@inheritdoc}
   */
  public function buildRow(EntityInterface $entity) {
    $row = array();

    $row['date']['data'] = $entity->get('activity_date')->view(array(
      'label' => 'hidden',
    ));

    $row['title']['data'] = array(
      '#type' => 'link',
      '#title' => $entity->label(),
    ) + $entity->urlInfo()->toRenderArray();

    $row['type'] = $entity->get('type')->entity->label();

    return $row + parent::buildRow($entity);
  }

  /**
   * {@inheritdoc}
   */
  public function render() {
    $build = parent::render();

    $build['#empty'] = $this->t('There are no activities available.');

    return $build;
  }
}

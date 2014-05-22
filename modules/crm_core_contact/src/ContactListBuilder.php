<?php

/**
 * @file
 * Contains \Drupal\crm_core_contact\ContactListBuilder.
 */

namespace Drupal\crm_core_contact;

use Drupal\Component\Utility\String;
use Drupal\Core\Datetime\Date;
use Drupal\Core\Entity\EntityInterface;
use Drupal\Core\Entity\EntityListBuilder;
use Drupal\Core\Entity\EntityStorageInterface;
use Drupal\Core\Entity\EntityTypeInterface;
use Symfony\Component\DependencyInjection\ContainerInterface;

class ContactListBuilder extends EntityListBuilder {


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

    $header['label'] = $this->t('Label');

    $header['type'] = array(
      'data' => $this->t('Contact type'),
      'class' => array(RESPONSIVE_PRIORITY_MEDIUM),
    );

    $header['changed'] = array(
      'data' => $this->t('Updated'),
      'class' => array(RESPONSIVE_PRIORITY_LOW),
    );

    return $header + parent::buildHeader();
  }

  /**
   * {@inheritdoc}
   */
  public function buildRow(EntityInterface $entity) {
    $row = array();

    $row['title']['data'] = array(
      '#type' => 'link',
      '#title' => $entity->label(),
    ) + $entity->urlInfo()->toRenderArray();

    $row['type'] = String::checkPlain($entity->get('type')->entity->label());

    $row['changed'] = $this->dateService->format($entity->get('changed')->value, 'short');

    return $row + parent::buildRow($entity);
  }
}

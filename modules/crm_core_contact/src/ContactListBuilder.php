<?php

/**
 * @file
 * Contains \Drupal\crm_core_contact\ContactListBuilder.
 */

namespace Drupal\crm_core_contact;

use Drupal\Component\Utility\SafeMarkup;
use Drupal\Core\Datetime\Date;
use Drupal\Core\Datetime\DateFormatter;
use Drupal\Core\Entity\EntityInterface;
use Drupal\Core\Entity\EntityListBuilder;
use Drupal\Core\Entity\EntityStorageInterface;
use Drupal\Core\Entity\EntityTypeInterface;
use Symfony\Component\DependencyInjection\ContainerInterface;

class ContactListBuilder extends EntityListBuilder {


  /**
   * The date formatter service.
   *
   * @var \Drupal\Core\Datetime\DateFormatter
   */
  protected $dateFormatter;

  /**
   * Constructs a new NodeListBuilder object.
   *
   * @param \Drupal\Core\Entity\EntityTypeInterface $entity_type
   *   The entity type definition.
   * @param \Drupal\Core\Entity\EntityStorageInterface $storage
   *   The entity storage class.
   * @param \Drupal\Core\Datetime\DateFormatter $date_formatter
   *   The date formatter service.
   */
  public function __construct(EntityTypeInterface $entity_type, EntityStorageInterface $storage, DateFormatter $date_formatter) {
    parent::__construct($entity_type, $storage);

    $this->dateFormatter = $date_formatter;
  }

  /**
   * {@inheritdoc}
   */
  public static function createInstance(ContainerInterface $container, EntityTypeInterface $entity_type) {
    return new static(
      $entity_type,
      $container->get('entity.manager')->getStorage($entity_type->id()),
      $container->get('date.formatter')
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

    $row['label']['data'] = array(
      '#type' => 'link',
      '#title' => SafeMarkup::checkPlain($entity->label()),
      '#url' => $entity->urlInfo(),
    );

    $row['type'] = SafeMarkup::checkPlain($entity->get('type')->entity->label());

    $row['changed'] = $this->dateFormatter->format($entity->get('changed')->value, 'short');

    return $row + parent::buildRow($entity);
  }

  /**
   * {@inheritdoc}
   */
  public function render() {
    $build = parent::render();

    $build['table']['#empty'] = $this->t('There are no contacts available. Add one now.');

    return $build;
  }
}

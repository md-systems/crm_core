<?php

/**
 * @file
 * Contains \Drupal\crm_core_contact\ContactTypeListBuilder.
 */

namespace Drupal\crm_core_contact;

use Drupal\Component\Utility\Xss;
use Drupal\Core\Config\Entity\ConfigEntityListBuilder;
use Drupal\Core\Entity\EntityInterface;

/**
 * Class ContactTypeListBuilder
 *
 * List builder for the contact type entity.
 *
 * @package Drupal\crm_core_contact
 * @see \Drupal\crm_core_contact\Entity\ContactType
 */
class ContactTypeListBuilder extends ConfigEntityListBuilder {

  /**
   * {@inheritdoc}
   */
  public function buildHeader() {
    $header = array();

    $header['title'] = $this->t('Name');

    $header['description'] = array(
      'data' => $this->t('Description'),
      'class' => array(RESPONSIVE_PRIORITY_MEDIUM),
    );

    return $header + parent::buildHeader();
  }

  /**
   * {@inheritdoc}
   */
  public function buildRow(EntityInterface $entity) {
    $row = array();

    $row['title'] = array(
      'data' => $this->getLabel($entity),
      'class' => array('menu-label'),
    );

    $row['description'] = Xss::filterAdmin($entity->description);

    return $row + parent::buildRow($entity);
  }
}

<?php

/**
 * @file
 * Contains \Drupal\crm_core_match\Matcher\MatcherListBuilder.
 */

namespace Drupal\crm_core_match\Matcher;

use Drupal\Core\Config\Entity\ConfigEntityListBuilder;
use Drupal\Core\Entity\EntityInterface;

/**
 * Defines a class to build a listing of matcher config entities.
 *
 * @see \Drupal\crm_core_match\Entity\Matcher
 */
class MatcherListBuilder extends ConfigEntityListBuilder {

  /**
   * {@inheritdoc}
   */
  public function buildHeader() {
    // Overrides the original Header completely.
    $header['label'] = $this->t('Label');
    $header['description'] = $this->t('Description');
    $header['plugin'] = $this->t('Match engine');

    return $header + parent::buildHeader();
  }

  /**
   * {@inheritdoc}
   */
  public function buildRow(EntityInterface $entity) {
    /** @var \Drupal\crm_core_match\Entity\Matcher $entity */
    $row['label'] = $entity->label();
    $row['description'] = $entity->getDescription();
    $row['plugin'] = $entity->getPluginTitle();

    return $row + parent::buildRow($entity);
  }

}

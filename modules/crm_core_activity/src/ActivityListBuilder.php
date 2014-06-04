<?php

/**
 * @file
 * Contains \Drupal\crm_core_activity\ActivityListBuilder.
 */

namespace Drupal\crm_core_activity;

use Drupal\Core\Entity\EntityListBuilder;

class ActivityListBuilder extends EntityListBuilder {

  /**
   * {@inheritdoc}
   */
  public function render() {
    $build = parent::render();

    $build['#empty'] = $this->t('There are no activities available.');

    return $build;
  }
}

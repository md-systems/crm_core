<?php
/**
 * @file
 * Contains \Drupal\crm_core_submission\Submission.
 */

namespace Drupal\crm_core_submission;

use Symfony\Component\HttpFoundation\ParameterBag;

class Submission extends ParameterBag implements MultipartInterface {

  /**
   * {@inheritdoc}
   */
  public function setPart($identifier, $data) {
    $this->set($identifier, $data);
  }

  /**
   * {@inheritdoc}
   */
  public function getPart($identifier) {
    $this->get($identifier);
  }

  /**
   * {@inheritdoc}
   */
  public function hasPart($identifier) {
    $this->has($identifier);
  }

  /**
   * {@inheritdoc}
   */
  public function getParts() {
    return $this->all();
  }

}

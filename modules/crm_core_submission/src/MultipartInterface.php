<?php
/**
 * @file
 * Contains \Drupal\crm_core_submission\MultipartInterface.
 *
 * @todo Move to a multipart module similar to hal.
 */

namespace Drupal\crm_core_submission;

interface MultipartInterface extends \IteratorAggregate {

  /**
   * Adds a part.
   *
   * @param string $identifier
   *   The name of the part or some other identifier.
   * @param mixed $data
   *   The data.
   */
  public function setPart($identifier, $data);

  /**
   * Gets a part.
   *
   * @param string $identifier
   *   The name of the part or some other identifier.
   *
   * @return mixed|null
   *   The data. NULL if not defined.
   */
  public function getPart($identifier);

  /**
   * Tests if a part is set.
   *
   * @param string $identifier
   *   The name of the part or some other identifier.
   *
   * @return bool
   *   TRUE if part exists, FALSE otherwise.
   */
  public function hasPart($identifier);

  /**
   * Gets all parts.
   *
   * @return array
   *   Array containing all parts.
   */
  public function getParts();
}

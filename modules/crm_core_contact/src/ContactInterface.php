<?php
/**
 * @file
 * Contains \Drupal\crm_core_contact\ContactInterface.
 */

namespace Drupal\crm_core_contact;

use Drupal\Core\Entity\ContentEntityInterface;
use Drupal\Core\Entity\EntityChangedInterface;

/**
 * Defines methods for CRM Contact entities.
 */
interface ContactInterface extends ContentEntityInterface, EntityChangedInterface {

}

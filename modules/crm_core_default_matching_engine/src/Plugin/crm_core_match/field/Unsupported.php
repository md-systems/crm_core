<?php
/**
 * @file
 * Contains Drupal\crm_core_default_matching_engine\Plugin\crm_core_match\field\Unsupported.
 */

namespace Drupal\crm_core_default_matching_engine\Plugin\crm_core_match\field;

use Drupal\crm_core_contact\ContactInterface;
use Drupal\crm_core_contact\Entity\Contact;

/**
 * Class for evaluating unsupported fields.
 *
 * @CrmCoreMatchFieldHandler (
 *   id = "unsupported"
 * )
 */
class Unsupported extends FieldHandlerBase {

  /**
   * {@inheritdoc}
   */
  public function getOperators($property = 'value') {
    return array();
  }

  /**
   * {@inheritdoc}
   */
  public function match(ContactInterface $contact, $property = 'value') {
    return array();
  }

}

<?php

/**
 * @file
 * Implementation of FieldHandlerInterface for email fields.
 */

namespace Drupal\crm_core_match\Plugin\crm_core_match\field;

/**
 * Class for evaluating email fields.
 *
 * @CrmCoreMatchFieldHandler (
 *   id = "email"
 * )
 */
class Email extends FieldHandlerBase {

  /**
   * {@inheritdoc}
   */
  public function getOperators($property = 'value') {
    return array(
      '=' => t('Equals'),
    );
  }
}

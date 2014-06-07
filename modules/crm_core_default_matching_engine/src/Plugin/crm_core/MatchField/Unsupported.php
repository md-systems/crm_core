<?php
/**
 * @file
 * Contains Drupal\crm_core_default_matching_engine\Plugin\crm_core\MatchField\Unsupported.
 */

namespace Drupal\crm_core_default_matching_engine\Plugin\crm_core\MatchField;

use Drupal\Core\Field\FieldDefinitionInterface;

/**
 * Class for evaluating unsupported fields.
 *
 * @MatchField (
 *   id = "unsupported"
 * )
 */
class Unsupported extends MatchFieldBase {

  /**
   * {@inheritdoc}
   */
  public function operators() {
    return array();
  }

  /**
   * {@inheritdoc}
   */
  public function fieldRender(FieldDefinitionInterface $field) {
    $form = parent::fieldRender($field);

    // Disable fields.
    foreach ($form as &$field) {
      $field['#weight'] += 100;
      $field['supported']['#value'] = FALSE;
      $field['status']['#disabled'] = TRUE;
      $field['status']['#default_value'] = FALSE;
      $field['operator']['#disabled'] = TRUE;
      $field['options']['#disabled'] = TRUE;
      $field['score']['#disabled'] = TRUE;
      $field['weight']['#disabled'] = TRUE;
    }

    return $form;
  }
}

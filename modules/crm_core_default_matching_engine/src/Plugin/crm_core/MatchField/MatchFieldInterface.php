<?php

/**
 * @file
 * Contains \Drupal\crm_core_default_matching_engine\Plugin\crm_core\MatchField\MatchFieldInterface.
 */

namespace Drupal\crm_core_default_matching_engine\Plugin\crm_core\MatchField;

use Drupal\Core\Field\FieldDefinitionInterface;

/**
 * Interface for defining the logical operators and query criteria used to identify duplicate contacts based on
 * different field types in DefaultMatchingEngine.
 */
interface MatchFieldInterface {

  /**
   * Field Renderer.
   *
   * Used for complex field types such as name.
   * Renders them into component parts for use in applying logical operators and ordering functions.
   *
   * @param \Drupal\Core\Field\FieldDefinitionInterface $field
   *   The field being rendered
   *
   * @return array
   *   The form elements.
   */
  public function fieldRender(FieldDefinitionInterface $field);

  /**
   * Operators.
   *
   * Defines the logical operators that can be used by this field type.
   * Provides any additional fields needed to capture information used in logical evaluations.
   * For instance: if this was a text field, there might be 3 logical operators: EQUALS, STARTS WITH, and ENDS WITH.
   * This function should return a select list with the operator values, and a text field to be used to enter
   * something like 'first 3'.
   */
  public function operators();

  /**
   * Query.
   *
   * Used when generating queries to identify matches in the system
   */
  public function fieldQuery($contact, $rule);
}

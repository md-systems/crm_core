<?php

/**
 * @file
 * Contains \Drupal\crm_core_default_matching_engine\Plugin\crm_core_match\field\FieldHandlerInterface.
 */

namespace Drupal\crm_core_default_matching_engine\Plugin\crm_core_match\field;

use Drupal\crm_core_contact\ContactInterface;
use Drupal\crm_core_contact\Entity\Contact;

/**
 * Interface for defining the logical operators and query criteria used to identify duplicate contacts based on
 * different field types in DefaultMatchingEngine.
 */
interface FieldHandlerInterface {

  /**
   * Returns the names of the field's subproperties.
   *
   * @return string[]
   *   The property names.
   */
  public function getPropertyNames();

  /**
   * Gets the property label.
   *
   * @param string $property
   *   The name of the property.
   *
   * @return string
   *   The property label.
   */
  public function getLabel($property = 'value');


  /**
   * Gets the property status.
   *
   * @param string $property
   *   The name of the property.
   *
   * @return bool
   *   The property status.
   */
  public function getStatus($property = 'value');

  /**
   * Gets the field type.
   *
   * @return string
   *   The name of the field type.
   */
  public function getType();

  /**
   * Gets the operators.
   *
   * Defines the logical operators that can be used by this field type.
   * Provides any additional fields needed to capture information used in
   * logical evaluations. See {@code QueryInterface::condition} for valid
   * operators.
   *
   * For instance: if this was a text field, there might be 4 logical operators:
   * =, STARTS_WITH, ENDS_WITH and CONTAINS.
   *
   * @param string $property
   *   The name of the property.
   *
   * @return string[]
   *   Array of operators, with the operator name as key and the translated
   *   operator label as value.
   */
  public function getOperators($property = 'value');

  /**
   * Gets the current selected operator.
   *
   * @param string $property
   *   The name of the property.
   *
   * @return string
   *   The operator name.
   *
   * @see FieldHandlerInterface::getOperators()
   */
  public function getOperator($property = 'value');

  /**
   * Gets the operator options.
   *
   * @param string $property
   *   The name of the property.
   *
   * @return string
   *   The operator options.
   *
   * @see FieldHandlerInterface::getOperators()
   * @see FieldHandlerInterface::getOperator()
   */
  public function getOptions($property = 'value');

  /**
   * Gets the score.
   *
   * @param string $property
   *   The name of the property.
   *
   * @return int
   *   The score.
   */
  public function getScore($property = 'value');

  /**
   * Gets the weight.
   *
   * @param string $property
   *   The name of the property.
   *
   * @return int
   *   The weight.
   */
  public function getWeight($property = 'value');

  /**
   * Executes the match query.
   *
   * @param \Drupal\crm_core_contact\Entity\Contact $contact
   *   The contact to find matches for.
   *
   * @param string $property
   *   The name of the property.
   *
   * @return array
   *   An array containing the found matches.
   *   The first level keys are the contact ids found as matches.
   *   The second level key is the rule id responsible for the match containing
   *   its score as value.
   *   @code
   *   array(
   *     $contact_id => array(
   *       $rule_id => $core,
   *     ),
   *   );
   *   @end
   */
  public function match(ContactInterface $contact, $property = 'value');
}

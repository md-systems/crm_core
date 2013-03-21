<?php

/**
 * @file
 * Default match engine definitions.
 */

define('MATCH_DEFAULT_CHARS', 3);

/**
 * Interface for defining the logical operators and query criteria used to identify duplicate contacts based on
 * different field types in DefaultMatchingEngine.
 */
interface DefaultMatchingEngineFieldTypeInterface {

  /**
   * Field Renderer.
   *
   * Used for complex field types such as name.
   * Renders them into component parts for use in applying logical operators and ordering functions.
   *
   * @param array $field
   *   The field being rendered
   * @param array $field_info
   *   Info of the field  being rendered
   * @param array $form
   *   Form to be modified.
   */
  public function fieldRender($field, $field_info, &$form);

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

abstract class DefaultMatchingEngineFieldType implements DefaultMatchingEngineFieldTypeInterface {

  /**
   * Template used to render fields matching rules configuration form.
   *
   * @param array $field
   *   Field to render config for.
   * @param array $field_info
   *   Field info.
   * @param array $form
   *   Form to be modified
   */
  protected function fieldRenderTemplate($field, $field_info, &$form) {
    $field_name = $field['field_name'];
    $field_item = isset($field['field_item']) ? $field['field_item'] : '';
    $field_label = $field['label'];
    $contact_type = $field['bundle'];

    $config = crm_core_default_matching_engine_load_field_config($contact_type, $field_name, $field_item);

    $form['field_matching'][$field_name . $field_item]['#weight'] = $config['weight'];

    $form['field_matching'][$field_name . $field_item]['field_type'] = array(
      '#type' => 'value',
      '#value' => $field_info['type'],
    );

    $form['field_matching'][$field_name . $field_item]['field_name'] = array(
      '#type' => 'value',
      '#value' => $field_name,
    );

    $form['field_matching'][$field_name . $field_item]['field_item'] = array(
      '#type' => 'value',
      '#value' => $field_item,
    );

    $form['field_matching'][$field_name . $field_item]['status'] = array(
      '#type' => 'checkbox',
      '#default_value' => $config['status'],
    );

    $form['field_matching'][$field_name . $field_item]['name'] = array(
      '#type' => 'item',
      '#markup' => $field_label,
    );

    $form['field_matching'][$field_name . $field_item]['field_type_markup'] = array(
      '#type' => 'item',
      '#markup' => $field_info['type'],
    );

    $operator = array(
      '#type' => 'select',
      '#default_value' => $config['operator'],
    );
    switch ($field_info['type']) {
      case 'date':
      case 'datestamp':
      case 'datetime':
        $operator += array(
          '#options' => $this->operators($field_info),
          '#empty_option' => t('-- Please Select --'),
          '#empty_value' => '',
        );
        break;

      case 'name':
        $class = get_class($this);
        if ($class == 'selectMatchField') {
          $operator += array('#options' => $this->operators());
        }
        elseif ($class == 'textMatchField') {
          $operator += array(
            '#options' => $this->operators(),
            '#empty_option' => t('-- Please Select --'),
            '#empty_value' => '',
          );
        }
        break;

      case 'select':
      case 'email':
        $operator += array('#options' => $this->operators());
        break;

      case 'text':
        $operator += array(
          '#options' => $this->operators(),
          '#empty_option' => t('-- Please Select --'),
          '#empty_value' => '',
        );
        break;
    }

    $form['field_matching'][$field_name . $field_item]['operator'] = $operator;

    // @TODO: add a textfield for any extra information that's needed
    $form['field_matching'][$field_name . $field_item]['options'] = array(
      '#type' => 'item',
      '#markup' => '---',
    );

    $form['field_matching'][$field_name . $field_item]['score'] = array(
      '#type' => 'textfield',
      '#maxlength' => 28,
      '#size' => 28,
      '#default_value' => $config['score'],
    );

    $form['field_matching'][$field_name . $field_item]['weight'] = array(
      '#type' => 'weight',
      '#default_value' => $config['weight'],
      '#attributes' => array(
        'class' => array('crm-core-match-engine-order-weight'),
      ),
    );
  }
}

/**
 * DefaultMatchingEngine class
 *
 * Extends CrmCoreMatchEngine to provide rules for identifying duplicate contacts.
 */
class DefaultMatchingEngine extends CrmCoreMatchEngine {

  /**
   * Constructor: sets basic variables.
   */
  public function __construct() {
    $this->name = t('Default Matching Engine');
    $this->machineName = 'default_matching_engine';
    $this->description = t('This is a simple matching engine from CRM Core. Allows administrators to specify matching'
      . ' rules for individual contact types on a field-by-field basis.');
    $this->settings = array();
  }

  /**
   * Applies logical rules for identifying matches in the database.
   *
   * Any matching engine should implement this to apply it's unique matching logic.
   * Variables are passed in by reference, so it's not necessary to return anything.
   * Accepts a list of matches and contact information to identify potential duplicates.
   *
   * @see CrmCoreMatchEngineInterface::execute()
   */
  public function execute(&$contact, &$ids = array()) {
    if ($this->status) {
      $base_config = crm_core_default_matching_engine_load_contact_type_config($contact->type);
      // Check if match is enabled for this contact type.
      if ($base_config['status']) {
        $matching_rules = crm_core_default_matching_engine_load_field_config($contact->type);
        $contact_fields = field_info_instances('crm_core_contact', $contact->type);
        $results = array();
        foreach ($matching_rules as $matching_rule) {
          if (isset($contact_fields[$matching_rule->field_name])) {
            $rule_matches = array();
            $field_match_handler_class = $matching_rule->field_type . 'MatchField';
            if (class_exists($field_match_handler_class)) {
              $field_match_handler = new $field_match_handler_class();
              $rule_matches = $field_match_handler->fieldQuery($contact, $matching_rule);

            }
            foreach ($rule_matches as $matched_id) {
              $results[$matched_id][] = $matching_rule->score;
            }
          }
        }
        foreach ($results as $id => $rule_matches) {
          $total_score = array_sum($rule_matches);
          if ($total_score >= $base_config['threshold']) {
            $ids[] = $id;
          }
        }
      }
    }
  }
}

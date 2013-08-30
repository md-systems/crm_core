<?php

/**
 * @file
 * Default match engine definitions.
 */

if (!defined('MATCH_DEFAULT_CHARS')) {
  define('MATCH_DEFAULT_CHARS', '3');
}

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

  const WEIGHT_DELTA = 25;

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
  public function fieldRender($field, $field_info, &$form) {
    $disabled = FALSE;
    if (get_class($this) == 'UnsupportedFieldMatchField') {
      $disabled = TRUE;
    }
    $field_name = $field['field_name'];
    $field_item = isset($field['field_item']) ? $field['field_item'] : '';
    $separator = empty($field_item) ? $field_name : $field_name . '_' . $field_item;
    $field_label = $field['label'];
    if ($disabled) {
      $field_label .= ' (UNSUPPORTED)';
    }
    $contact_type = $field['bundle'];

    $config = crm_core_default_matching_engine_load_field_config($contact_type, $field_name, $field_item);
    $display_weight = self::WEIGHT_DELTA;
    if ($config['weight'] == 0) {
      // Table row positioned incorrectly if "#weight" is 0.
      $display_weight = 0.001;
    }
    else {
      $display_weight = $config['weight'];
    }

    $form['field_matching'][$separator]['#weight'] = $disabled ? 100 : $display_weight;

    $form['field_matching'][$separator]['supported'] = array(
      '#type' => 'value',
      '#value' => $disabled ? FALSE : TRUE,
    );

    $form['field_matching'][$separator]['field_type'] = array(
      '#type' => 'value',
      '#value' => $field_info['type'],
    );

    $form['field_matching'][$separator]['field_name'] = array(
      '#type' => 'value',
      '#value' => $field_name,
    );

    $form['field_matching'][$separator]['field_item'] = array(
      '#type' => 'value',
      '#value' => $field_item,
    );

    $form['field_matching'][$separator]['status'] = array(
      '#type' => 'checkbox',
      '#default_value' => $config['status'],
      '#disabled' => $disabled,
    );

    $form['field_matching'][$separator]['name'] = array(
      '#type' => 'item',
      '#markup' => check_plain($field_label),
    );

    $form['field_matching'][$separator]['field_type_markup'] = array(
      '#type' => 'item',
      '#markup' => $field_info['type'],
    );

    $operator = array(
      '#type' => 'select',
      '#default_value' => $config['operator'],
      '#empty_option' => t('-- Please Select --'),
      '#empty_value' => '',
      '#disabled' => $disabled,
    );
    switch ($field_info['type']) {
      case 'date':
      case 'datestamp':
      case 'datetime':
        $operator += array(
          '#options' => $this->operators($field_info),
        );
        break;

      default:
        $operator += array(
          '#options' => $this->operators(),
        );
    }

    $form['field_matching'][$separator]['operator'] = $operator;

    $form['field_matching'][$separator]['options'] = array(
      '#type' => 'textfield',
      '#maxlength' => 28,
      '#size' => 28,
      '#default_value' => $config['options'],
      '#disabled' => $disabled,
    );

    $form['field_matching'][$separator]['score'] = array(
      '#type' => 'textfield',
      '#maxlength' => 4,
      '#size' => 3,
      '#default_value' => $config['score'],
      '#disabled' => $disabled,
    );

    $form['field_matching'][$separator]['weight'] = array(
      '#type' => 'weight',
      '#default_value' => $config['weight'],
      '#attributes' => array(
        'class' => array('crm-core-match-engine-order-weight'),
      ),
      '#disabled' => $disabled,
      '#delta' => self::WEIGHT_DELTA,
    );
  }

  /**
   * Field query to search matches.
   *
   * @param object $contact
   *   CRM Core contact entity.
   * @param object $rule
   *   Matching rule object.
   *
   * @return array
   *   Founded matches.
   */
  public function fieldQuery($contact, $rule) {

    $results = array();
    $contact_wrapper = entity_metadata_wrapper('crm_core_contact', $contact);
    $needle = '';
    $field_item = '';

    if (empty($rule->field_item)) {
      $needle = $contact_wrapper->{$rule->field_name}->value();
      $field_item = 'value';
    }
    else {
      $field_value = $contact_wrapper->{$rule->field_name}->value();
      if (isset($field_value)) {
        $needle = $contact_wrapper->{$rule->field_name}->{$rule->field_item}->value();
        $field_item = $rule->field_item;
      }
    }

    if (!empty($needle)) {
      $query = new EntityFieldQuery();
      $query->entityCondition('entity_type', 'crm_core_contact')
        ->entityCondition('bundle', $contact->type);
      $query->entityCondition('entity_id', $contact->contact_id, '<>');

      switch ($rule->operator) {
        case 'equals':
          $query->fieldCondition($rule->field_name, $field_item, $needle);
          break;

        case 'starts':
          $needle = db_like(substr($needle, 0, MATCH_DEFAULT_CHARS)) . '%';
          $query->fieldCondition($rule->field_name, $field_item, $needle, 'LIKE');
          break;

        case 'ends':
          $needle = '%' . db_like(substr($needle, -1, MATCH_DEFAULT_CHARS));
          $query->fieldCondition($rule->field_name, $field_item, $needle, 'LIKE');
          break;

        case 'contains':
          $needle = '%' . db_like($needle) . '%';
          $query->fieldCondition($rule->field_name, $field_item, $needle, 'LIKE');
          break;
      }
      $results = $query->execute();
    }

    return isset($results['crm_core_contact']) ? array_keys($results['crm_core_contact']) : $results;
  }

  /**
   * Each field handler MUST implement this method.
   *
   * Must return associative array of supported operators for current field.
   * By default now supports only this keys: 'equals', 'starts', 'ends',
   * 'contains'. In case you need additional operators you must implement
   * this method and DefaultMatchingEngineFieldTypeInterface::fieldQuery.
   *
   * @return array
   *   Assoc array, keys must be
   */
  public function operators() {
  }
}

class UnsupportedFieldMatchField extends DefaultMatchingEngineFieldType {

  public function operators() {
    return array();
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
    $description = 'This is a simple matching engine from CRM Core. Allows administrators to specify matching'
      . ' rules for individual contact types on a field-by-field basis.';
    $this->description = t($description);
    $this->settings = array(
      array(
        'name' => 'settings',
        'path' => 'admin/config/crm-core/match/default_match',
        'label' => t('Configuration'),
      ),
    );
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
              $results[$matched_id][$matching_rule->mrid] = $matching_rule->score;
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

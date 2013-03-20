<?php

/**
 * @file
 * Implementation of DefaultMatchingEngineFieldTypeInterface for date fields.
 */


/**
 * Class for evaluating date fields.
 */
class datetimeMatchField implements DefaultMatchingEngineFieldTypeInterface {

  /**
   * Constructs the UI for configuring matching on date fields.
   *
   * @see CrmCoreMatchField::fieldRender()
   */
  public function fieldRender($field, $field_info, &$form) {

    $field_name = $field['field_name'];
    $field_label = $field['label'];
    $contact_type = $field['bundle'];

    $config = _crm_core_default_matching_engine_load_field_config($contact_type, $field_name);

    $form['field_matching'][$field_name]['#weight'] = $config['weight'];

    $form['field_matching'][$field_name]['status'] = array(
      '#type' => 'checkbox',
      '#default_value' => $config['status'],
    );

    $form['field_matching'][$field_name]['name'] = array(
      '#type' => 'item',
      '#markup' => $field_label,
    );

    $form['field_matching'][$field_name]['field_name'] = array(
      '#type' => 'item',
      '#markup' => $field_info['type'],
    );

    $form['field_matching'][$field_name]['operator'] = array(
      '#type' => 'select',
      '#default_value' => $config['operator'],
      '#options' => $this->operators($field_info),
      '#empty_option' => t('-- Please Select --'),
      '#empty_value' => '',
    );

    // @TODO: add a textfield for any extra information that's needed
    $form['field_matching'][$field_name]['options'] = array(
      '#type' => 'item',
      '#markup' => '---',
    );

    $form['field_matching'][$field_name]['score'] = array(
      '#type' => 'textfield',
      '#maxlength' => 28,
      '#size' => 28,
      '#default_value' => $config['score'],
    );

    $form['field_matching'][$field_name]['weight'] = array(
      '#type' => 'weight',
      '#default_value' => $config['weight'],
      '#attributes' => array(
        'class' => array('crm-core-match-engine-order-weight'),
      ),
    );

  }

  /**
   * Defines logical operators to use with this field.
   *
   * This operators would be interpreted in fieldQuery() method.
   *
   * @param array $field_info
   *   Array returned by field_info_field($field_name).
   *
   * @return array
   *   Assoc array of operators.
   */
  public function operators($field_info = NULL) {

    $operators = array(
      'equals' => t('Equals'),
    );

    if (isset($field_info['columns']['value2'])) {
      $operators['between'] = t('Between');
    }

    return $operators;
  }

  public function fieldQuery() {
  }
}

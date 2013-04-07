<?php

/**
 * @file
 * Implementation of DefaultMatchingEngineFieldTypeInterface for email fields.
 */

/**
 * Class for evaluating email fields.
 */
class EmailMatchField extends SelectMatchField {
  
  
  public function fieldRender($field, $field_info, &$form) {
    
    $field_name = $field['field_name'];
    $field_item = 'email';
    
    $separator = empty($field_item) ? $field_name : $field_name . '_' . $field_item;
    
    $field_label = $field['label'];
    
    $contact_type = $field['bundle'];

    $config = crm_core_default_matching_engine_load_field_config($contact_type, $field_name, $field_item);
    
    dpm($config);
    
    $display_weight = self::WEIGHT_DELTA;
    if ($config['weight'] == 0) {
      // Table row positioned incorrectly if "#weight" is 0.
      $display_weight = 0.001;
    }
    else {
      $display_weight = $config['weight'];
    }

    $form['field_matching'][$separator]['#weight'] = $display_weight;

    $form['field_matching'][$separator]['supported'] = array(
      '#type' => 'value',
      '#value' => TRUE,
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
      '#disabled' => FALSE,
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
      '#disabled' => FALSE,
      '#options' => $this->operators(),
    );
    
    $form['field_matching'][$separator]['operator'] = $operator;

    $form['field_matching'][$separator]['options'] = array(
      '#type' => 'textfield',
      '#maxlength' => 28,
      '#size' => 28,
      '#default_value' => $config['options'],
      '#disabled' => FALSE,
    );

    $form['field_matching'][$separator]['score'] = array(
      '#type' => 'textfield',
      '#maxlength' => 4,
      '#size' => 3,
      '#default_value' => $config['score'],
      '#disabled' => FALSE,
    );

    $form['field_matching'][$separator]['weight'] = array(
      '#type' => 'weight',
      '#default_value' => $config['weight'],
      '#attributes' => array(
        'class' => array('crm-core-match-engine-order-weight'),
      ),
      '#disabled' => FALSE,
      '#delta' => self::WEIGHT_DELTA,
    );
  }
    
}

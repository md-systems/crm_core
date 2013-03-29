<?php

/**
 * @file
 * Administrative screens.
 */

/**
 * Creates a form for customizing the matching rules per contact type.
 */
function crm_core_default_matching_engine_form($form, &$form_state, $contact_type) {
  $config = crm_core_default_matching_engine_load_contact_type_config($contact_type);

  $form['contact_type'] = array(
    '#type' => 'value',
    '#value' => $contact_type,
  );

  $form['status'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable matching for this contact type'),
    '#description' => t('Check this box to allow CRM Core to check for duplicate contact records for this contact type.'),
    '#default_value' => $config['status'],
  );

  $form['threshold'] = array(
    '#type' => 'textfield',
    '#title' => t('Threshold'),
    '#description' => t('Defines the score at which a contact is considered a match.'),
    '#maxlength' => 28,
    '#size' => 28,
    '#required' => TRUE,
    '#default_value' => $config['threshold'],
  );

  $return_description = 'If two or more contact records result in matches with identical scores, CRM Core will give'
    . ' preference to one over the other base on selected option.';
  $form['return_order'] = array(
    '#type' => 'select',
    '#title' => t('Return Order'),
    '#description' => t($return_description),
    '#default_value' => $config['return_order'],
    '#options' => array(
      'created' => t('Most recently created'),
      'updated' => t('Most recently updated'),
      'associated' => t('Associated with user'),
    ),
  );

  $strict_description = 'Check this box to return a match for this contact type the first time one is identified that'
    . ' meets the threshold. Stops redundant processing.';
  $form['strict'] = array(
    '#type' => 'checkbox',
    '#title' => t('Strict matching'),
    '#description' => t($strict_description),
    '#default_value' => $config['strict'],
  );

  $form['fields'] = array(
    '#type' => 'item',
    '#title' => t('Field Matching'),
  );

  $form['field_matching'] = array(
    '#type' => 'container',
    '#tree' => TRUE,
  );

  foreach (field_info_instances('crm_core_contact', $contact_type) as $field_name => $field) {
    $field_info = field_info_field($field_name);
    $field_type = $field_info['type'];
    $field_handler = $field_type . 'MatchField';
    if (class_exists($field_handler)) {
      $obj = new $field_handler();
      $obj->fieldRender($field, $field_info, $form);
    }
    else {
      $obj = new UnsupportedFieldMatchField();
      $obj->fieldRender($field, $field_info, $form);
    }
  }

  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
  );

  return $form;
}

/**
 * Validate handler for crm_core_default_matching_engine_form.
 */
function crm_core_default_matching_engine_form_validate($form, &$form_state) {
  $fields_rules = array();
  if (isset($form_state['values']['field_matching'])) {
    $fields_rules = $form_state['values']['field_matching'];
  }
  foreach ($fields_rules as $field_name => $config) {
    if ($config['status'] && empty($config['operator'])) {
      form_set_error('field_matching][' . $field_name . '][operator', t('You must select an operator for enabled field.'));
    }
    if (!is_numeric($config['score'])) {
      form_set_error('field_matching][' . $field_name . '][score', t('You must enter number in "Score" column.'));
    }
  }
}

/**
 * Submit handler for crm_core_default_matching_engine_form.
 */
function crm_core_default_matching_engine_form_submit($form, &$form_state) {
  if (!isset($form_state['values'])) {
    return;
  }

  $values = $form_state['values'];
  db_merge('crm_core_match_contact_types')
    ->condition('contact_type', $values['contact_type'])
    ->fields(array(
      'contact_type' => $values['contact_type'],
      'threshold' => $values['threshold'],
      'status' => $values['status'],
      'strict' => $values['strict'],
      'return_order' => $values['return_order'],
    ))
    ->execute();

  if (isset($values['field_matching'])) {
    foreach ($values['field_matching'] as $config) {
      if ($config['supported']) {
        db_merge('crm_core_match_contact_type_rules')
          ->condition('contact_type', $values['contact_type'])
          ->condition('field_name', $config['field_name'])
          ->condition('field_item', $config['field_item'])
          ->fields(array(
            'contact_type' => $values['contact_type'],
            'field_type' => $config['field_type'],
            'field_name' => $config['field_name'],
            'field_item' => $config['field_item'],
            'status' => $config['status'],
            'operator' => $config['operator'],
            'options' => $config['options'],
            'weight' => $config['weight'],
            'score' => $config['score'],
          ))
          ->execute();
      }
    }
  }
  drupal_set_message(t('The configuration options have been saved.'));
}

/**
 * Theme implementation of the crm_core_match_admin_config_engines_form form.
 */
function theme_crm_core_default_matching_engine_form($variables) {
  $form = $variables['form'];

  $rows = array();
  foreach (element_children($form['field_matching'], TRUE) as $key) {
    $rows[] = array(
      'data' => array(
        drupal_render($form['field_matching'][$key]['status']),
        drupal_render($form['field_matching'][$key]['name']),
        drupal_render($form['field_matching'][$key]['field_type_markup']),
        drupal_render($form['field_matching'][$key]['operator']),
        drupal_render($form['field_matching'][$key]['options']),
        drupal_render($form['field_matching'][$key]['score']),
        drupal_render($form['field_matching'][$key]['weight']),
      ),
      'class' => array('draggable'),
    );
  }

  $header = array(
    t('Enabled'),
    t('Name'),
    t('Field Type'),
    t('Operator'),
    t('Options'),
    t('Score'),
    t('Weight'),
  );
  $form['field_matching']['content']['#markup'] = theme('table', array(
    'header' => $header,
    'rows' => $rows,
    'attributes' => array('id' => 'crm-core-match-engine-order'),
  ));
  $output = drupal_render_children($form);

  drupal_add_tabledrag('crm-core-match-engine-order', 'order', 'sibling', 'crm-core-match-engine-order-weight');

  return $output;
}

/**
 * Page callback for engine configuration page.
 */
function crm_core_default_matching_engine_config_page() {

  $item = menu_get_item();
  $types = crm_core_contact_types();
  $content = array();
  foreach ($types as $type) {
    $_content = menu_get_item($item['href'] . '/' . $type->type . '/edit');
    if ($_content['access']) {
      $_content['description'] = $_content['title'];
      $content[] = $_content;
    }
  }

  return theme('crm_core_contact_ui_add_list', array('content' => $content));
}

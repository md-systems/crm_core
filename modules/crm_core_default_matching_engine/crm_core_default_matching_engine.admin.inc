<?php

/**
 * @file
 * Administrative screens.
 */

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

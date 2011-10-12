<?php

/**
 * Crm relationship settings form.
 */
function crm_relationship_settings() {
  $form = array();

  $form['crm_relationship_settings_types'] = array(
    '#type' => 'checkboxes',
    '#options' => relation_list_types(),
    '#default_value' => variable_get('crm_relationship_settings_types', array()),
    '#title' => t('CRM relationships'),
    '#description' => t('Enable relationships, that can be used by CRM module.'),
  );

  return system_settings_form($form);
}

/**
 * List crm relationship types
 */
function crm_relationship_types_overview() {
  $relation_types = relation_list_types();
  $crm_relationship_types = array_values(variable_get('crm_relationship_settings_types', array()));
  $field_ui = module_exists('field_ui');

  $headers = array(
    array('data' => t('Name'), 'width' => '40%'),
    array('data' => t('Operations'), 'colspan' => $field_ui ? '4' : '2')
  );
  $rows = array();

  foreach ($crm_relationship_types as $type) {
    if (!$type) continue;
    
    $url = 'admin/structure/relation/manage/' . $type;
    $url_local = 'crm/admin/relationship/types/' . $type;

    $row = array(l($type, $url_local));
    $row[] = l(t('edit'), $url_local . '/edit');
    if ($field_ui) {
      $row[] =  l(t('manage fields'), $url . '/fields');
      $row[] =  l(t('display fields'), $url . '/display');
    }
    $row[] =  l(t('delete'), $url . '/delete');
    $rows[] = $row;
  }

  $build['crm_relationship_types_table'] = array(
    '#theme' => 'table',
    '#header' => $headers,
    '#rows' => $rows,
  );

  return $build;
}

function crm_relationship_type_form($form, &$form_state, $relation_type = array()) {
  return array();
}
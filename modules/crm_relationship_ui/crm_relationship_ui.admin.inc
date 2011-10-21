<?php

/**
 * List crm relationship types
 */
function crm_relationship_ui_types_overview() {
  module_load_include('inc', 'crm_relationship_ui');

  $relation_types = relation_list_types();
  
  $field_ui = module_exists('field_ui');
  $headers = array(
    array('data' => t('Name'), 'width' => '40%'),
    array('data' => t('Operations'), 'colspan' => $field_ui ? '4' : '2')
  );
  $rows = array();

  foreach ($relation_types as $type => $label) { 
    if (!crm_relationship_ui_is_relationship_type($type)) continue;
    
    $url = 'admin/structure/relation/manage/' . $type;

    $row = array(l($type, $url));
    $row[] = l(t('edit'), $url . '/edit');
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
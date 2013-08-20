<?php

/**
 * @file
 * Contains theme implementation.
 */

/**
 * Renders table to merge contacts.
 */
function theme_crm_core_contact_merge_table($vars) {
  $table = $vars['table'];
  $header_dirty = $table['contact_id'];
  unset($table['contact_id']);
  $header = array();
  foreach (element_children($header_dirty) as $key) {
    $header[$key]['data'] = $header_dirty[$key];
  }

  $selected_contacts = $table['#selected'];
  unset($table['#selected']);
  $rows = array();
  foreach (element_children($table) as $id) {
    $row = array('data' => array());
    $row['data'][] = drupal_render($table[$id]['field_name']);
    foreach ($selected_contacts as $cid) {
      $row['data'][] = drupal_render($table[$id][$cid]);
    }
    $rows[] = $row;
  }
  $output = theme('table', array(
    'header' => $header,
    'rows' => $rows,
    'attributes' => array('id' => 'merge-contacts-table'),
  ));

  return $output;
}

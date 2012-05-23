<?php

/**
 * @file
 * Altering queries.
 */

/**
 * Implements hook_views_query_alter
 */
function crm_core_contact_ui_views_query_alter(&$view, &$query) {
  // we are limiting the visibility of the contact listing
  // view to people without proper permissions
  if ($view->name == 'crm_core_contacts') {
    if (!user_access('view disabled contact types')) {
        // we will have to join the tables first
        // we have to respect if any filter has already created the join
        if (!array_key_exists('crm_core_contact_type', $query->table_queue)) {
          $query->table_queue['crm_core_contact_type'] = array(
            'table' => 'crm_core_contact_type',
            'num' => 1,
            'alias' => 'crm_core_contact_type',
            'relationship' => 'crm_core_contact',
          );
          
          $query->table_queue['crm_core_contact_type']['join'] = new views_join();
          $query->table_queue['crm_core_contact_type']['join']->definition = array(
            'left_field' => 'type',
            'field' => 'type',
            'table' => 'crm_core_contact_type',
            'left_table' => 'crm_core_contact',          
          );
          $query->table_queue['crm_core_contact_type']['join']->extra_type = 'AND';
          $query->table_queue['crm_core_contact_type']['join']->table = 'crm_core_contact_type';
          $query->table_queue['crm_core_contact_type']['join']->left_table = 'crm_core_contact';
          $query->table_queue['crm_core_contact_type']['join']->left_field = 'type';
          $query->table_queue['crm_core_contact_type']['join']->field = 'type';
          $query->table_queue['crm_core_contact_type']['join']->type = 'LEFT';
          $query->table_queue['crm_core_contact_type']['join']->adjusted = 1;
        }
        
        if (!array_key_exists('crm_core_contact_type', $query->tables['crm_core_contact'])) {
          $query->tables['crm_core_contact']['crm_core_contact_type'] = array(
            'count' => '1',
            'alias' => 'crm_core_contact_type',
          );
        }
        
        // let's construct another where clause component
        $count = (empty($query->where)) ? 0 : count($query->where);
        $query->where[$count]['type'] = 'AND';
        $query->where[$count]['args'] = array();
        $query->where[$count]['conditions'][0] = array(
          'field' => 'crm_core_contact_type.disabled',
          'value' => 0,
          'operator' => '=',
        );
    }
  }
}
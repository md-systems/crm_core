<?php

/**
 * Admin form for Og Synchronization
 */
function crm_og_sync_admin_form($form, &$form_state) { 
  
  // need to get default value from database as well
  $og_nodes = _crm_og_sync_og_types();
  if (empty($og_nodes)) {
    $form['message'] = array(
      '#type' => 'item',
      '#prefix' => '<div class="messages warning">',
      '#suffix' => '</div>',
      '#markup' => 'There\'s no organic group type node type created',
    );
    return $form;
  }
  
  foreach($og_nodes as $node_type) {
    
    $default_settings = _crm_og_sync_settings_get($node_type);
    
    $form[$node_type] = array(
      '#type' => 'fieldset',
      '#title' => $node_type,
      '#collapsible' => TRUE, 
      '#collapsed' => TRUE,
      '#tree' => TRUE,
    );
    
    $form[$node_type]['is_new'] = array(
      '#type' => 'value',
      '#value' => (empty($default_settings)) ? TRUE : FALSE,
    );
    
    // need a flag to tell if we are saving or updating
    
    $form[$node_type]['toggle'] = array(
      '#type' => 'checkbox',
      '#title' => t('enable sync?'),
      '#default_value' => (!empty($default_settings)) ? $default_settings['status'] : 0,
    );
    
    // @todo, allow selection of contact type to be synced with nodes
    // and use ajax form to load the fields associated with the contact
//    $form[$node_type]['contact_type'] = array(
//      '#type' => 'select',
//      '#title' => t('Contact type to be synced'),
//    );

    $options = array(
      '__none' => t('--Do not sync--'),
    );
    $options += _crm_og_sync_contact_fields();
    
    $default_values = unserialize($default_settings['field_mapping']);
    $node_type_fields = field_info_instances('node', $node_type);
    foreach($node_type_fields as $field_name => $field) {
      $form[$node_type]['mapping'][$field_name] = array(
        '#type' => 'select',
        '#title' => $field['label'],
        // need to load the options from the contact type
        '#options' => $options,
        '#default_value' => $default_values[$field_name],
      );    
    }  
  }
  
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save configurations'),
  );
  
  return $form;
}

function crm_og_sync_admin_form_validate($form, &$form_state) {
}

/**
 * submission handler
 */
function crm_og_sync_admin_form_submit($form, &$form_state) {
  $og_node_types = _crm_og_sync_og_types();
  $values = $form_state['values'];
  
  foreach($og_node_types as $node_type) {
     // save the record
     $record = array(
       'og_bundle_type' => $node_type,
       'og_entity_type' => 'node',
       'status' => $values[$node_type]['toggle'],
       'field_mapping' => $values[$node_type]['mapping'],
     );
     
     if ($values[$node_type]['is_new']) {
       drupal_write_record('crm_og_sync_settings', $record);
     }
     else {
       drupal_write_record('crm_og_sync_settings', $record, array('og_bundle_type', 'og_entity_type'));  
     } 
  }
  
  drupal_set_message('Configuration settings saved'); 
}

function _crm_og_sync_settings_get($bundle_type, $entity_type = 'node') {
  $data = array();
  $query = db_select('crm_og_sync_settings', 'o')
  ->fields('o')
  ->condition('og_entity_type', $entity_type)
  ->condition('og_bundle_type', $bundle_type);
  
  $result = $query->execute();
  
  $data = $result->fetchAssoc();

  return $data;
}


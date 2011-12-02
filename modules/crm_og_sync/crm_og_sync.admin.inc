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
  
  foreach($og_nodes as $og_entity_type) {
    
    $default_settings = _crm_og_sync_settings_get($og_entity_type);
    
    $form[$og_entity_type] = array(
      '#type' => 'fieldset',
      '#title' => $og_entity_type,
      '#collapsible' => TRUE, 
      '#collapsed' => TRUE,
      '#tree' => TRUE,
    );
    
    $form[$og_entity_type]['is_new'] = array(
      '#type' => 'value',
      '#value' => (empty($default_settings)) ? TRUE : FALSE,
    );
    
    // need a flag to tell if we are saving or updating
    
    $form[$og_entity_type]['toggle'] = array(
      '#type' => 'checkbox',
      '#title' => t('enable sync?'),
      '#default_value' => (!empty($default_settings)) ? $default_settings['status'] : 0,
    );
    
    // @todo, allow selection of contact type to be synced with nodes
    // and use ajax form to load the fields associated with the contact
//    $form[$og_entity_type]['contact_type'] = array(
//      '#type' => 'select',
//      '#title' => t('Contact type to be synced'),
//    );

    $options = array(
      '__none' => t('--Do not sync--'),
    );
    $options += _crm_og_sync_contact_fields();
    
    $default_values = unserialize($default_settings['field_mapping']);
    $og_entity_type_fields = field_info_instances('node', $og_entity_type);
    foreach($og_entity_type_fields as $field_name => $field) {
      $form[$og_entity_type]['mapping'][$field_name] = array(
        '#type' => 'select',
        '#title' => $field['label'],
        // need to load the options from the contact type
        '#options' => $options,
        '#default_value' => (array_key_exists($field_name, $default_values)) ? $default_values[$field_name] : '__none',
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
  
  foreach($og_node_types as $og_entity_type) {
     // save the record
     $record = array(
       'og_bundle_type' => $og_entity_type,
       'og_entity_type' => 'node',
       'status' => $values[$og_entity_type]['toggle'],
       'field_mapping' => $values[$og_entity_type]['mapping'],
     );
     
     crm_og_sync_save_config($record, $values[$og_entity_type]['is_new']);     
  }
  
  drupal_set_message('Configuration settings saved'); 
}
<?php

/**
 * Admin form for Og Synchronization
 */
function crm_og_sync_admin_form($form, &$form_state) { 
  
  $form['crm_og_sync_reverse'] = array(
    '#type' => 'checkbox',
    '#title' => 'Enable reverse sync? (experimental)',
    '#description' => t('Enable limited reverse synchronization from contact data back to organic group data (mapped fields update only)'),
    '#default_value' => variable_get('crm_og_sync_reverse', 0),
  );
  
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
  
  $form['#prefix'] = '<div id="form_target">';
  $form['#suffix'] = '</div>';
  
  foreach($og_nodes as $og_entity_type) {
    
    $default_settings = _crm_og_sync_settings_get($og_entity_type);
    $selected_contact_type = (isset($form_state['values'])) ? $form_state['values'][$og_entity_type]['contact_type'] : array();
    $form[$og_entity_type] = array(
      '#type' => 'fieldset',
      '#title' => $og_entity_type,
      '#collapsible' => TRUE, 
      '#collapsed' => FALSE,
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
    $_contact_types = crm_core_contact_types(TRUE);
    foreach($_contact_types as $_contact_type) {
      $contact_types[$_contact_type->type] = $_contact_type->name;  
    }
    $form[$og_entity_type]['contact_type'] = array(
       '#type' => 'select',
       '#title' => t('Contact type to be synced'),
       '#options' => $contact_types,
       '#default_value' => (!empty($default_settings)) ? $default_settings['crm_contact_type'] : 'organization',
       '#ajax' => array(
         'callback' => 'crm_og_sync_admin_form_ajax_callback',
         'wrapper' => 'form_target',
       ),
    );
    
    $form[$og_entity_type]['mapping'] = array(
      '#type' => 'fieldset',
      '#title' => t('Field mapping'),
      '#collapsible' => TRUE, 
      '#collapsed' => FALSE,
      '#tree' => TRUE,
      // '#prefix' => '<div id="'. $og_entity_type .'_target">',
      // '#suffix' => '</div>',
    );

    $options = array(
      '__none' => t('--Do not sync--'),
    );
    
    // ajax callback based on contact type
    if (!empty($selected_contact_type)) {
       $options += _crm_og_sync_contact_fields($form_state['values'][$og_entity_type]['contact_type']);
    } else {
      $default_contact_type = (isset($default_settings['crm_contact_type'])) ? $default_settings['crm_contact_type'] : 'organization';
      $options += _crm_og_sync_contact_fields($default_contact_type);  
    }
    
    $default_values = unserialize($default_settings['field_mapping']);
    
    $og_entity_type_fields = field_info_instances('node', $og_entity_type);
    foreach($og_entity_type_fields as $field_name => $field) {
      if (empty($default_values)) {
        $_default_value = '__none';
      } else {
        $_default_value = (array_key_exists($field_name, $default_values)) ? $default_values[$field_name] : '__none';
      }
      $form[$og_entity_type]['mapping'][$field_name] = array(
        '#type' => 'select',
        '#title' => $field['label'],
        // need to load the options from the contact type
        '#options' => $options,
        '#default_value' => $_default_value,
      );    
    }  
  }
  
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save configurations'),
  );
  
  return $form;
}

function crm_og_sync_admin_form_ajax_callback($form, &$form_state) {
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
  
  variable_set('crm_og_sync_reverse', $values['crm_og_sync_reverse']);
  
  foreach($og_node_types as $og_entity_type) {
     // save the record
     $record = array(
       'og_bundle_type' => $og_entity_type,
       'og_entity_type' => 'node',
       'crm_contact_type' => $values[$og_entity_type]['contact_type'],
       'status' => $values[$og_entity_type]['toggle'],
       'field_mapping' => $values[$og_entity_type]['mapping'],
     );
     
     crm_og_sync_save_config($record, $values[$og_entity_type]['is_new']);     
  }
  
  drupal_set_message('Configuration settings saved'); 
}
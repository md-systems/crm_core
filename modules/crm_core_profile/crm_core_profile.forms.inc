<?php

/**
 * The main form that will generate the form to collect information
 * This form will be loaded in a default form path or a block
 *
 * @params
 * (array) $crm_core_profile
 * 
 */
function crm_core_profile_entry_form($form, &$form_state, $crm_core_profile) {
  
}

/**
 * Generate a new profile form or edit an existing form
 *
 * @param
 * (int) $id
 * If we are editing an existing form 
 */
function crm_core_profile_new_form($form, &$form_state, $id = NULL) {
  
  $form['label'] = array(
    '#type' => 'textfield',
    '#title' => 'Name',
    '#description' => 'Please enter the profile form name',
    '#maxlength' => 64,
    '#required' => TRUE,
  );

  $field_options = array();
  if (!empty($form_state['values']['bundle_type'])) { 
     foreach(field_info_instances('crm_core_contact', $form_state['values']['bundle_type']) as $field_name => $field) {
      $field_options[$field_name] = $field['label'];
    }  
  }
  
 $form['name'] = array(
    '#type' => 'machine_name',
    '#maxlength' => 64,
    '#machine_name' => array(
      'exists' => 'crm_core_profile_new_form_machine_name_check',
      'source' => array('label'),
    ),
  );
  
  // select bundles from the CRM
  $options = array('' => t('--Select--'));
  foreach(crm_core_contact_types(TRUE) as $type => $contact_type) {
    $options[$type] = $contact_type->name;  
  }
  
  $form['bundle_type'] = array(
    '#type' => 'select',
    '#title' => t('Select the contact type below:'),
    '#options' => $options,
    // depends on the contact bundle chose, different fields will appear
    '#ajax' => array(
      'callback' => 'crm_core_profile_new_form_ajax_callback',
      'wrapper' => 'crm_core_profile_bundle_fields',
     ),
  );
  

  
  $form['fields'] = array(
    '#type' => 'checkboxes',
    '#title' => 'Field selection',
    '#options' => $field_options,
    '#description' => t('Check the contact fields that will appear on the profile form'),
    '#prefix' => '<div id="crm_core_profile_bundle_fields">',
    '#suffix' => '</div>',
  );

  // if path module is enabled, user can also specify the path alias for this form
  if (module_exists('path')) {
    $form['path'] = array(
      '#type' => 'textfield',
      '#title' => 'Path alias',
      '#description' => 'Set a desired path alias for this profile form',
    );    
  }
  
  $form['block'] = array(
    '#type' => 'checkbox',
    '#title' => 'Enable form block',
    '#description' => t('Check this if you want to enable a block for this profile form'),
  );
    
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Create'),
  );
  
  return $form;
}

/**
 * Ajax callback
 */
function crm_core_profile_new_form_ajax_callback($form, $form_state) {
  return $form['fields'];
}

function crm_core_profile_new_form_machine_name_check($value) {
  
}

/**
 * Validation callback
 */
function crm_core_profile_new_form_validate($form, &$form_state) {
  if ($form_state['values']['bundle_type'] == '') {
    form_set_error('contact_type', 'You must choose a contact type to save this profile form');  
  }
    
   // TO TO: check if the machine name alredy exist in the database
  
}

/*
 * Submission callback
 */
function crm_core_profile_new_form_submit($form, &$form_state) {
  
  $record = $form_state['values'];
  unset($record['submit'], $record['form_build_id'], $record['form_token'], $record['form_id'], $record['op']);
  
  crm_core_profile_save($record);
    
}
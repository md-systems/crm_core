<?php

/**
 * The main form that will generate the form to collect information
 * This form will be loaded in a default form path or a block
 *
 * @params
 * (array) $crm_core_profile
 * 
 */
function crm_core_profile_entry_form($form, &$form_state, $profile) {

  drupal_set_title($profile['label']);

  $form_state['profile'] = $profile;

  global $user;

  $fields = array();
  // build the form based on the visibility setting given
   $fields = unserialize($profile['fields']);  
   
  // handle anonymous users
  if ($user->uid === 0) {
     $contact = new stdClass();
     $contact->type = $profile['bundle_type'];
     $contact->uid = 1;       
  }
  else {
    // @todo: optionally we want to load the contact information if it is an auth user, the user sync has to be enabled and configured.
    
//    $related_contact = _crm_user_sync_get_related_entity('user', $user->uid, 'crm_user_sync');
//    if (!empty($related_contact)) {
//        $contact = $related_contact;      
//    }
//    else {
//       $contact = new stdClass();
//       $contact->type = 'individual';
//       $contact->uid = $user->uid;   
//    }
     $contact = new stdClass();
     $contact->type = $profile['bundle_type'];
     $contact->uid = 1;   
  }
  
  $form_state['contact'] = $contact;
   
   field_attach_form('crm_core_contact', $contact, $form, $form_state); 
   foreach ($fields as $field_name => $visibility) {
     if ($visibility === 0) {
       $form[$field_name]['#access'] = FALSE; 
     }
   }
   
   $form['submit'] = array(
     '#type' => 'submit',
     '#value' => t('Submit'),
     '#weight' => 99,
   );
      
   dpm($form);
    
   return $form;

}

/*
 * Validation callback
 */
function crm_core_profile_entry_form_validate($form, &$form_state) {
  // additional validition can be handled here
   $contact = $form_state['contact'];
   field_attach_form_validate('crm_core_contact', $contact, $form, $form_state);

}

/*
 * Submission callback
 */
function crm_core_profile_entry_form_submit($form, &$form_state) {
  $values = $form_state['values'];
  unset($values['submit'], $values['form_build_id'], $values['form_token'], $values['form_id'], $values['op']);
  
  // notify other modules about the profile form has been submitted
  module_invoke_all('crm_core_profile_save', $form_state['profile']['name'], $values);
  
  $contact = &$form_state['contact'];
  
  field_attach_submit('crm_core_contact', $contact, $form, $form_state);
  $saved_contact = crm_core_contact_save($contact);

  
  // rules integration  
  // This should be set from the form
  $form_state['redirect'] = (isset($form_state['profile']['redirect'])) ? $form_state['profile']['redirect'] : '';
  
}


/**
 * Generate a new profile form or edit an existing form
 *
 * @param
 * (int) $id
 * If we are editing an existing form, it is the machine name 
 */
function crm_core_profile_new_form($form, &$form_state, $profile) {
  
  global $user;
  
  $form['is_new'] = array(
    '#type' => 'value',
    '#value' => 0,
  );
  
  if (empty($profile)) {
    $form['is_new']['#value'] = 1;
    
    $form['created'] = array(
      '#type' => 'value',
      '#value' => time(),
    );  
  }
  else {
    drupal_set_title(t('Profile settings for @profile', array('@profile' => $profile['label'])));  
  }
   
  $form['label'] = array(
    '#type' => 'textfield',
    '#title' => 'Name',
    '#description' => 'Please enter the profile form name',
    '#maxlength' => 64,
    '#required' => TRUE,
    '#default_value' => (!empty($profile)) ? $profile['label'] : '',
  );

  $field_options = array();
  if (!empty($profile)) {
    $form_state['values']['bundle_type'] = $profile['bundle_type'];  
  }
  
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
    '#default_value' => (!empty($profile)) ? $profile['name'] : '',
    '#disabled' => (!empty($profile)) ? TRUE : FALSE,
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
     '#default_value' => (!empty($profile)) ? $profile['bundle_type'] : '',
  );
 
  $fields_default = array();
  if (!empty($profile)) {
    $fields_default = unserialize($profile['fields']);  
  }

  $form['fields'] = array(
    '#type' => 'checkboxes',
    '#title' => 'Field selection',
    '#options' => $field_options,
    '#description' => t('Check the contact fields that will appear on the profile form'),
    '#prefix' => '<div id="crm_core_profile_bundle_fields">',
    '#suffix' => '</div>',
    '#default_value' => (!empty($fields_default)) ? $fields_default : array(),
  );

  // if path module is enabled, user can also specify the path alias for this form
  // @todo
  if (module_exists('path')) {
    $form['path'] = array(
      '#type' => 'textfield',
      '#title' => 'Path alias',
      '#description' => 'Set a desired path alias for this profile form',
      '#default_value' => (!empty($profile)) ? $profile['path'] : '',
    );    
  }
  
  $form['redirect'] = array(
    '#type' => 'textfield',
    '#title' => 'Redirect path',
    '#description' => 'The redirect path when the profile form is submitted',
    '#default_value' => (!empty($profile)) ? $profile['redirect'] : '',  
  );
  
  $form['block'] = array(
    '#type' => 'checkbox',
    '#title' => 'Enable form block',
    '#description' => t('Check this if you want to enable a block for this profile form'),
    '#default_value' => (!empty($profile)) ? $profile['block'] : 0,
  );
  
  $form['uid'] = array(
    '#type' => 'value',
    '#value' => $user->uid,
  );
    
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => (!empty($profile)) ? t('Save') : t('Create'),
  );
  
  return $form;
}

/**
 * Ajax callback
 */
function crm_core_profile_new_form_ajax_callback($form, $form_state) {
  return $form['fields'];
}

function crm_core_profile_new_form_machine_name_check($value) {}

/**
 * Validation callback
 */
function crm_core_profile_new_form_validate($form, &$form_state) {
  
  if ($form_state['values']['bundle_type'] == '') {
    form_set_error('contact_type', 'You must choose a contact type to save this profile form');  
  }

  $count = 0;
  foreach ($form_state['values']['fields'] as $values) {
    if ($values === 0) {
      $count++;
    }  
  }
  
  if ($count == count($form_state['values']['fields'])) {
     form_set_error('fields', 'The profile should contain at least 1 field'); 
  }
  
  if (isset($form_state['values']['path']) && $form_state['values']['path'] != '') {
    // check if the path alias exist   
    $path = drupal_lookup_path('source', $form_state['values']['path']);
    if (!empty($path)) {
      if ($path !== 'crm_core_profile/' . $form_state['values']['name']) {
        form_set_error('path', 'the url alias already exist'); 
      }
    }
  }

  if ($form_state['values']['is_new'] == 1) {
    $profile = crm_core_profile_load($form_state['values']['name']);
    if (!empty($profile)) {
      form_set_error('label', 'A profile with the machine name already exists, please enter another name');
    }
  }
  
}

/*
 * Submission callback
 */
function crm_core_profile_new_form_submit($form, &$form_state) {
  
  $record = $form_state['values'];
  unset($record['submit'], $record['form_build_id'], $record['form_token'], $record['form_id'], $record['op']);
  $record['redirect'] = ltrim($record['redirect'], '\/\\');
  
  // save the path alias
  if (isset($form_state['values']['path']) && $form_state['values']['path'] != '') {
    
    $loaded_path = path_load('crm_core_profile/' . $form_state['values']['name']); 
    $path = array(
      'source' => 'crm_core_profile/' . $form_state['values']['name'],
      'alias' => $form_state['values']['path'],
    );
    
    if ($loaded_path) {
      $path['pid'] = $loaded_path['pid'];
    } 
    path_save($path);  
  }
  
  crm_core_profile_save($record);
  
  $form_state['redirect'] = 'admin/structure/crm/profile';
  drupal_set_message('Your profile has been successfully saved');
    
}
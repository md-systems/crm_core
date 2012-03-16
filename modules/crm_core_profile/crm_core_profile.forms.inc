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
  $form['#attributes']['class'] = array($profile['name']);
  $form['profile_name'] = array(
    '#type' => 'value',
    '#value' => $profile['name'],
  );

  global $user;

  $fields = array();
  
  // we have to use the list of fields from the contact bundle rather than from the profile settings because new field(s)
  // may be added to the contact bundle
   $profile_fields = unserialize($profile['fields']);
   $profile_fields_visibility = $profile_fields['toggle'];  
   $contact_fields = field_info_instances('crm_core_contact',$profile['bundle_type']);
   $contact_fields = array_keys($contact_fields);
   foreach ($contact_fields as $field_name) {
     if (!array_key_exists($field_name, $profile_fields_visibility)) {
        $profile_fields_visibility[$field_name] = 0; 
      }  
   }
   
  // handle anonymous users
  if ($user->uid === 0) {
     $contact = new stdClass();
     $contact->type = $profile['bundle_type'];
     $contact->uid = 1;       
  }
  else {
    if (module_exists('crm_user_sync')) {
      $related_contact = _crm_user_sync_get_related_entity('user', $user->uid, 'crm_user_sync'); 
      if (!empty($related_contact)) {
        $contact = $related_contact;  
      }
      else {
      // @todo: user is logged in but does not have an associated contact record (crm_user_sync is enabled)
      $contact = crm_user_sync_sync($user);
      } 
    }
    else {
      $contact = new stdClass();
      $contact->type = $profile['bundle_type'];
      $contact->uid = 1;       
    }  
  }
  
  $form_state['contact'] = $contact;    
   field_attach_form('crm_core_contact', $contact, $form, $form_state); 
   foreach ($profile_fields_visibility as $field_name => $visibility) {
     if ($visibility === 0) {
       $form[$field_name]['#access'] = FALSE; 
     }
   }

    // getting the weight of each element from settings
    // bug: submit button floats above the rest of the form elements
   if (array_key_exists('weight', $profile_fields)) {
     foreach ($profile_fields['weight'] as $field_name => $value) {
       $form[$field_name]['#weight'] = $value['weight'];  
     }  
   }
      
   $form['submit'] = array(
     '#type' => 'submit',
     '#value' => t('Submit'),
     '#weight' => 99,
   );
        
   return $form;
}

/**
 * Theme function for the crm_core_profile form
 */
function theme_crm_core_profile_entry_form($variables) {
  $form = $variables['form'];
  
   $profile = crm_core_profile_load($form['profile_name']['#value']);
   $fields = unserialize($profile['fields']);
  $render_fields = array();
  foreach($fields['weight'] as $field => $weight) {
      $render_fields[$weight['weight']['weight']] = $field;  
  } 

  // call others to add fields to the render fields array in weighted order before we sort the whole render
  // fields array and render them in order
  // $render_fields += module_invoke_all('crm_core_profile_theme', $form);
  drupal_alter('crm_core_profile_theme', $form, $render_fields);
  
  ksort($render_fields);  
  
  $output = '';
  foreach ($render_fields as $field) {
    if(substr($field, 0, 10) == 'activity||') {
      $activity_field = explode('||', $field);
      $output .= drupal_render($form['activity'][$activity_field[1]]);  
    }
    else {
      $output .= drupal_render($form[$field]);  
    }
  }
  
  $output .= drupal_render_children($form);
  
  return $output;
}

/**
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
  module_invoke_all('crm_core_profile_save', $form_state['profile'], $values);
  
  $contact = &$form_state['contact'];
  
  field_attach_submit('crm_core_contact', $contact, $form, $form_state);
  $saved_contact = crm_core_contact_save($contact);
  
  // add this to the form_state values
  $form_state['saved_contact_id'] = $saved_contact;
  
  // fire another hook that carries the saved contact (contact_id) information, this can be used
  // by other modules to create activity or relationships   
  module_invoke_all('crm_core_profile_save_contact', $form_state['profile']['name'], $saved_contact);

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
    $form_state['profile'] = $profile;  
  }
   
  $form['label'] = array(
    '#type' => 'textfield',
    '#title' => 'Name',
    '#description' => 'Please enter the profile form name',
    '#maxlength' => 28,
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
    '#maxlength' => 28,
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
    '#type' => 'container',
    '#tree' => TRUE,
  );

  $form['fields']['toggle'] = array(
    '#type' => 'checkboxes',
    '#title' => 'Field selection',
    '#options' => $field_options,
    '#description' => t('Check the contact fields that will appear on the profile form'),
    '#prefix' => '<div id="crm_core_profile_bundle_fields">',
    '#suffix' => '</div>',
    '#default_value' => (!empty($fields_default['toggle'])) ? $fields_default['toggle'] : array(),
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
    '#weight' => 999,
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
  
  if (isset($form_state['profile'])) {
    $fields = unserialize($form_state['profile']['fields']);
    $record['fields']['weight'] = (isset($fields['weight'])) ? $fields['weight'] : array();
  }
  
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

/**
 * Create a sorting table for saving the weight of each crm_core_profile fields
 */
function crm_core_profile_weight_form($form, &$form_state, $crm_core_profile) {
  
  $form['#tree'] = TRUE;
  
  $form_state['profile'] = $crm_core_profile;
  
  // load all the fields from the crm core profile
  $fields = unserialize($crm_core_profile['fields']);
  
  if (empty($fields)) {
    $form['weight'] = array();
    return $form;
  }
  
  $fields['weight'] = (isset($fields['weight'])) ? $fields['weight'] : array();
  
  $fields_toggle = array();
  // sort the toggles if weight has been assigned
   foreach($fields['weight'] as $field_name => $value) {
     $fields_toggle[$field_name] = $fields['toggle'][$field_name];
   }
  $fields_toggle = (!empty($fields_toggle)) ? $fields_toggle : $fields['toggle'];
  
  foreach($fields_toggle as $field_name => $value) {
      if ($value !== 0) {
        $field = field_info_instance('crm_core_contact', $field_name, $crm_core_profile['bundle_type']);
        $form['weight'][$field_name]['label'] = array('#markup' => check_plain($field['label']));
        $form['weight'][$field_name]['weight'] = array(
         '#type' => 'weight',
         '#title' => t('Weight for @title', array('@title' => $field['label'])),
         '#title_display' => 'invisible',
         '#default_value' => (array_key_exists($field_name, $fields['weight'])) ?  $fields['weight'][$field_name]['weight'] : -10,
    );
    }
  }
    
 $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit'] = array('#type' => 'submit', '#value' => t('Save changes'));
  
  return $form;  
}

function crm_core_profile_weight_form_validate($form, &$form_state) {

 // form_set_error('aaa', 'bbb');
}

/**
 * Submission handler
 * We use this to save the field weights
 */
function crm_core_profile_weight_form_submit($form, &$form_state) {
  $form_state['profile']['fields'] = unserialize($form_state['profile']['fields']);

   foreach ($form_state['values']['weight'] as $field => $weight) {
     if (array_key_exists($field, $form_state['profile']['fields']['toggle'])) {
       if($form_state['profile']['fields']['toggle'][$field] !== 0) {
         $sort_keys[$field] = $weight;
       }  
     }   
   }

   asort($sort_keys);
   // unset($form_state['values']['weight']); 
   foreach($sort_keys as $field_name => $weight) {
     $weights[$field_name]['weight'] = $weight;   
   }
   
   $form_state['profile']['fields']['weight'] = $weights; 

  crm_core_profile_save($form_state['profile']);
  
  drupal_set_message('Your changes have been saved');
}


/**
 * Theme implementation of the weight form
 */
function theme_crm_core_profile_weight_form($variables) {
  $form = $variables['form'];

  $rows = array();
  
  foreach (element_children($form['weight']) as $field_name) {
    $form['weight'][$field_name]['weight']['#attributes']['class'] = array('crm-profile-field-order-weight');
    $rows[] = array(
      'data' => array(
        drupal_render($form['weight'][$field_name]['label']),
        drupal_render($form['weight'][$field_name]['weight']),
      ),
      'class' => array('draggable'),  
    );  
  }
  
  $header = array(t('Field'), t('Weight'),);
  
  $output = theme('table', array('header' => $header, 'rows' => $rows, 'attributes' => array('id' => 'crm-profile-field-order')));
  $output .= drupal_render_children($form);
  
  drupal_add_tabledrag('crm-profile-field-order', 'order', 'sibling', 'crm-profile-field-order-weight');
  
  return $output;
  
}
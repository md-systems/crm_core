<?php

function _crm_core_default_matching_engine_placeholder() {
  
  $text = 'CRM Core Match UI would be here soon!';
  
  
  
  
  
  
  foreach(field_info_instances('crm_core_contact', 'individual') as $field_name => $field) {
    dpm($field_name);
    dpm($field);
    
  }  
  
  
  return $text;
}




function _crm_core_default_matching_engine_form ($form, &$form_state){
  
  $form['enabled'] = 
  
  
  $form['enabled'] = array(
    '#type' => 'checkbox',
    '#title' => 'Enable matching for this contact type',
    '#description' => t('Check this box to allow CRM Core to check for duplicate contact records for this contact type.'),
    '#default_value' => (!empty($form['enabled'])) ? $profile['enabled'] : 0,
  );
  
  
}
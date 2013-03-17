<?php

/**
 * @file
 * Admnistrative screens.
 */

/**
 * Placeholder for matching engine configuration pages
 * @TODO: remove this function, it's no longer needed
 */
function _crm_core_default_matching_engine_placeholder() {
  
  $text = 'CRM Core Match UI would be here soon!';
  
  foreach(field_info_instances('crm_core_contact', 'individual') as $field_name => $field) {
    dpm($field_name);
    dpm($field);
  }  
  
  return $text;
}

/**
 * Creates a form for customizing the match rules for an individual contact type.
 * @param unknown_type $form
 * @param unknown_type $form_state
 */
function crm_core_default_matching_engine_form ($form, &$form_state, $contact_type){
  
  $form['enabled'] = array(
    '#type' => 'checkbox',
    '#title' => 'Enable matching for this contact type',
    '#description' => t('Check this box to allow CRM Core to check for duplicate contact records for this contact type.'),
    '#default_value' => (!empty($form['enabled'])) ? $form['enabled'] : 0,
  );

  $form['strict'] = array(
    '#type' => 'checkbox',
    '#title' => 'Strict matching',
    '#description' => t('Check this box to return a match for this contact type the first time one is identified that meets the threshold. Stops rednudant processing.'),
    '#default_value' => (!empty($form['strict'])) ? $form['strict'] : 0,
  );
  
  $form['label'] = array(
    '#type' => 'textfield',
    '#title' => 'Threshold',
    '#description' => 'Defines the score at which a contact is considered a match.',
    '#maxlength' => 28,
    '#required' => TRUE,
    '#default_value' => (!empty($form['threshold'])) ? $form['threshold'] : '',
  );
  
  $form['fields'] = array(
    '#type' => 'item',
    '#title' => t('Field Matching'),
  );
  
  $form['field_matching'] = array(
    '#type' => 'container'
  );
  
  // dpm(field_info_instance('crm_core_contact', 'contact_name', 'individual'));
  
  /**
   * This returns a list of all fields associated with the selected contact type
   */
  foreach(field_info_instances('crm_core_contact', $contact_type) as $field_name => $field) {
    
    // dpm($field_name);
    // dpm($field);
    
    /**
     * This returns some additional information about the field
     */
    $field_info = field_info_field($field_name);
    
    /**
     * We now have enough information about the field to render a row
     * for it. Some fields are going to have multiple rows (for instance,
     * name fields) and need to be handled a little differently.
     */
    
    // so, check to see if there is a handler for the field
    
    
    // once we know the field type, we need to load some information about the fieldtype
    // in order to allow the proper operators to appear.
    
    // dpm($field_info);
    
    // we want to check if there is an include file with a class with the same name.
    // if so, great, we are ready to rock! If not, we will have to generate some default data
    
    $fieldHandler = $field_info['type'] . 'MatchField';
    
    if(class_exists($fieldHandler)){
      
      // @TODO: need to modify interface and classes to accept field and field info
      // need to pass in the form array by reference and allow it to be modified
      
      $obj = new $fieldHandler();
      $obj->fieldRender();
      
      // @TODO: need to add something that adds the output of fieldRender to the form
      // the output of fieldrender can be one field, or it can be many
      
      
    } else {

      $form['field_matching'][$field_name]['check'] = array(
        '#type' => 'checkbox',
        '#title' => '',
      );
      $form['field_matching'][$field_name]['name'] = array(
        '#type' => 'item',
        '#markup' => $field['label'],
      );
      $form['field_matching'][$field_name]['fieldtype'] = array(
        '#type' => 'item',
        '#markup' => $field_info['type'],
      );
      
      // @TODO: add a selector for the logical operator
      $form['field_matching'][$field_name]['operator'] = array(
        '#type' => 'item',
        '#markup' => '---',
      );
      
      // @TODO: add a textfield for any extra information that's needed
      $form['field_matching'][$field_name]['options'] = array(
        '#type' => 'item',
        '#markup' => '---',
      );
      
      // @TODO: add a textfield for any extra information that's needed
      $form['field_matching'][$field_name]['score'] = array(
        '#type' => 'textfield',
        '#title' => 'Score',
        '#maxlength' => 28,
        '#title_display' => 'invisible',
        '#default_value' => 0,
      );
      
      // @TODO: get the actual weight as the default value
      $form['field_matching'][$field_name]['weight'] = array(
        '#type' => 'weight',
        '#title_display' => 'invisible',
        '#default_value' => 0,
        '#attributes' => array(
          'class' => array('crm-core-match-engine-order-weight'),
        ),
      );
      
    }
      
    
  }  
  
  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
  );

  
  return $form;
  
}



/**
 * Theme implementation of the crm_core_match_admin_config_engines_form form.
 */
function theme_crm_core_default_matching_engine_form($variables) {
  $form = $variables['form'];

  $rows = array();
  foreach (element_children($form['field_matching']) as $key) {
    $rows[] = array(
      'data' => array(
        drupal_render($form['field_matching'][$key]['check']),
        drupal_render($form['field_matching'][$key]['name']),
        drupal_render($form['field_matching'][$key]['fieldtype']),
        drupal_render($form['field_matching'][$key]['operator']),
        drupal_render($form['field_matching'][$key]['options']),
        drupal_render($form['field_matching'][$key]['score']),
        drupal_render($form['field_matching'][$key]['weight']),
      ),
      'class' => array('draggable'),
    );
  }

  $header = array(t('Enabled'), t('Name'), t('Field Type'), t('Operator'), t('Options'), t('Score'), t('Weight'));
  $form['field_matching']['content']['#markup'] = theme('table', array(
    'header' => $header,
    'rows' => $rows,
    'attributes' => array('id' => 'crm-core-match-engine-order')
  ));
  $output = drupal_render_children($form);

  drupal_add_tabledrag('crm-core-match-engine-order', 'order', 'sibling', 'crm-core-match-engine-order-weight');

  return $output;
}



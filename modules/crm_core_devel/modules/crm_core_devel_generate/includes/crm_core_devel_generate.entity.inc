<?php

/**
 * Generates nodes using FormAPI.
 */
function crm_core_devel_generate_content_form() {  
  $entities = array();
 
  $entity_type = entity_get_info('crm_core_contact');  
  if (!empty($entity_type) && isset($entity_type['bundles'])) {
    foreach ($entity_type['bundles'] as $bundle_name => $bundle) {      
      $entities[$bundle_name] = t($bundle['label']);
    }

    $form['bundle_types'] = array(
      '#type' => 'checkboxes',
      '#title' => 'Contact type',        
      '#options' => $entities,
      '#default_value' => array_keys($entities),
    );
  }

  if (empty($entities)) {
    drupal_set_message(t('You do not have any content types that can be generated. <a href="@create-type">Go create a new content type</a> already!</a>', array('@create-type' => url('admin/structure/types/add'))), 'error', FALSE);
    return;
  }

  if (module_exists('crm_core_activity')) {
    $form['generate_activity'] = array(
      '#type' => 'fieldset',   
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
      '#title' => t('Generate activities'),    
    );

    $options = array();
    $activities = field_info_bundles('crm_core_activity');
    foreach ($activities as $activity_name => $activity) {
      $options[$activity_name] = $activity['label'];
    }

    $form['generate_activity']['activity_types'] = array(
      '#type' => 'checkboxes',
      '#title' => 'Activity type',        
      '#options' => $options,      
    );

    $form['generate_activity']['num_acts'] = array(
      '#type' => 'textfield',
      '#title' => t('How many activities per user would you like to generate?'),
      '#default_value' => 2,
      '#size' => 10,
    );

    $form['generate_activity']['kill_acts'] = array(
      '#type' => 'checkbox',
      '#title' => t('Delete existing activities before generating new ones.'),
      '#default_value' => FALSE,
    );
  }

  if (module_exists('crm_core_relationship')) {
    $form['generate_relations'] = array(
      '#type' => 'fieldset',   
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
      '#title' => t('Generate relations'),    
    );  
  
    $form['generate_relations']['relation_types'] = array(
      '#type' => 'container',
      '#tree' => TRUE,
    );

    foreach ($entities as $entity_name => $entity_label) {    
      $options = array();
      $relations = crm_core_relationship_load_relationship_types($entity_name);
      foreach ($relations as $key => $relation) {
        $options[$relation->relation_type] = $relation->label;
     }
     if (count($options)) {
        $form['generate_relations']['relation_types'][$entity_name] = array(
          '#type' => 'checkboxes',
          '#title' => 'Relation type for ' . $entity_label,
          '#options' => $options,          
        );
      }
    }

    $form['generate_relations']['num_rels'] = array(
      '#type' => 'textfield',
      '#title' => t('How many relations per user would you like to generate?'),
      '#default_value' => 2,
      '#size' => 10,
    );

    $form['generate_relations']['kill_rels'] = array(
      '#type' => 'checkbox',
      '#title' => t('Delete existing relations before generating new ones.'),
      '#default_value' => FALSE,
    );
  }
  

  $form['kill_content'] = array(
    '#type' => 'checkbox',
    '#prefix' => t('<br/><b>Delete all content</b>'),
    '#title' => t('in these content types before generating new content.'),
    '#default_value' => FALSE,
  );

  $form['num_contacts'] = array(
    '#type' => 'textfield',
    '#title' => t('How many items of content would you like to generate?'),
    '#default_value' => 50,
    '#size' => 10,
  );

  $options = array(1 => t('Now'));
  foreach (array(3600, 86400, 604800, 2592000, 31536000) as $interval) {
    $options[$interval] = format_interval($interval, 1) . ' ' . t('ago');
  }

  $form['time_range'] = array(
    '#type' => 'select',
    '#title' => t('How far back in time should the content items be dated?'),
    '#description' => t('Content creation dates will be distributed randomly from the current time, back to the selected time.'),
    '#options' => $options,
    '#default_value' => 604800,
  );

  $form['title_length'] = array(
    '#type' => 'textfield',
    '#title' => t('Max word length of titles'),
    '#default_value' => 4,
    '#size' => 10,
  );

  unset($options);

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Generate'),
  );
  $form['#redirect'] = FALSE;

  return $form;
}

/**
 * FormAPI submission to generate nodes.
 */
function crm_core_devel_generate_content_form_submit($form_id, &$form_state) {
  module_load_include('inc', 'devel_generate', 'devel_generate');    
  $form_state['values']['bundle_types'] = array_filter($form_state['values']['bundle_types']);
  $form_state['values']['activity_types'] = array_filter($form_state['values']['activity_types']);
  if ($form_state['values']['num_contacts'] <= 50) {
    module_load_include('inc', 'crm_core_devel_generate', 'devel_generate');
    crm_core_devel_generate_content($form_state);
  }
  else {
    module_load_include('inc', 'crm_core_devel_generate', 'devel_generate_batch');
    crm_core_devel_generate_batch_content($form_state);
  }
}

/**
 * Pre-process the crm_core_devel_generate_content_form request. This is needed so
 * batch api can get the list of users once.
 *
 * @param $num
 *   array of options obtained from crm_core_devel_generate_content_form.
 */
function crm_core_devel_generate_contacts_pre_contact(&$results) {
  module_load_include('inc', 'devel_generate');
  // Get user id.
  $users = devel_get_users();
  $users = array_merge($users, array('0'));
  $results['users'] = $users;
}
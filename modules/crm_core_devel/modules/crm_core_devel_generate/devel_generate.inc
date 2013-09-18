<?php

/**
 * The main API function for creating contacts.
 *
 * See crm_core_devel_generate_content_form() for the supported keys in $form_state['values'].
 * Other modules may participate by form_alter() on that form and then handling their data during presave or in own submit handler for the form.
 *
 * @param string $form_state
 * @return void
 */
function crm_core_devel_generate_content($form_state) {
  if (!empty($form_state['values']['kill_content'])) {
    crm_core_devel_generate_contact_kill($form_state['values']);
  }

  if (count($form_state['values']['bundle_types'])) {
    crm_core_devel_generate_contacts_pre_contact($form_state['values']);
    $start = time();
    for ($i = 1; $i <= $form_state['values']['num_contacts']; $i++) {
      // Get list of users we will use.
      $contact = crm_core_devel_generate_contact_add_contact($form_state['values']);
      if (function_exists('drush_log') && $i % drush_get_option('feedback', 1000) == 0) {
        $now = time();
        drush_log(dt('Completed !feedback contacts (!rate contact/min)', array('!feedback' => drush_get_option('feedback', 1000), '!rate' => (drush_get_option('feedback', 1000)*60)/($now-$start))), 'ok');
        $start = $now;
      }

      // Create activities
      if (count($form_state['values']['activity_types'])) {
        $form_state['values']['contact'] = $contact;
        foreach ($form_state['values']['activity_types'] as $type) {    
          for ($j = 1; $j <= $form_state['values']['num_acts']; $j++) {         
            crm_core_devel_generate_contact_add_activity($form_state['values'], $type);
          }
        }
      }
   
      // Create relationships
    }
  }

  // Include devel here to set this message
  //devel_generate_set_message(format_plural($form_state['values']['num_contacts'], '1 contact created.', 'Finished creating @count contacts'));
  drupal_set_message(format_plural($form_state['values']['num_contacts'], '1 contact created.', 'Finished creating @count contacts'));
}

/**
 * Handle the crm_core_devel_generate_content_form request to kill all of the
 * existing contact.
 *
 * This is used by both the batch and non-batch branches of the code.
 *
 * @param $values
 *   array of options obtained from crm_core_devel_generate_content_form.
 */
function crm_core_devel_generate_contact_kill($values) {
  $bundles = $values['bundle_types'];
  $op = 'IN';
  if (empty($bundles)) {
    $bundles = '%';
    $op = 'LIKE';
  }
  $results = db_select('crm_core_contact', 'c')
              ->fields('c', array('contact_id'))
              ->condition('type', $bundles, $op)
              ->execute();
  foreach ($results as $result) {
    $contact_ids[] = $result->contact_id;  
    if (!empty($values['kill_acts'])) {
      $values['contact'] = crm_core_contact_load($result->contact_id);
      crm_core_devel_generate_activity_kill($values);
    } 
  }

  if (!empty($contact_ids)) {
    crm_core_contact_delete_multiple($contact_ids);
    drupal_set_message(t('Deleted %count contact.', array('%count' => count($contact_ids))));
  }
}


/**
 * Handle the crm_core_devel_generate_content_form request to kill all of the
 * existing contact.
 *
 * This is used by both the batch and non-batch branches of the code.
 *
 * @param $values
 *   array of options obtained from crm_core_devel_generate_content_form.
 */
function crm_core_devel_generate_activity_kill($values) {
  $bundles = $values['activity_types'];
  $op = 'IN';
  if (empty($bundles)) {
    $bundles = '%';
    $op = 'LIKE';
  }
  $results = db_select('crm_core_activity', 'c')
              ->fields('c', array('activity_id'))
              ->condition('uid', $values['contact']->uid)
              ->condition('type', $bundles, $op)
              ->execute();
  foreach ($results as $result) {
    $activity_ids[] = $result->activity_id;    
  }

  if (!empty($activity_ids)) {
    crm_core_activity_delete_multiple($activity_ids);
    drupal_set_message(t('Deleted %count activities.', array('%count' => count($activity_ids))));
  }
}

/**
 * Create one contact. Used by both batch and non-batch code branches.
 *
 * @param $num
 *   array of options obtained from crm_core_devel_generate_content_form. 
 */
function crm_core_devel_generate_contact_add_contact(&$results) {
  $type = array_rand($results['bundle_types']);
  $values = array('type' => $type);
  $contact = entity_create('crm_core_contact', $values);

  // Insert new data:
  // $contact->title = devel_create_greeking(mt_rand(2, $results['title_length']), TRUE);

  $contact->created = REQUEST_TIME - mt_rand(0, $results['time_range']);

    // Save default parameters back into the $contact object. 
  $contact->status = TRUE;  
  devel_generate_set_language($results, $contact);

  $contact->uid = 1;
  module_load_include('inc', 'devel_generate', 'devel_generate.fields');
  devel_generate_fields($contact, 'crm_core_contact', $contact->type);

  // Save new contact
  return crm_core_contact_save($contact);
}


/**
 * Create one activity. Used by both batch and non-batch code branches.
 *
 * @param $num
 *   array of options obtained from crm_core_devel_generate_content_form.
 *
 * @param $type
 *  type of activity entity
 */
function crm_core_devel_generate_contact_add_activity(&$results, $type) {  
  $values = array('type' => $type);
  $activity = entity_create('crm_core_activity', $values);

  // Insert new data:
  $activity->title = devel_create_greeking(mt_rand(2, $results['title_length']), TRUE);

  $activity->created = REQUEST_TIME - mt_rand(0, $results['time_range']);

  // Save default parameters back into the $contact object. 
  $activity->status = TRUE;  
  devel_generate_set_language($results, $activity);
 
  $activity->uid = $results['contact']->uid;

  module_load_include('inc', 'devel_generate', 'devel_generate.fields');
  devel_generate_fields($activity, 'crm_core_activity', $activity->type);  

  // Save new activity
  return crm_core_activity_save($activity);
}
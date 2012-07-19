<?php

/**
 * Admin form for User Synchronization
 */
function crm_user_sync_admin($form_state) {
  $form = array();

  $roles = user_roles(TRUE);
  $types = crm_core_contact_types();
 
  $form['crm_user_sync_roles'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Roles to enable User Synchronization.'),
    '#options' => $roles,
    '#default_value' => variable_get('crm_user_sync_roles', array()),
  );

  $types_options = array();
  foreach ($types as $key => $type) {
    $types_options[$key] = $type->name;
  }

  if (empty($types[$key])) {
    $form[] = array('#markup' => t('You need at least a Contact type to enable User Synchronization'));
  }
  else {
    foreach ($roles as $rid => $role) {
      $form['crm_user_sync_' . $rid . '_type'] = array(
        '#type' => 'select',
        '#title' => t('Contact type to use for role @role', array('@role' => $role)),
        '#options' => $types_options,
        '#states' => array(
          'visible' => array(
            'input[name="crm_user_sync_roles[' . $rid . ']"]' => array('checked' => TRUE),
          ),
        ),
        '#default_value' => variable_get('crm_user_sync_' . $rid . '_type', isset($types_options['individual']) ? 'individual' : NULL),
      );
    }
  }

  $form['crm_user_sync_auto_sync_user_create'] = array(
    '#type' => 'checkbox',
    '#title' => t('Automatically create an associated contact when account is created'),
    '#description' => t('Disable this option if you want to manage synchronization within custom modules.'),
    '#default_value' => variable_get('crm_user_sync_auto_sync_user_create', 1),
  );

  $form['#submit'][] = 'crm_user_sync_admin_submit';

  return system_settings_form($form);
}

/**
 * Validation callback.
 */
function crm_user_sync_admin_validate($form, &$form_state) {
  // we need to make sure that no single contact type is selected to match multiple roles
  $errors = array();
  $values = $form_state['values'];
  foreach($values['crm_user_sync_roles'] as $rid => $status) {
    if ($status != 0) {
      $contact_type_sync[$rid] = $values['crm_user_sync_' . $rid . '_type'];  
    }
  }
  $value_count = array_count_values($contact_type_sync);
  foreach($value_count as $contact_type => $count) {
    if ($count > 1) {
       $errors[$contact_type] = $contact_type;
       
    }
  }
  if (!empty($errors)) {
    foreach($contact_type_sync as $rid => $contact_type) {
      if (array_key_exists($contact_type, $errors)) {
        form_set_error('crm_user_sync_' . $rid . '_type', t('@contact_type has been mapped to more than 1 user roles', array('@contact_type' => $contact_type)));         
      }  
    }       
  }
}

/**
 * Submit callback. Handles synchronization changes.
 */
function crm_user_sync_admin_submit($form, &$form_state) {
  // we shouldn't use the submission handler to perform bulk operations, it can potentially be messy,
  // instead we will let the user hooks to do the works
}

/**
 * Submit callback. Handles synchronization changes.
 */
/* 
function crm_user_sync_admin_submit($form, &$form_state) {
  // @TODO: This needs to be thinked through - There are many cases here.
  
  // Check for new roles being sync'd
  foreach ($form_state['values']['crm_user_sync_roles'] as $rid => $value) {
    if ($value != $form['crm_user_sync_roles']['#default_value'][$rid]) {
      // Two cases here:
      if ($value) {
        // Role wasn't sync'd and now it is going to be.
        // For now we just sync all users and let syncing function takes 
        // decisions.
        $query = db_select('users');
        $query->leftJoin('users_roles', 'ur', 'users.uid = ur.uid');
        $query->fields('users', array('uid'));
        $query->condition('users.uid', '0', '<>');
        if ($rid == 2) {
          $query->isNull('ur.rid');
        }
        else {
          $query->condition('rid', $rid);
        }
        $result = $query->execute();
        $ops = array();
        foreach (array_keys($result->fetchAllKeyed()) as $uid) {
          $ops[] = array(
            'crm_user_sync_sync' => array(user_load($uid)),
          );
        }
        batch_set(array('operations' => $ops));
      }
      else {
        // Role was sync'd and now it's not goin to be anymore
      }
    }
    elseif ($value && ($form_state['values']['crm_user_sync_' . $rid . '_type'] != $form['crm_user_sync_' . $rid . '_type']['#default_value'])) {
      // Role had the contact type changed.
    }
  }

  
  // dsm($form);
  // dsm($form_state);
}*/

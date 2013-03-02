<?php

/**
 * Admin form for User Synchronization
 */
function crm_user_sync_admin_form($form_state) {
  $form = array();

  $roles = user_roles(TRUE);
  $types = crm_core_contact_types();

  $types_options = array();
  foreach ($types as $key => $type) {
    $types_options[$key] = $type->name;
  }

  $settings = variable_get('crm_user_sync_settings', array());

  // TODO: remove this
  $settings = array(
    array('rid' => 2, 'contact_type' => 'individual', 'enabled' => true, 'weight' => 1),
    array('rid' => 3, 'contact_type' => 'organization', 'enabled' => false, 'weight' => 1)
  );

  $form['crm_user_sync_description'] = array(
    '#markup' => t('CRM Core User Synchronization can automatically create contact records associated with user accounts under certain conditions.')
  );

  $form['crm_user_sync_settings'] = array(
    '#type' => 'container'
  );

  foreach ($settings as $id => $setting) {
    $form['crm_user_sync_settings'][$id]['role'] = array('#markup' => $roles[$setting['rid']]);
    $form['crm_user_sync_settings'][$id]['contact_type'] = array('#markup' => $types_options[$setting['contact_type']]);
    $form['crm_user_sync_settings'][$id]['enabled'] = array('#markup' => $setting['enabled'] ? 'Yes' : 'No');

    $form['crm_user_sync_settings'][$id]['weight'] = array(
      '#type' => 'weight',
      '#title_display' => 'invisible',
      '#default_value' => $setting['weight'],
    );
  }

  $form['crm_user_sync_auto_sync_user_create'] = array(
    '#type' => 'checkbox',
    '#title' => t('Automatically create an associated contact when account is created'),
    '#description' => t('When checked, this checkbox will automatically create new contacts when a new user account is created according to rules listed above. Rules will be processed in order until a new contact is created.'),
    '#default_value' => variable_get('crm_user_sync_auto_sync_user_create', 1),
  );

  $form['#submit'][] = 'crm_user_sync_admin_form_submit';

  return $form;
}

/**
 * Validation callback.
 */
function crm_user_sync_admin_form_validate($form, &$form_state) {
  // we need to make sure that no single contact type is selected to match multiple roles
  $errors = array();
  $values = $form_state['values'];
  foreach ($values['crm_user_sync_roles'] as $rid => $status) {
    if ($status != 0) {
      $contact_type_sync[$rid] = $values['crm_user_sync_' . $rid . '_type'];  
    }
  }
  $value_count = array_count_values($contact_type_sync);
  foreach ($value_count as $contact_type => $count) {
    if ($count > 1) {
       $errors[$contact_type] = $contact_type;
       
    }
  }
  if (!empty($errors)) {
    foreach ($contact_type_sync as $rid => $contact_type) {
      if (array_key_exists($contact_type, $errors)) {
        form_set_error('crm_user_sync_' . $rid . '_type', t('@contact_type has been mapped to more than 1 user roles', array('@contact_type' => $contact_type)));         
      }  
    }       
  }
}

/**
 * Submit callback. Handles synchronization changes.
 */
function crm_user_sync_admin_form_submit($form, &$form_state) {
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


/**
 * Theme implementation of the weight form
 */
function theme_crm_user_sync_admin_form($variables) {
  $form = $variables['form'];

  $rows = array();

  foreach (element_children($form['crm_user_sync_settings']) as $field_name) {
    $form['crm_user_sync_settings'][$field_name]['weight']['#attributes']['class'] = array('crm-user-sync-settings-order-weight');
    $rows[] = array(
      'data' => array(
        drupal_render($form['crm_user_sync_settings'][$field_name]['role']),
        drupal_render($form['crm_user_sync_settings'][$field_name]['contact_type']),
        drupal_render($form['crm_user_sync_settings'][$field_name]['enabled']),
        drupal_render($form['crm_user_sync_settings'][$field_name]['weight']),
      ),
      'class' => array('draggable'),  
    );
  }

  $header = array(t('Role'), t('Contact Type'), t('Enabled'), t('Weight'),);
  $form['crm_user_sync_settings']['content']['#markup'] = theme('table', array('header' => $header, 'rows' => $rows, 'attributes' => array('id' => 'crm-user-sync-settings-order')));
  $output = drupal_render_children($form);

  drupal_add_tabledrag('crm-user-sync-settings-order', 'order', 'sibling', 'crm-user-sync-settings-order-weight');

  return $output;
}

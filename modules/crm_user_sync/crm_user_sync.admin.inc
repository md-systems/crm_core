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

  $form['crm_user_sync_description'] = array(
    '#markup' => t('CRM Core User Synchronization can automatically create contact records associated with user accounts under certain conditions.')
  );

  $form['#tree'] = TRUE;
  $form['crm_user_sync_settings'] = array(
    '#type' => 'container'
  );

  $settings = variable_get('crm_user_sync_settings', array());
  foreach ($settings as $key => $setting) {
    $form['crm_user_sync_settings'][$key]['role'] = array('#markup' => $roles[$setting['rid']]);
    $form['crm_user_sync_settings'][$key]['contact_type'] = array('#markup' => $types_options[$setting['contact_type']]);
    $form['crm_user_sync_settings'][$key]['enabled'] = array('#markup' => $setting['enabled'] ? 'Yes' : 'No');
    $form['crm_user_sync_settings'][$key]['weight'] = array(
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

  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit'] = array('#type' => 'submit', '#value' => t('Save changes'));

  return $form;
}

/**
 * Validation callback.
 */
function crm_user_sync_admin_form_validate($form, &$form_state) {

}

/**
 * Submit callback. Handles synchronization changes.
 */
function crm_user_sync_admin_form_submit($form, &$form_state) {
  $settings = variable_get('crm_user_sync_settings', array());

  // TODO: remove this
  /*
  $settings = array(
    array('rid' => 2, 'contact_type' => 'individual', 'enabled' => true, 'weight' => 1),
    array('rid' => 3, 'contact_type' => 'organization', 'enabled' => false, 'weight' => 1)
  );
  */

  foreach ($form_state['values']['crm_user_sync_settings']as $key => $values) {
    if (!empty($values['weight'])) {
      $settings[$key]['weight'] = $values['weight'];
    }
  }

  uasort($settings, 'crm_user_sync_weight_cmp');

  variable_set('crm_user_sync_settings', $settings);

  variable_set('crm_user_sync_auto_sync_user_create', $form_state['values']['crm_user_sync_auto_sync_user_create']);

  drupal_set_message('Your changes have been saved');
}

/**
 * Theme implementation of the weight form
 */
function theme_crm_user_sync_admin_form($variables) {
  $form = $variables['form'];

  $rows = array();

  foreach (element_children($form['crm_user_sync_settings']) as $key) {
    $form['crm_user_sync_settings'][$key]['weight']['#attributes']['class'] = array('crm-user-sync-settings-order-weight');
    $rows[] = array(
      'data' => array(
        drupal_render($form['crm_user_sync_settings'][$key]['role']),
        drupal_render($form['crm_user_sync_settings'][$key]['contact_type']),
        drupal_render($form['crm_user_sync_settings'][$key]['enabled']),
        drupal_render($form['crm_user_sync_settings'][$key]['weight']),
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

/**
 * Weight comparison function
 */
function crm_user_sync_weight_cmp($a, $b) {
  if ($a['weight'] == $b['weight']) {
    return 0;
  }
  return ($a['weight'] < $b['weight']) ? -1 : 1;
}



<?php

/**
 * Admin form.
 */
function crm_core_user_sync_admin_form($form, &$form_state) {
  $form = array();

  $roles = user_roles(TRUE);
  $types = crm_core_contact_types();

  $types_options = array();
  foreach ($types as $key => $type) {
    $types_options[$key] = $type->name;
  }

  $rules = variable_get('crm_core_user_sync_rules', array());
  uasort($rules, 'crm_core_user_sync_weight_cmp');

  $form['crm_core_user_sync_description'] = array(
    '#markup' => t('CRM Core User Synchronization can automatically create contact records associated with user accounts under certain conditions.')
  );

  $form['crm_core_user_sync_rules'] = array(
    '#type' => 'container'
  );

  $form['#tree'] = TRUE;
  foreach ($rules as $key => $rule) {
    $form['crm_core_user_sync_rules'][$key]['role'] = array('#markup' => $roles[$rule['rid']]);
    $form['crm_core_user_sync_rules'][$key]['contact_type'] = array('#markup' => $types_options[$rule['contact_type']]);
    $form['crm_core_user_sync_rules'][$key]['enabled'] = array('#markup' => $rule['enabled'] ? 'Yes' : 'No');
    $form['crm_core_user_sync_rules'][$key]['weight'] = array(
      '#type' => 'weight',
      '#title_display' => 'invisible',
      '#default_value' => $rule['weight'],
    );

    $form['crm_core_user_sync_rules'][$key]['operations'] = array(
      '#theme' => 'links',
      '#links' => array(),
      '#attributes' => array('class' => array('links', 'inline')),
    );

    $links = &$form['crm_core_user_sync_rules'][$key]['operations']['#links'];
    $path = 'admin/config/crm-core/user_sync/' . $key;
    $links['edit'] = array('title' => 'Edit', 'href' => $path . '/edit');
    $links['delete'] = array('title' => 'Delete', 'href' => $path . '/delete');

    if  ($rule['enabled']) {
      $links['disable'] = array('title' => 'Disable', 'href' =>$path . '/disable');
    }
    else {
      $links['enable'] = array('title' => 'Enable', 'href' => $path . '/enable');
    }
  }

  $form['crm_core_user_sync_auto_sync_user_create'] = array(
    '#type' => 'checkbox',
    '#title' => t('Automatically create an associated contact when account is created'),
    '#description' => t('When checked, this checkbox will automatically create new contacts when a new user account is created according to rules listed above. Rules will be processed in order until a new contact is created.'),
    '#default_value' => variable_get('crm_core_user_sync_auto_sync_user_create', 1),
  );

  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit'] = array('#type' => 'submit', '#value' => t('Save changes'));

  return $form;
}

/**
 * Validation callback.
 */
function crm_core_user_sync_admin_form_validate($form, &$form_state) {

}

/**
 * Submit callback. Handles synchronization changes.
 */
function crm_core_user_sync_admin_form_submit($form, &$form_state) {
  $rules = variable_get('crm_core_user_sync_rules', array());

  foreach ($form_state['values']['crm_core_user_sync_rules']as $key => $values) {
    if (!empty($values['weight'])) {
      $rules[$key]['weight'] = $values['weight'];
    }
  }

  uasort($rules, 'crm_core_user_sync_weight_cmp');

  variable_set('crm_core_user_sync_rules', $rules);

  variable_set('crm_core_user_sync_auto_sync_user_create', $form_state['values']['crm_core_user_sync_auto_sync_user_create']);

  drupal_set_message('Your changes have been saved');
}

/**
 * Theme implementation of the crm_core_user_sync_settings form.
 */
function theme_crm_core_user_sync_admin_form($variables) {
  $form = $variables['form'];

  $rows = array();

  foreach (element_children($form['crm_core_user_sync_rules']) as $key) {
    $form['crm_core_user_sync_rules'][$key]['weight']['#attributes']['class'] = array('crm-user-sync-settings-order-weight');
    $rows[] = array(
      'data' => array(
        drupal_render($form['crm_core_user_sync_rules'][$key]['role']),
        drupal_render($form['crm_core_user_sync_rules'][$key]['contact_type']),
        drupal_render($form['crm_core_user_sync_rules'][$key]['enabled']),
        drupal_render($form['crm_core_user_sync_rules'][$key]['weight']),
        drupal_render($form['crm_core_user_sync_rules'][$key]['operations']),
      ),
      'class' => array('draggable'),
    );
  }

  $header = array(t('Role'), t('Contact Type'), t('Enabled'), t('Weight'), t('Operations'));
  $form['crm_core_user_sync_rules']['content']['#markup'] = theme('table', array('header' => $header, 'rows' => $rows, 'attributes' => array('id' => 'crm-user-sync-settings-order')));
  $output = drupal_render_children($form);

  drupal_add_tabledrag('crm-user-sync-settings-order', 'order', 'sibling', 'crm-user-sync-settings-order-weight');

  return $output;
}

/**
 * Add new rule form.
 */
function crm_core_user_sync_admin_edit_rule_form($form, &$form_state, $rule_key = null) {
  $form = array();

  $roles = array('' => t('- Select -')) + user_roles(TRUE);
  $types = crm_core_contact_types();

  $types_options = array('' => t('- Select -'));
  foreach ($types as $key => $type) {
    $types_options[$key] = $type->name;
  }

  $rules = variable_get('crm_core_user_sync_rules', array());

  if (isset($rule_key) && isset($rules[$rule_key])) {
    $form_state['rule'] = $rule = $rules[$rule_key];
    $form_state['rule_key'] = $rule_key;
    drupal_set_title(t('Edit user syncrhonzation rule'));
  }
  else {
    drupal_set_title(t('Add user syncrhonzation rule'));
  }

  $form['rid'] = array(
    '#type' => 'select',
    '#title' => t('User Role'),
    '#options' => $roles,
    '#default_value' => isset($rule['rid']) ? $rule['rid'] : '',
    '#required' => true,
  );

  $form['contact_type'] = array(
    '#type' => 'select',
    '#title' => t('Contact Type'),
    '#options' => $types_options,
    '#default_value' => isset($rule['contact_type']) ? $rule['contact_type'] : '',
    '#required' => true,
  );

  $form['weight'] = array(
    '#type' => 'weight',
    '#title' => t('Weight'),
    '#default_value' => isset($rule['weight']) ? $rule['weight'] : 0,
  );

  $form['enabled'] = array(
    '#type' =>'checkbox',
    '#title' => t('Enabled'),
    '#default_value' => isset($rule['enabled']) ? $rule['enabled'] : true,
    '#description' => 'When checked, this rule will be used to synchronize user accounts. When unchecked, it will be ignored throughout the system.'
  );

  $form['actions'] = array('#type' => 'actions');
  $form['actions']['save'] = array('#type' => 'submit', '#value' => t('Save'));
  $form['actions']['cancel'] = array('#type' => 'submit', '#value' => t('Cancel'));

  return $form;
}

/**
 * Submit handler for add new rule form.
 */
function crm_core_user_sync_admin_edit_rule_form_submit($form, &$form_state) {
  if ($form_state['values']['op'] == $form['actions']['save']['#value']) {
    $rules = variable_get('crm_core_user_sync_rules', array());

    $rule = array(
      'rid' => $form_state['values']['rid'],
      'contact_type' => $form_state['values']['contact_type'],
      'weight' => $form_state['values']['weight'],
      'enabled' => $form_state['values']['enabled'],
    );

    if (isset($form_state['rule_key'])) {
      $rules[$form_state['rule_key']] = $rule;
    }
    else {
      $rules[] = $rule;
    }

    uasort($rules, 'crm_core_user_sync_weight_cmp');
    variable_set('crm_core_user_sync_rules', $rules);

    $form_state['redirect'] = 'admin/config/crm-core/user_sync';
    drupal_set_message(t('User syncrhonzation rule has been saved.'));
  }
  elseif ($form_state['values']['op'] == $form['actions']['cancel']['#value']) {
    $form_state['redirect'] = 'admin/config/crm-core/user_sync';
  }
}

/**
 * Delete rule form.
 */
function crm_core_user_sync_admin_delete_rule_form($form, &$form_state, $rule_key) {
  $form['rule_key'] = array(
    '#type' => 'value',
    '#value' => $rule_key,
  );

  $question = t('Are you sure you want to delete the user synchronization rule?');
  $path = 'admin/config/crm-core/user_sync';

  return confirm_form($form, $question, $path);
}

/**
 * Submission handler for delete rule form.
 */
function crm_core_user_sync_admin_delete_rule_form_submit($form, &$form_state) {
  $rule_key = $form_state['values']['rule_key'];

  $rules = variable_get('crm_core_user_sync_rules', array());
  unset($rules[$rule_key]);
  variable_set('crm_core_user_sync_rules', $rules);

  $form_state['redirect'] = 'admin/config/crm-core/user_sync';
  drupal_set_message(t('User syncrhonzation rule has been deleted.'));
}

/**
 * Change status callback.
 */
function crm_core_user_sync_admin_update_rule_status($rule_key, $status) {
  $rules = variable_get('crm_core_user_sync_rules', array());
  $rules[$rule_key]['enabled'] = $status;
  variable_set('crm_core_user_sync_rules', $rules);

  drupal_goto('admin/config/crm-core/user_sync');
}

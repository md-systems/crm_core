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

    $links = & $form['crm_core_user_sync_rules'][$key]['operations']['#links'];
    $path = 'admin/config/crm-core/user-sync/' . $key;
    $links['edit'] = array('title' => 'Edit', 'href' => $path . '/edit');
    $links['delete'] = array('title' => 'Delete', 'href' => $path . '/delete');

    if ($rule['enabled']) {
      $links['disable'] = array(
        'title' => 'Disable',
        'href' => $path . '/disable'
      );
    }
    else {
      $links['enable'] = array(
        'title' => 'Enable',
        'href' => $path . '/enable'
      );
    }
  }

  $form['crm_core_user_sync_auto_sync_user_create'] = array(
    '#type' => 'checkbox',
    '#title' => t('Automatically create an associated contact when account is created'),
    '#description' => t('When checked, this checkbox will automatically create new contacts when a new user account is created according to rules listed above. Rules will be processed in order until a new contact is created.'),
    '#default_value' => variable_get('crm_core_user_sync_auto_sync_user_create', 1),
  );

  $form['crm_core_user_sync_wrapper'] = array(
    '#type' => 'fieldset',
    '#title' => t('Sync Current Users'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['crm_core_user_sync_wrapper']['user_sync'] = array(
    '#type' => 'submit',
    '#value' => t('Synchronize Users'),
    '#submit' => array('crm_core_user_sync_admin_form_user_sync_submit'),
  );
  $form['crm_core_user_sync_wrapper']['description_wrapper'] = array(
    '#type' => 'container',
  );
  $form['crm_core_user_sync_wrapper']['description_wrapper']['description'] = array(
    '#type' => 'item',
    '#markup' => t('Click this button to apply user synchronization rules to all user accounts that are currently not'
      . ' associated with a contact in the system. It will create an associated contact record for each user according'
      . ' to the rules configured above. Warning: this cannot be undone.'),
  );

  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save changes')
  );

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

  foreach ($form_state['values']['crm_core_user_sync_rules'] as $key => $values) {
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

  $header = array(
    t('Role'),
    t('Contact Type'),
    t('Enabled'),
    t('Weight'),
    t('Operations')
  );
  $form['crm_core_user_sync_rules']['content']['#markup'] = theme('table', array(
    'header' => $header,
    'rows' => $rows,
    'attributes' => array('id' => 'crm-user-sync-settings-order')
  ));
  $output = drupal_render_children($form);

  drupal_add_tabledrag('crm-user-sync-settings-order', 'order', 'sibling', 'crm-user-sync-settings-order-weight');

  return $output;
}

/**
 * Add new rule form.
 */
function crm_core_user_sync_admin_edit_rule_form($form, &$form_state, $rule_key = NULL) {
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
    '#required' => TRUE,
  );

  $form['contact_type'] = array(
    '#type' => 'select',
    '#title' => t('Contact Type'),
    '#options' => $types_options,
    '#default_value' => isset($rule['contact_type']) ? $rule['contact_type'] : '',
    '#required' => TRUE,
  );

  $form['weight'] = array(
    '#type' => 'weight',
    '#title' => t('Weight'),
    '#default_value' => isset($rule['weight']) ? $rule['weight'] : 0,
  );

  $form['enabled'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enabled'),
    '#default_value' => isset($rule['enabled']) ? $rule['enabled'] : TRUE,
    '#description' => 'When checked, this rule will be used to synchronize user accounts. When unchecked, it will be ignored throughout the system.'
  );

  $form['actions'] = array('#type' => 'actions');
  $form['actions']['save'] = array('#type' => 'submit', '#value' => t('Save'));
  $form['actions']['cancel'] = array(
    '#type' => 'submit',
    '#value' => t('Cancel')
  );

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

    $form_state['redirect'] = 'admin/config/crm-core/user-sync';
    drupal_set_message(t('User syncrhonzation rule has been saved.'));
  }
  elseif ($form_state['values']['op'] == $form['actions']['cancel']['#value']) {
    $form_state['redirect'] = 'admin/config/crm-core/user-sync';
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
  $path = 'admin/config/crm-core/user-sync';

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

  $form_state['redirect'] = 'admin/config/crm-core/user-sync';
  drupal_set_message(t('User syncrhonzation rule has been deleted.'));
}

/**
 * Change status callback.
 */
function crm_core_user_sync_admin_update_rule_status($rule_key, $status) {
  $rules = variable_get('crm_core_user_sync_rules', array());
  $rules[$rule_key]['enabled'] = $status;
  variable_set('crm_core_user_sync_rules', $rules);

  drupal_goto('admin/config/crm-core/user-sync');
}

/**
 * Contact->User relation edit form.
 */
function crm_core_user_sync_edit_relation_form($form, &$form_state, $relation) {
  if (empty($relation)) {
    drupal_set_title('Create a new relation');

    if (!empty($_GET['uid'])) {
      $user = user_load($_GET['uid']);
      $disabled_user_field = !empty($user);
    }

    if (!empty($_GET['contact_id'])) {
      $contact = crm_core_contact_load($_GET['contact_id']);
      $disabled_contact_field = !empty($contact);
    }
  }
  else {
    drupal_set_title('Edit relation');

    if (!empty($relation->endpoints[LANGUAGE_NONE][0]['entity_id'])) {
      $user = user_load($relation->endpoints[LANGUAGE_NONE][0]['entity_id']);
    }

    if (!empty($relation->endpoints[LANGUAGE_NONE][1]['entity_id'])) {
      $contact = crm_core_contact_load($relation->endpoints[LANGUAGE_NONE][1]['entity_id']);
    }
  }

  $form['relation'] = array(
    '#type' => 'value',
    '#value' => $relation,
  );

  $form['description'] = array(
    '#type' => 'item',
    '#markup' => t('You can edit the details of a contact record to user account relationship using the following form.'),
  );

  $form['contact'] = array(
    '#title' => t('Contact record'),
    '#type' => 'textfield',
    '#default_value' => empty($contact) ? '' : $contact->label() . " [contact_id:{$contact->contact_id}]",
    '#disabled' => empty($disabled_contact_field) ? FALSE : TRUE,
    '#autocomplete_path' => 'admin/config/crm-core/user-sync/contact-to-user-management/contact-autocomplete/%',
  );

  $form['user'] = array(
    '#title' => t('User record'),
    '#type' => 'textfield',
    '#default_value' => empty($user) ? '' : $user->name . " [uid:{$user->uid}]",
    '#disabled' => empty($disabled_user_field) ? FALSE : TRUE,
    '#autocomplete_path' => 'admin/config/crm-core/user-sync/contact-to-user-management/user-autocomplete/%',
  );

  $form['actions'] = array('#type' => 'actions');

  $form['actions']['save'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
  );

  if (!empty($relation)) {
    $form['actions']['delete'] = array(
      '#type' => 'submit',
      '#value' => t('Delete'),
    );
  }

  $form['actions']['cancel'] = array(
    '#markup' => l(t('Cancel'), 'admin/config/crm-core/user-sync/contact-to-user-management'),
  );

  return $form;
}

/**
 * Validate callback on edit form.
 */
function crm_core_user_sync_edit_relation_form_validate($form, &$form_state) {
  if ($form_state['values']['op'] == $form['actions']['save']['#value']) {
    $user = _crm_core_user_sync_get_entity_id_from_text($form_state['values']['user'], 'user');
    $contact = _crm_core_user_sync_get_entity_id_from_text($form_state['values']['contact'], 'crm_core_contact');

    if (empty($user)) {
      form_set_error('user', t('Could not load a user account.'));
    }

    if (empty($contact)) {
      form_set_error('contact', t('Could not load a contact.'));
    }

    if (empty($user) || empty($contact)) {
      return;
    }

    // Check if the same user->contact relation exists when adding or editing relation.
    if (crm_core_user_sync_get_contact_from_uid($user->uid) && (empty($form_state['values']['relation']) ||
      $form_state['values']['relation']->endpoints[LANGUAGE_NONE][0]['entity_id'] != $user->uid)
    ) {
      form_set_error('user', t('The user account has been already synchronized.'));
    }

    // Check if the same user->contact relation exists when adding or editing relation.
    if (crm_core_user_sync_get_user_from_contact_id($contact->contact_id) && (empty($form_state['values']['relation']) ||
      $form_state['values']['relation']->endpoints[LANGUAGE_NONE][1]['entity_id'] != $contact->contact_id)
    ) {
      form_set_error('contact', t('The contact record has been already synchronized.'));
    }
  }
}

/**
 * Submit callback on edit form.
 */
function crm_core_user_sync_edit_relation_form_submit($form, &$form_state) {
  if ($form_state['values']['op'] == $form['actions']['save']['#value']) {
    $user = _crm_core_user_sync_get_entity_id_from_text($form_state['values']['user'], 'user');
    $contact = _crm_core_user_sync_get_entity_id_from_text($form_state['values']['contact'], 'crm_core_contact');

    if (!empty($form_state['values']['relation'])) {
      $relation = $form_state['values']['relation'];

      $relation->endpoints[LANGUAGE_NONE][0]['entity_id'] = $user->uid;
      $relation->endpoints[LANGUAGE_NONE][1]['entity_id'] = $contact->contact_id;

      relation_save($relation);
    }
    else {
      $relation = relation_create('crm_core_user_sync', array(
        array('entity_type' => 'user', 'entity_id' => $user->uid),
        array(
          'entity_type' => 'crm_core_contact',
          'entity_id' => $contact->contact_id
        ),
      ));
      relation_save($relation);
    }

    $form_state['redirect'] = 'admin/config/crm-core/user-sync/contact-to-user-management';
    drupal_set_message('Relation has been saved.');
  }
  elseif ($form_state['values']['op'] == $form['actions']['delete']['#value']) {
    $relation = $form_state['values']['relation'];
    $form_state['redirect'] = 'admin/config/crm-core/user-sync/contact-to-user-management/' . $relation->rid . '/delete';
  }
}

/**
 * User autocomplete for user->contact relation form.
 */
function crm_core_user_sync_user_autocomplete($search_string) {
  $matches = array();

  $query = db_select('users', 'user');
  $on_cond = "endpoints.endpoints_entity_type = 'user' AND user.uid = endpoints.endpoints_entity_id";
  $query->leftJoin('field_data_endpoints', 'endpoints', $on_cond);
  $query->addField('user', 'uid');
  $query->isNull('endpoints_entity_id');
  $query->condition('name', $search_string . '%', 'LIKE');
  watchdog('debug', $query->__toString());
  $uids = $query->range(0, 20)->execute()->fetchCol();

  if (!empty($uids)) {
    $users = entity_load('user', $uids);
    foreach ($users as $user) {
      $matches[$user->name . " [uid:$user->uid]"] = $user->name;
    }
  }

  drupal_json_output($matches);
}

/**
 * User autocomplete for user->contact relation form.
 */
function crm_core_user_sync_contact_autocomplete($search_string) {
  $matches = array();

  $query = new EntityFieldQuery;
  $query->entityCondition('entity_type', 'crm_core_contact')
    ->range(0, 20);

  $query->addMetaData('match', $search_string)
    ->addTag(variable_get('crm_core_contact_search_query_tag', 'crm_core_contact_search'));
  $query->addMetaData('user', FALSE);

  $result = $query->execute();
  if (!empty($result)) {
    $contacts = entity_load('crm_core_contact', array_keys($result['crm_core_contact']));
    foreach ($contacts as $contact) {
      $crm_core_contact_title = crm_core_contact_title($contact);
      $matches[$crm_core_contact_title . " [contact_id:$contact->contact_id]"] = $crm_core_contact_title;
    }
  }
  drupal_json_output($matches);
}

/**
 * Delete relation form.
 */
function crm_core_user_sync_delete_relation_form($form, &$form_state, $relation) {
  $form['relation'] = array(
    '#type' => 'value',
    '#value' => $relation,
  );

  $question = t('Are you sure you want to delete the relation?');
  $path = 'admin/config/crm-core/user-sync/contact-to-user-management';

  return confirm_form($form, $question, $path);
}

/**
 * Submission handler for delete relation form.
 */
function crm_core_user_sync_delete_relation_form_submit($form, &$form_state) {
  $relation = $form_state['values']['relation'];

  relation_delete($relation->rid);

  $form_state['redirect'] = 'admin/config/crm-core/user-sync/contact-to-user-management';
  drupal_set_message(t('Relation has been deleted.'));
}

/**
 * Submit callback. Handles user synchronization.
 */
function crm_core_user_sync_admin_form_user_sync_submit($form, &$form_state) {
  $operations[] = array('_crm_core_user_sync_batch_processing', array());
  $batch = array(
    'operations' => $operations,
    'title' => t('Processing user synchronization'),
    'finished' => '_crm_core_user_sync_batch_processing_finished',
    'file' => drupal_get_path('module', 'crm_core_user_sync') . '/crm_core_user_sync.admin.inc'
  );
  batch_set($batch);
}

/**
 * Helper function for batch processing of users synchronization.
 *
 * @param $uid
 *  User id for which contact should be created.
 */
function _crm_core_user_sync_batch_processing(&$context) {
  $query = db_select('users', 'users');
  $query->leftJoin('field_data_endpoints', 'endpoints', "endpoints.endpoints_entity_id = users.uid AND endpoints.endpoints_entity_type = 'user'");
  $query->addField('users', 'uid');
  $query->addField('endpoints', 'entity_id', 'rid');
  $query->condition('users.status', "'0'", '<>');
  $query->isNull('endpoints.entity_id');

  if (empty($context['sandbox'])) {
    $context['sandbox'] = array();
    $count_query = $query->countQuery();
    $context['sandbox']['max'] = $count_query->execute()->fetchField();
    $context['sandbox']['progress'] = 0;
  }
  // Limit users quantity to process per request.
  $limit = 20;
  $uids = $query->range(0, $limit)->execute()->fetchCol();
  $users = user_load_multiple($uids);
  foreach ($users as $user) {
    crm_core_user_sync_sync($user);
    $context['sandbox']['progress']++;
  }

  if ($context['sandbox']['progress'] != $context['sandbox']['max']) {
    $context['finished'] = $context['sandbox']['progress'] / $context['sandbox']['max'];
  }
  else {
    $context['results']['synced'] = $context['sandbox']['max'];
  }
}

/**
 * Helper to set message to user when batch processing finished.
 */
function _crm_core_user_sync_batch_processing_finished($success, $results, $operations) {
  if ($success) {
    drupal_set_message(t('@count users have been associated with contacts.', array('@count' => $results['synced'])));
  }
  else {
    drupal_set_message(t('Something went wrong. Please check !dblog.', array(
      '!dblog' =>
      l(t('Recent log messages'), 'admin/reports/dblog')
    )), 'error');
  }
}

<?php

/**
 * Implements hook_views_default_views().
 */
function crm_core_user_sync_views_default_views() {
  $view = new view();
  $view->name = 'crm_core_contact_to_user_management';
  $view->description = '';
  $view->tag = 'default';
  $view->base_table = 'crm_core_contact';
  $view->human_name = 'CRM Core Contact to User Management';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Contact to User Management';
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['access']['perm'] = 'administer relations';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '25';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['pager']['options']['id'] = '0';
  $handler->display->display_options['pager']['options']['quantity'] = '9';
  $handler->display->display_options['style_plugin'] = 'table';
  $handler->display->display_options['style_options']['columns'] = array(
    'contact_id' => 'contact_id',
    'uid' => 'uid',
    'rid' => 'rid',
    'contact_name' => 'contact_name',
    'name' => 'name',
    'uid_2' => 'uid_2',
    'nothing' => 'nothing',
    'nothing_1' => 'nothing',
    'nothing_2' => 'nothing',
    'rid_1' => 'nothing',
    'contact_id_1' => 'contact_id_1',
    'uid_1' => 'nothing',
    'nothing_3' => 'nothing',
  );
  $handler->display->display_options['style_options']['default'] = '-1';
  $handler->display->display_options['style_options']['info'] = array(
    'contact_id' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'uid' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'rid' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'contact_name' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'name' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'uid_2' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'nothing' => array(
      'align' => '',
      'separator' => ' | ',
      'empty_column' => 0,
    ),
    'nothing_1' => array(
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'nothing_2' => array(
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'rid_1' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'contact_id_1' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'uid_1' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'nothing_3' => array(
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
  );
  /* Header: Global: Text area */
  $handler->display->display_options['header']['area']['id'] = 'area';
  $handler->display->display_options['header']['area']['table'] = 'views';
  $handler->display->display_options['header']['area']['field'] = 'area';
  $handler->display->display_options['header']['area']['content'] = 'Occasionally, a contact will be mismatched with a user account. This tool provides you with manual control over the relations between contact records and user accounts. [nothing_4] for automated contact synchronization.';
  $handler->display->display_options['header']['area']['format'] = 'filtered_html';
  $handler->display->display_options['header']['area']['tokenize'] = TRUE;
  /* Relationship: CRM Contact: Relation: User (crm_core_contact → user) */
  $handler->display->display_options['relationships']['relation_crm_core_user_sync_user']['id'] = 'relation_crm_core_user_sync_user';
  $handler->display->display_options['relationships']['relation_crm_core_user_sync_user']['table'] = 'crm_core_contact';
  $handler->display->display_options['relationships']['relation_crm_core_user_sync_user']['field'] = 'relation_crm_core_user_sync_user';
  $handler->display->display_options['relationships']['relation_crm_core_user_sync_user']['r_index'] = '-1';
  $handler->display->display_options['relationships']['relation_crm_core_user_sync_user']['entity_deduplication_left'] = 0;
  $handler->display->display_options['relationships']['relation_crm_core_user_sync_user']['entity_deduplication_right'] = 0;
  /* Relationship: User: Relation: Contact (user → relation) */
  $handler->display->display_options['relationships']['relation_base_left_crm_core_user_sync']['id'] = 'relation_base_left_crm_core_user_sync';
  $handler->display->display_options['relationships']['relation_base_left_crm_core_user_sync']['table'] = 'users';
  $handler->display->display_options['relationships']['relation_base_left_crm_core_user_sync']['field'] = 'relation_base_left_crm_core_user_sync';
  $handler->display->display_options['relationships']['relation_base_left_crm_core_user_sync']['relationship'] = 'relation_crm_core_user_sync_user';
  $handler->display->display_options['relationships']['relation_base_left_crm_core_user_sync']['label'] = 'Relation';
  /* Field: CRM Contact: Contact ID */
  $handler->display->display_options['fields']['contact_id']['id'] = 'contact_id';
  $handler->display->display_options['fields']['contact_id']['table'] = 'crm_core_contact';
  $handler->display->display_options['fields']['contact_id']['field'] = 'contact_id';
  $handler->display->display_options['fields']['contact_id']['label'] = '';
  $handler->display->display_options['fields']['contact_id']['exclude'] = TRUE;
  $handler->display->display_options['fields']['contact_id']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['contact_id']['link_to_contact'] = 0;
  /* Field: User: Uid */
  $handler->display->display_options['fields']['uid']['id'] = 'uid';
  $handler->display->display_options['fields']['uid']['table'] = 'users';
  $handler->display->display_options['fields']['uid']['field'] = 'uid';
  $handler->display->display_options['fields']['uid']['relationship'] = 'relation_crm_core_user_sync_user';
  $handler->display->display_options['fields']['uid']['label'] = '';
  $handler->display->display_options['fields']['uid']['exclude'] = TRUE;
  $handler->display->display_options['fields']['uid']['alter']['alter_text'] = TRUE;
  $handler->display->display_options['fields']['uid']['alter']['text'] = '[name]';
  $handler->display->display_options['fields']['uid']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['uid']['link_to_user'] = FALSE;
  /* Field: Relation: Rid */
  $handler->display->display_options['fields']['rid']['id'] = 'rid';
  $handler->display->display_options['fields']['rid']['table'] = 'relation';
  $handler->display->display_options['fields']['rid']['field'] = 'rid';
  $handler->display->display_options['fields']['rid']['relationship'] = 'relation_base_left_crm_core_user_sync';
  $handler->display->display_options['fields']['rid']['label'] = '';
  $handler->display->display_options['fields']['rid']['exclude'] = TRUE;
  $handler->display->display_options['fields']['rid']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['rid']['separator'] = '';
  /* Field: CRM Contact: Household name */
  $handler->display->display_options['fields']['contact_name']['id'] = 'contact_name';
  $handler->display->display_options['fields']['contact_name']['table'] = 'field_data_contact_name';
  $handler->display->display_options['fields']['contact_name']['field'] = 'contact_name';
  $handler->display->display_options['fields']['contact_name']['label'] = 'Contact name';
  $handler->display->display_options['fields']['contact_name']['alter']['make_link'] = TRUE;
  $handler->display->display_options['fields']['contact_name']['alter']['path'] = 'crm-core/contact/[contact_id]';
  $handler->display->display_options['fields']['contact_name']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['contact_name']['empty'] = '---';
  $handler->display->display_options['fields']['contact_name']['click_sort_column'] = 'title';
  $handler->display->display_options['fields']['contact_name']['settings'] = array(
    'format' => 'default',
    'markup' => 0,
    'output' => 'default',
    'multiple' => 'default',
    'multiple_delimiter' => ', ',
    'multiple_and' => 'text',
    'multiple_delimiter_precedes_last' => 'never',
    'multiple_el_al_min' => '3',
    'multiple_el_al_first' => '1',
  );
  /* Field: User: Name */
  $handler->display->display_options['fields']['name']['id'] = 'name';
  $handler->display->display_options['fields']['name']['table'] = 'users';
  $handler->display->display_options['fields']['name']['field'] = 'name';
  $handler->display->display_options['fields']['name']['relationship'] = 'relation_crm_core_user_sync_user';
  $handler->display->display_options['fields']['name']['label'] = 'User Name';
  $handler->display->display_options['fields']['name']['exclude'] = TRUE;
  $handler->display->display_options['fields']['name']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['name']['empty'] = '---';
  $handler->display->display_options['fields']['name']['hide_empty'] = TRUE;
  /* Field: User: Uid */
  $handler->display->display_options['fields']['uid_2']['id'] = 'uid_2';
  $handler->display->display_options['fields']['uid_2']['table'] = 'users';
  $handler->display->display_options['fields']['uid_2']['field'] = 'uid';
  $handler->display->display_options['fields']['uid_2']['relationship'] = 'relation_crm_core_user_sync_user';
  $handler->display->display_options['fields']['uid_2']['label'] = 'User name';
  $handler->display->display_options['fields']['uid_2']['alter']['alter_text'] = TRUE;
  $handler->display->display_options['fields']['uid_2']['alter']['text'] = '[name]';
  $handler->display->display_options['fields']['uid_2']['empty'] = '---';
  $handler->display->display_options['fields']['uid_2']['empty_zero'] = TRUE;
  $handler->display->display_options['fields']['uid_2']['link_to_user'] = FALSE;
  /* Field: Global: Custom text */
  $handler->display->display_options['fields']['nothing']['id'] = 'nothing';
  $handler->display->display_options['fields']['nothing']['table'] = 'views';
  $handler->display->display_options['fields']['nothing']['field'] = 'nothing';
  $handler->display->display_options['fields']['nothing']['label'] = 'Operations';
  /* Field: Global: Custom text */
  $handler->display->display_options['fields']['nothing_1']['id'] = 'nothing_1';
  $handler->display->display_options['fields']['nothing_1']['table'] = 'views';
  $handler->display->display_options['fields']['nothing_1']['field'] = 'nothing';
  $handler->display->display_options['fields']['nothing_1']['label'] = 'Edit Link Text';
  $handler->display->display_options['fields']['nothing_1']['exclude'] = TRUE;
  $handler->display->display_options['fields']['nothing_1']['alter']['text'] = 'Edit';
  $handler->display->display_options['fields']['nothing_1']['alter']['make_link'] = TRUE;
  $handler->display->display_options['fields']['nothing_1']['alter']['path'] = 'admin/config/crm-core/user-sync/contact-to-user-management/[rid]/edit';
  $handler->display->display_options['fields']['nothing_1']['element_label_colon'] = FALSE;
  /* Field: Global: Custom text */
  $handler->display->display_options['fields']['nothing_2']['id'] = 'nothing_2';
  $handler->display->display_options['fields']['nothing_2']['table'] = 'views';
  $handler->display->display_options['fields']['nothing_2']['field'] = 'nothing';
  $handler->display->display_options['fields']['nothing_2']['label'] = 'Delete Link Text';
  $handler->display->display_options['fields']['nothing_2']['exclude'] = TRUE;
  $handler->display->display_options['fields']['nothing_2']['alter']['text'] = 'Delete';
  $handler->display->display_options['fields']['nothing_2']['alter']['make_link'] = TRUE;
  $handler->display->display_options['fields']['nothing_2']['alter']['path'] = 'admin/config/crm-core/user-sync/contact-to-user-management/[rid]/delete';
  $handler->display->display_options['fields']['nothing_2']['element_label_colon'] = FALSE;
  /* Field: Relation: Rid */
  $handler->display->display_options['fields']['rid_1']['id'] = 'rid_1';
  $handler->display->display_options['fields']['rid_1']['table'] = 'relation';
  $handler->display->display_options['fields']['rid_1']['field'] = 'rid';
  $handler->display->display_options['fields']['rid_1']['relationship'] = 'relation_base_left_crm_core_user_sync';
  $handler->display->display_options['fields']['rid_1']['label'] = 'Edit/Delete Link Controller';
  $handler->display->display_options['fields']['rid_1']['alter']['alter_text'] = TRUE;
  $handler->display->display_options['fields']['rid_1']['alter']['text'] = '[nothing_1] | [nothing_2] ';
  $handler->display->display_options['fields']['rid_1']['hide_empty'] = TRUE;
  $handler->display->display_options['fields']['rid_1']['empty_zero'] = TRUE;
  $handler->display->display_options['fields']['rid_1']['separator'] = '';
  /* Field: CRM Contact: Contact ID */
  $handler->display->display_options['fields']['contact_id_1']['id'] = 'contact_id_1';
  $handler->display->display_options['fields']['contact_id_1']['table'] = 'crm_core_contact';
  $handler->display->display_options['fields']['contact_id_1']['field'] = 'contact_id';
  $handler->display->display_options['fields']['contact_id_1']['label'] = 'Add user account link';
  $handler->display->display_options['fields']['contact_id_1']['exclude'] = TRUE;
  $handler->display->display_options['fields']['contact_id_1']['alter']['alter_text'] = TRUE;
  $handler->display->display_options['fields']['contact_id_1']['alter']['text'] = 'Add user record';
  $handler->display->display_options['fields']['contact_id_1']['alter']['make_link'] = TRUE;
  $handler->display->display_options['fields']['contact_id_1']['alter']['path'] = 'admin/config/crm-core/user-sync/contact-to-user-management/add?contact_id=[contact_id]';
  $handler->display->display_options['fields']['contact_id_1']['link_to_contact'] = 0;
  /* Field: User: Uid */
  $handler->display->display_options['fields']['uid_1']['id'] = 'uid_1';
  $handler->display->display_options['fields']['uid_1']['table'] = 'users';
  $handler->display->display_options['fields']['uid_1']['field'] = 'uid';
  $handler->display->display_options['fields']['uid_1']['relationship'] = 'relation_crm_core_user_sync_user';
  $handler->display->display_options['fields']['uid_1']['label'] = 'Add user account link controller';
  $handler->display->display_options['fields']['uid_1']['exclude'] = TRUE;
  $handler->display->display_options['fields']['uid_1']['alter']['alter_text'] = TRUE;
  $handler->display->display_options['fields']['uid_1']['alter']['text'] = ' ';
  $handler->display->display_options['fields']['uid_1']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['uid_1']['empty'] = '[contact_id_1]';
  $handler->display->display_options['fields']['uid_1']['link_to_user'] = FALSE;
  /* Field: Global: Custom text */
  $handler->display->display_options['fields']['nothing_3']['id'] = 'nothing_3';
  $handler->display->display_options['fields']['nothing_3']['table'] = 'views';
  $handler->display->display_options['fields']['nothing_3']['field'] = 'nothing';
  $handler->display->display_options['fields']['nothing_3']['label'] = 'Add user account trimmer';
  $handler->display->display_options['fields']['nothing_3']['alter']['text'] = '[uid_1]';
  $handler->display->display_options['fields']['nothing_3']['alter']['trim_whitespace'] = TRUE;
  $handler->display->display_options['fields']['nothing_3']['element_label_colon'] = FALSE;
  /* Field: Global: Custom text */
  $handler->display->display_options['fields']['nothing_4']['id'] = 'nothing_4';
  $handler->display->display_options['fields']['nothing_4']['table'] = 'views';
  $handler->display->display_options['fields']['nothing_4']['field'] = 'nothing';
  $handler->display->display_options['fields']['nothing_4']['label'] = '';
  $handler->display->display_options['fields']['nothing_4']['exclude'] = TRUE;
  $handler->display->display_options['fields']['nothing_4']['alter']['text'] = 'Configure the default rules';
  $handler->display->display_options['fields']['nothing_4']['alter']['make_link'] = TRUE;
  $handler->display->display_options['fields']['nothing_4']['alter']['path'] = 'admin/config/crm-core/user-sync';
  $handler->display->display_options['fields']['nothing_4']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['nothing_4']['element_default_classes'] = FALSE;
  /* Sort criterion: CRM Contact: Household name (contact_name:given) */
  $handler->display->display_options['sorts']['contact_name_given']['id'] = 'contact_name_given';
  $handler->display->display_options['sorts']['contact_name_given']['table'] = 'field_data_contact_name';
  $handler->display->display_options['sorts']['contact_name_given']['field'] = 'contact_name_given';
  $handler->display->display_options['sorts']['contact_name_given']['exposed'] = TRUE;
  $handler->display->display_options['sorts']['contact_name_given']['expose']['label'] = 'First name';
  /* Sort criterion: CRM Contact: Household name (contact_name:family) */
  $handler->display->display_options['sorts']['contact_name_family']['id'] = 'contact_name_family';
  $handler->display->display_options['sorts']['contact_name_family']['table'] = 'field_data_contact_name';
  $handler->display->display_options['sorts']['contact_name_family']['field'] = 'contact_name_family';
  $handler->display->display_options['sorts']['contact_name_family']['exposed'] = TRUE;
  $handler->display->display_options['sorts']['contact_name_family']['expose']['label'] = 'Last name';
  /* Sort criterion: User: Name */
  $handler->display->display_options['sorts']['name']['id'] = 'name';
  $handler->display->display_options['sorts']['name']['table'] = 'users';
  $handler->display->display_options['sorts']['name']['field'] = 'name';
  $handler->display->display_options['sorts']['name']['relationship'] = 'relation_crm_core_user_sync_user';
  $handler->display->display_options['sorts']['name']['exposed'] = TRUE;
  $handler->display->display_options['sorts']['name']['expose']['label'] = 'User name';
  /* Filter criterion: CRM Contact: Household name */
  $handler->display->display_options['filters']['contact_name']['id'] = 'contact_name';
  $handler->display->display_options['filters']['contact_name']['table'] = 'field_data_contact_name';
  $handler->display->display_options['filters']['contact_name']['field'] = 'contact_name';
  $handler->display->display_options['filters']['contact_name']['group'] = 1;
  $handler->display->display_options['filters']['contact_name']['exposed'] = TRUE;
  $handler->display->display_options['filters']['contact_name']['expose']['operator_id'] = 'contact_name_op';
  $handler->display->display_options['filters']['contact_name']['expose']['label'] = 'Filter by contact name';
  $handler->display->display_options['filters']['contact_name']['expose']['operator'] = 'contact_name_op';
  $handler->display->display_options['filters']['contact_name']['expose']['identifier'] = 'contact_name';
  $handler->display->display_options['filters']['contact_name']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
  );
  /* Filter criterion: User: Name */
  $handler->display->display_options['filters']['uid']['id'] = 'uid';
  $handler->display->display_options['filters']['uid']['table'] = 'users';
  $handler->display->display_options['filters']['uid']['field'] = 'uid';
  $handler->display->display_options['filters']['uid']['relationship'] = 'relation_crm_core_user_sync_user';
  $handler->display->display_options['filters']['uid']['value'] = '';
  $handler->display->display_options['filters']['uid']['group'] = 1;
  $handler->display->display_options['filters']['uid']['exposed'] = TRUE;
  $handler->display->display_options['filters']['uid']['expose']['operator_id'] = 'uid_op';
  $handler->display->display_options['filters']['uid']['expose']['label'] = 'Filter by user name';
  $handler->display->display_options['filters']['uid']['expose']['operator'] = 'uid_op';
  $handler->display->display_options['filters']['uid']['expose']['identifier'] = 'uid';
  $handler->display->display_options['filters']['uid']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
  );
  /* Filter criterion: User: E-mail */
  $handler->display->display_options['filters']['mail']['id'] = 'mail';
  $handler->display->display_options['filters']['mail']['table'] = 'users';
  $handler->display->display_options['filters']['mail']['field'] = 'mail';
  $handler->display->display_options['filters']['mail']['relationship'] = 'relation_crm_core_user_sync_user';
  $handler->display->display_options['filters']['mail']['operator'] = 'contains';
  $handler->display->display_options['filters']['mail']['group'] = 1;
  $handler->display->display_options['filters']['mail']['exposed'] = TRUE;
  $handler->display->display_options['filters']['mail']['expose']['operator_id'] = 'mail_op';
  $handler->display->display_options['filters']['mail']['expose']['label'] = 'Filter by user email address';
  $handler->display->display_options['filters']['mail']['expose']['operator'] = 'mail_op';
  $handler->display->display_options['filters']['mail']['expose']['identifier'] = 'mail';
  $handler->display->display_options['filters']['mail']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
  );

  /* Display: Page */
  $handler = $view->new_display('page', 'Page', 'page');
  $handler->display->display_options['defaults']['hide_admin_links'] = FALSE;
  $handler->display->display_options['path'] = 'admin/config/crm-core/user-sync/contact-to-user-management';
  $handler->display->display_options['menu']['type'] = 'tab';
  $handler->display->display_options['menu']['title'] = 'Contact to User Management';
  $handler->display->display_options['menu']['weight'] = '0';
  $handler->display->display_options['menu']['context'] = 0;

  $views['crm_core_contact_to_user_management'] = $view;

  return $views;
}

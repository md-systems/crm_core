<?php
// $Id$

/**
 * Views for the default CRM UI.
 */

/**
 * Implements hook_views_default_views().
 */
function crm_core_user_sync_views_default_views() {
$view = new view();
$view->name = 'crm_сore_contact_to_user_management';
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
$handler->display->display_options['access']['type'] = 'none';
$handler->display->display_options['cache']['type'] = 'none';
$handler->display->display_options['query']['type'] = 'views_query';
$handler->display->display_options['exposed_form']['type'] = 'basic';
$handler->display->display_options['pager']['type'] = 'full';
$handler->display->display_options['pager']['options']['items_per_page'] = '10';
$handler->display->display_options['style_plugin'] = 'table';
$handler->display->display_options['style_options']['columns'] = array(
  'contact_id' => 'contact_id',
  'contact_name' => 'contact_name',
  'rid' => 'rid',
  'name' => 'name',
  'uid' => 'uid',
  'edit' => 'edit',
  'delete' => 'edit',
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
  'contact_name' => array(
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
  'name' => array(
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
  'edit' => array(
    'sortable' => 0,
    'default_sort_order' => 'asc',
    'align' => '',
    'separator' => ' | ',
    'empty_column' => 0,
  ),
  'delete' => array(
    'sortable' => 0,
    'default_sort_order' => 'asc',
    'align' => '',
    'separator' => '',
    'empty_column' => 0,
  ),
);
/* Header: Global: Text area */
$handler->display->display_options['header']['area']['id'] = 'area';
$handler->display->display_options['header']['area']['table'] = 'views';
$handler->display->display_options['header']['area']['field'] = 'area';
$handler->display->display_options['header']['area']['content'] = 'Occasionally, a contact will be mismatched with a user account. This tool provides you with manual control over the relations between contact records and user accounts. You can configure the default rules for automated contact synchronization by <a href="admin/config/crm-core/user-sync">clicking here</a>.';
$handler->display->display_options['header']['area']['format'] = 'filtered_html';
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
$handler->display->display_options['relationships']['relation_base_left_crm_core_user_sync']['r_index'] = '-1';
$handler->display->display_options['relationships']['relation_base_left_crm_core_user_sync']['entity_deduplication_left'] = 0;
/* Field: CRM Contact: Contact ID */
$handler->display->display_options['fields']['contact_id']['id'] = 'contact_id';
$handler->display->display_options['fields']['contact_id']['table'] = 'crm_core_contact';
$handler->display->display_options['fields']['contact_id']['field'] = 'contact_id';
$handler->display->display_options['fields']['contact_id']['label'] = '';
$handler->display->display_options['fields']['contact_id']['exclude'] = TRUE;
$handler->display->display_options['fields']['contact_id']['element_label_colon'] = FALSE;
$handler->display->display_options['fields']['contact_id']['link_to_contact'] = 0;
/* Field: CRM Contact: Household name */
$handler->display->display_options['fields']['contact_name']['id'] = 'contact_name';
$handler->display->display_options['fields']['contact_name']['table'] = 'field_data_contact_name';
$handler->display->display_options['fields']['contact_name']['field'] = 'contact_name';
$handler->display->display_options['fields']['contact_name']['label'] = 'Contact name';
$handler->display->display_options['fields']['contact_name']['alter']['make_link'] = TRUE;
$handler->display->display_options['fields']['contact_name']['alter']['path'] = '[contact_id]';
$handler->display->display_options['fields']['contact_name']['element_label_colon'] = FALSE;
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
/* Field: Relation: Rid */
$handler->display->display_options['fields']['rid']['id'] = 'rid';
$handler->display->display_options['fields']['rid']['table'] = 'relation';
$handler->display->display_options['fields']['rid']['field'] = 'rid';
$handler->display->display_options['fields']['rid']['relationship'] = 'relation_base_left_crm_core_user_sync';
$handler->display->display_options['fields']['rid']['label'] = '';
$handler->display->display_options['fields']['rid']['exclude'] = TRUE;
$handler->display->display_options['fields']['rid']['element_label_colon'] = FALSE;
$handler->display->display_options['fields']['rid']['hide_empty'] = TRUE;
$handler->display->display_options['fields']['rid']['empty_zero'] = TRUE;
$handler->display->display_options['fields']['rid']['separator'] = '';
/* Field: User: Name */
$handler->display->display_options['fields']['name']['id'] = 'name';
$handler->display->display_options['fields']['name']['table'] = 'users';
$handler->display->display_options['fields']['name']['field'] = 'name';
$handler->display->display_options['fields']['name']['relationship'] = 'relation_crm_core_user_sync_user';
$handler->display->display_options['fields']['name']['label'] = '';
$handler->display->display_options['fields']['name']['exclude'] = TRUE;
$handler->display->display_options['fields']['name']['element_label_colon'] = FALSE;
$handler->display->display_options['fields']['name']['link_to_user'] = FALSE;
/* Field: User: Uid */
$handler->display->display_options['fields']['uid']['id'] = 'uid';
$handler->display->display_options['fields']['uid']['table'] = 'users';
$handler->display->display_options['fields']['uid']['field'] = 'uid';
$handler->display->display_options['fields']['uid']['relationship'] = 'relation_crm_core_user_sync_user';
$handler->display->display_options['fields']['uid']['label'] = 'User name';
$handler->display->display_options['fields']['uid']['alter']['alter_text'] = TRUE;
$handler->display->display_options['fields']['uid']['alter']['text'] = '[name]';
$handler->display->display_options['fields']['uid']['element_label_colon'] = FALSE;
/* Field: Relation: Edit link */
$handler->display->display_options['fields']['edit']['id'] = 'edit';
$handler->display->display_options['fields']['edit']['table'] = 'relation';
$handler->display->display_options['fields']['edit']['field'] = 'edit';
$handler->display->display_options['fields']['edit']['relationship'] = 'relation_base_left_crm_core_user_sync';
$handler->display->display_options['fields']['edit']['label'] = 'Operations';
/* Field: Relation: Delete link */
$handler->display->display_options['fields']['delete']['id'] = 'delete';
$handler->display->display_options['fields']['delete']['table'] = 'relation';
$handler->display->display_options['fields']['delete']['field'] = 'delete';
$handler->display->display_options['fields']['delete']['relationship'] = 'relation_base_left_crm_core_user_sync';
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


  $views['crm_сore_contact_to_user_management'] = $view;
  return $views;
}

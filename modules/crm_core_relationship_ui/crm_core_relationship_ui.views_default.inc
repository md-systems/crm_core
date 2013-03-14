<?php

/**
 * Implements hook_views_default_views().
 */
function crm_core_relationship_ui_views_default_views() {
  $view = new view();
  $view->name = 'crm_relationships_by_contact';
  $view->description = 'Displays contact relationships in tabs.';
  $view->tag = 'contact';
  $view->base_table = 'relation';
  $view->human_name = 'CRM Relationships by contact';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Relationships for [endpoints_2]';
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['access']['perm'] = 'view relation entities of any contact relationship';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['query']['options']['query_comment'] = FALSE;
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['exposed_form']['options']['reset_button'] = TRUE;
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '50';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['pager']['options']['id'] = '0';
  $handler->display->display_options['pager']['options']['quantity'] = '9';
  $handler->display->display_options['style_plugin'] = 'table';
  $handler->display->display_options['style_options']['columns'] = array(
    'rid' => 'rid',
    'endpoints' => 'endpoints',
    'endpoints_1' => 'endpoints_1',
    'edit' => 'edit',
    'crm_core_relationship_status_1' => 'crm_core_relationship_status_1',
    'delete' => 'delete',
    'nothing' => 'nothing',
  );
  $handler->display->display_options['style_options']['default'] = '-1';
  $handler->display->display_options['style_options']['info'] = array(
    'rid' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'endpoints' => array(
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'endpoints_1' => array(
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'edit' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'crm_core_relationship_status_1' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'delete' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'nothing' => array(
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
  );
  /* Header: Global: Result summary */
  $handler->display->display_options['header']['result']['id'] = 'result';
  $handler->display->display_options['header']['result']['table'] = 'views';
  $handler->display->display_options['header']['result']['field'] = 'result';
  $handler->display->display_options['header']['result']['content'] = 'Displaying @start - @end of @total active contact relationships';
  /* Header: Global: Text area */
  $handler->display->display_options['header']['area']['id'] = 'area';
  $handler->display->display_options['header']['area']['table'] = 'views';
  $handler->display->display_options['header']['area']['field'] = 'area';
  $handler->display->display_options['header']['area']['content'] = '<h2>Active Relationships</h2>';
  $handler->display->display_options['header']['area']['format'] = 'full_html';
  /* No results behavior: Global: Text area */
  $handler->display->display_options['empty']['area']['id'] = 'area';
  $handler->display->display_options['empty']['area']['table'] = 'views';
  $handler->display->display_options['empty']['area']['field'] = 'area';
  $handler->display->display_options['empty']['area']['empty'] = TRUE;
  $handler->display->display_options['empty']['area']['content'] = 'No active relationships are stored in the system for this contact at this time.';
  $handler->display->display_options['empty']['area']['format'] = 'plain_text';
  /* Field: Relation: Rid */
  $handler->display->display_options['fields']['rid']['id'] = 'rid';
  $handler->display->display_options['fields']['rid']['table'] = 'relation';
  $handler->display->display_options['fields']['rid']['field'] = 'rid';
  $handler->display->display_options['fields']['rid']['exclude'] = TRUE;
  /* Field: Relation: endpoints */
  $handler->display->display_options['fields']['endpoints']['id'] = 'endpoints';
  $handler->display->display_options['fields']['endpoints']['table'] = 'field_data_endpoints';
  $handler->display->display_options['fields']['endpoints']['field'] = 'endpoints';
  $handler->display->display_options['fields']['endpoints']['label'] = '';
  $handler->display->display_options['fields']['endpoints']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['endpoints']['hide_alter_empty'] = FALSE;
  $handler->display->display_options['fields']['endpoints']['click_sort_column'] = 'entity_type';
  $handler->display->display_options['fields']['endpoints']['type'] = 'contact_relationship_human';
  $handler->display->display_options['fields']['endpoints']['delta_offset'] = '0';
  /* Field: Relation: endpoints */
  $handler->display->display_options['fields']['endpoints_1']['id'] = 'endpoints_1';
  $handler->display->display_options['fields']['endpoints_1']['table'] = 'field_data_endpoints';
  $handler->display->display_options['fields']['endpoints_1']['field'] = 'endpoints';
  $handler->display->display_options['fields']['endpoints_1']['label'] = 'Contact ID';
  $handler->display->display_options['fields']['endpoints_1']['exclude'] = TRUE;
  $handler->display->display_options['fields']['endpoints_1']['alter']['alter_text'] = TRUE;
  $handler->display->display_options['fields']['endpoints_1']['alter']['text'] = '[endpoints_1-entity_id]';
  $handler->display->display_options['fields']['endpoints_1']['click_sort_column'] = 'entity_type';
  $handler->display->display_options['fields']['endpoints_1']['type'] = 'relation_endpoint_full';
  $handler->display->display_options['fields']['endpoints_1']['delta_offset'] = '0';
  /* Field: Relation: Edit link */
  $handler->display->display_options['fields']['edit']['id'] = 'edit';
  $handler->display->display_options['fields']['edit']['table'] = 'relation';
  $handler->display->display_options['fields']['edit']['field'] = 'edit';
  $handler->display->display_options['fields']['edit']['exclude'] = TRUE;
  /* Field: Relation: Status */
  $handler->display->display_options['fields']['crm_core_relationship_status_1']['id'] = 'crm_core_relationship_status_1';
  $handler->display->display_options['fields']['crm_core_relationship_status_1']['table'] = 'field_data_crm_core_relationship_status';
  $handler->display->display_options['fields']['crm_core_relationship_status_1']['field'] = 'crm_core_relationship_status';
  $handler->display->display_options['fields']['crm_core_relationship_status_1']['label'] = 'Status (Link)';
  $handler->display->display_options['fields']['crm_core_relationship_status_1']['exclude'] = TRUE;
  $handler->display->display_options['fields']['crm_core_relationship_status_1']['alter']['alter_text'] = TRUE;
  $handler->display->display_options['fields']['crm_core_relationship_status_1']['alter']['text'] = 'Mark as inactive';
  $handler->display->display_options['fields']['crm_core_relationship_status_1']['alter']['make_link'] = TRUE;
  $handler->display->display_options['fields']['crm_core_relationship_status_1']['alter']['path'] = 'crm-core/contact/[endpoints_1]/relationships/[rid]/inactive?destination=crm-core/contact/[endpoints_1]/relationships';
  $handler->display->display_options['fields']['crm_core_relationship_status_1']['type'] = 'list_key';
  /* Field: Relation: Delete link */
  $handler->display->display_options['fields']['delete']['id'] = 'delete';
  $handler->display->display_options['fields']['delete']['table'] = 'relation';
  $handler->display->display_options['fields']['delete']['field'] = 'delete';
  $handler->display->display_options['fields']['delete']['exclude'] = TRUE;
  /* Field: Global: Custom text */
  $handler->display->display_options['fields']['nothing']['id'] = 'nothing';
  $handler->display->display_options['fields']['nothing']['table'] = 'views';
  $handler->display->display_options['fields']['nothing']['field'] = 'nothing';
  $handler->display->display_options['fields']['nothing']['label'] = '';
  $handler->display->display_options['fields']['nothing']['alter']['text'] = '[edit] [crm_core_relationship_status_1] [delete]';
  $handler->display->display_options['fields']['nothing']['element_label_colon'] = FALSE;
  /* Field: Relation: endpoints */
  $handler->display->display_options['fields']['endpoints_2']['id'] = 'endpoints_2';
  $handler->display->display_options['fields']['endpoints_2']['table'] = 'field_data_endpoints';
  $handler->display->display_options['fields']['endpoints_2']['field'] = 'endpoints';
  $handler->display->display_options['fields']['endpoints_2']['label'] = 'Current contact name';
  $handler->display->display_options['fields']['endpoints_2']['exclude'] = TRUE;
  $handler->display->display_options['fields']['endpoints_2']['alter']['alter_text'] = TRUE;
  $handler->display->display_options['fields']['endpoints_2']['click_sort_column'] = 'entity_type';
  $handler->display->display_options['fields']['endpoints_2']['type'] = 'contact_relationship_current';
  $handler->display->display_options['fields']['endpoints_2']['delta_offset'] = '0';
  /* Contextual filter: Relation: endpoints (endpoints:entity_id) */
  $handler->display->display_options['arguments']['endpoints_entity_id']['id'] = 'endpoints_entity_id';
  $handler->display->display_options['arguments']['endpoints_entity_id']['table'] = 'field_data_endpoints';
  $handler->display->display_options['arguments']['endpoints_entity_id']['field'] = 'endpoints_entity_id';
  $handler->display->display_options['arguments']['endpoints_entity_id']['default_action'] = 'default';
  $handler->display->display_options['arguments']['endpoints_entity_id']['default_argument_type'] = 'php';
  $handler->display->display_options['arguments']['endpoints_entity_id']['default_argument_options']['code'] = 'return arg(2);';
  $handler->display->display_options['arguments']['endpoints_entity_id']['summary']['number_of_records'] = '0';
  $handler->display->display_options['arguments']['endpoints_entity_id']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['endpoints_entity_id']['summary_options']['items_per_page'] = '25';
  /* Contextual filter: Relation: Relation type */
  $handler->display->display_options['arguments']['relation_type']['id'] = 'relation_type';
  $handler->display->display_options['arguments']['relation_type']['table'] = 'relation';
  $handler->display->display_options['arguments']['relation_type']['field'] = 'relation_type';
  $handler->display->display_options['arguments']['relation_type']['default_action'] = 'default';
  $handler->display->display_options['arguments']['relation_type']['default_argument_type'] = 'php';
  $handler->display->display_options['arguments']['relation_type']['default_argument_options']['code'] = 'return implode(\',\',array_keys(crm_core_relationship_get_relationships()));';
  $handler->display->display_options['arguments']['relation_type']['summary']['number_of_records'] = '0';
  $handler->display->display_options['arguments']['relation_type']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['relation_type']['summary_options']['items_per_page'] = '25';
  $handler->display->display_options['arguments']['relation_type']['limit'] = '0';
  $handler->display->display_options['arguments']['relation_type']['break_phrase'] = TRUE;
  /* Filter criterion: Relation: Relation type */
  $handler->display->display_options['filters']['relation_type']['id'] = 'relation_type';
  $handler->display->display_options['filters']['relation_type']['table'] = 'relation';
  $handler->display->display_options['filters']['relation_type']['field'] = 'relation_type';
  $handler->display->display_options['filters']['relation_type']['value'] = array(
    'crm_employee' => 'crm_employee',
    'crm_friend' => 'crm_friend',
    'crm_member' => 'crm_member',
  );
  $handler->display->display_options['filters']['relation_type']['exposed'] = TRUE;
  $handler->display->display_options['filters']['relation_type']['expose']['operator_id'] = 'relation_type_op';
  $handler->display->display_options['filters']['relation_type']['expose']['label'] = 'Filter by:';
  $handler->display->display_options['filters']['relation_type']['expose']['operator'] = 'relation_type_op';
  $handler->display->display_options['filters']['relation_type']['expose']['identifier'] = 'relation_type';
  $handler->display->display_options['filters']['relation_type']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
  );
  $handler->display->display_options['filters']['relation_type']['expose']['reduce'] = TRUE;
  /* Filter criterion: Relation: Status (crm_core_relationship_status) */
  $handler->display->display_options['filters']['crm_core_relationship_status_value']['id'] = 'crm_core_relationship_status_value';
  $handler->display->display_options['filters']['crm_core_relationship_status_value']['table'] = 'field_data_crm_core_relationship_status';
  $handler->display->display_options['filters']['crm_core_relationship_status_value']['field'] = 'crm_core_relationship_status_value';
  $handler->display->display_options['filters']['crm_core_relationship_status_value']['value'] = array(
    1 => '1',
  );

  /* Display: Active */
  $handler = $view->new_display('page', 'Active', 'active');
  $handler->display->display_options['display_description'] = 'Active contact relationships';
  $handler->display->display_options['defaults']['hide_admin_links'] = FALSE;
  $handler->display->display_options['path'] = 'crm-core/contact/%/relationships/active';
  $handler->display->display_options['menu']['type'] = 'default tab';
  $handler->display->display_options['menu']['title'] = 'Active Relationships';
  $handler->display->display_options['menu']['weight'] = '0';
  $handler->display->display_options['menu']['context'] = 0;
  $handler->display->display_options['tab_options']['type'] = 'tab';
  $handler->display->display_options['tab_options']['title'] = 'Relationships';
  $handler->display->display_options['tab_options']['weight'] = '0';

  /* Display: Inactive */
  $handler = $view->new_display('page', 'Inactive', 'inactive');
  $handler->display->display_options['defaults']['hide_admin_links'] = FALSE;
  $handler->display->display_options['defaults']['header'] = FALSE;
  /* Header: Global: Result summary */
  $handler->display->display_options['header']['result']['id'] = 'result';
  $handler->display->display_options['header']['result']['table'] = 'views';
  $handler->display->display_options['header']['result']['field'] = 'result';
  $handler->display->display_options['header']['result']['content'] = 'Displaying @start - @end of @total inactive contact relationships';
  /* Header: Global: Text area */
  $handler->display->display_options['header']['area']['id'] = 'area';
  $handler->display->display_options['header']['area']['table'] = 'views';
  $handler->display->display_options['header']['area']['field'] = 'area';
  $handler->display->display_options['header']['area']['content'] = '<h2>Inactive Relationships</h2>';
  $handler->display->display_options['header']['area']['format'] = 'full_html';
  $handler->display->display_options['defaults']['empty'] = FALSE;
  /* No results behavior: Global: Text area */
  $handler->display->display_options['empty']['area']['id'] = 'area';
  $handler->display->display_options['empty']['area']['table'] = 'views';
  $handler->display->display_options['empty']['area']['field'] = 'area';
  $handler->display->display_options['empty']['area']['empty'] = TRUE;
  $handler->display->display_options['empty']['area']['content'] = 'No inactive relationships are stored in the system for this contact at this time.';
  $handler->display->display_options['empty']['area']['format'] = 'plain_text';
  $handler->display->display_options['defaults']['fields'] = FALSE;
  /* Field: Relation: Rid */
  $handler->display->display_options['fields']['rid']['id'] = 'rid';
  $handler->display->display_options['fields']['rid']['table'] = 'relation';
  $handler->display->display_options['fields']['rid']['field'] = 'rid';
  $handler->display->display_options['fields']['rid']['exclude'] = TRUE;
  /* Field: Relation: endpoints */
  $handler->display->display_options['fields']['endpoints']['id'] = 'endpoints';
  $handler->display->display_options['fields']['endpoints']['table'] = 'field_data_endpoints';
  $handler->display->display_options['fields']['endpoints']['field'] = 'endpoints';
  $handler->display->display_options['fields']['endpoints']['label'] = '';
  $handler->display->display_options['fields']['endpoints']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['endpoints']['hide_alter_empty'] = FALSE;
  $handler->display->display_options['fields']['endpoints']['click_sort_column'] = 'entity_type';
  $handler->display->display_options['fields']['endpoints']['type'] = 'contact_relationship_human';
  $handler->display->display_options['fields']['endpoints']['delta_offset'] = '0';
  /* Field: Relation: endpoints */
  $handler->display->display_options['fields']['endpoints_1']['id'] = 'endpoints_1';
  $handler->display->display_options['fields']['endpoints_1']['table'] = 'field_data_endpoints';
  $handler->display->display_options['fields']['endpoints_1']['field'] = 'endpoints';
  $handler->display->display_options['fields']['endpoints_1']['label'] = 'Contact ID';
  $handler->display->display_options['fields']['endpoints_1']['exclude'] = TRUE;
  $handler->display->display_options['fields']['endpoints_1']['alter']['alter_text'] = TRUE;
  $handler->display->display_options['fields']['endpoints_1']['alter']['text'] = '[endpoints_1-entity_id]';
  $handler->display->display_options['fields']['endpoints_1']['click_sort_column'] = 'entity_type';
  $handler->display->display_options['fields']['endpoints_1']['type'] = 'relation_endpoint_full';
  $handler->display->display_options['fields']['endpoints_1']['delta_offset'] = '0';
  /* Field: Relation: Status */
  $handler->display->display_options['fields']['crm_core_relationship_status_1']['id'] = 'crm_core_relationship_status_1';
  $handler->display->display_options['fields']['crm_core_relationship_status_1']['table'] = 'field_data_crm_core_relationship_status';
  $handler->display->display_options['fields']['crm_core_relationship_status_1']['field'] = 'crm_core_relationship_status';
  $handler->display->display_options['fields']['crm_core_relationship_status_1']['label'] = 'Status (Link)';
  $handler->display->display_options['fields']['crm_core_relationship_status_1']['exclude'] = TRUE;
  $handler->display->display_options['fields']['crm_core_relationship_status_1']['alter']['alter_text'] = TRUE;
  $handler->display->display_options['fields']['crm_core_relationship_status_1']['alter']['text'] = 'Mark as active';
  $handler->display->display_options['fields']['crm_core_relationship_status_1']['alter']['make_link'] = TRUE;
  $handler->display->display_options['fields']['crm_core_relationship_status_1']['alter']['path'] = 'crm-core/contact/[endpoints_1]/relationships/[rid]/active?destination=crm-core/contact/[endpoints_1]/relationships/inactive';
  $handler->display->display_options['fields']['crm_core_relationship_status_1']['type'] = 'list_key';
  /* Field: Relation: Delete link */
  $handler->display->display_options['fields']['delete']['id'] = 'delete';
  $handler->display->display_options['fields']['delete']['table'] = 'relation';
  $handler->display->display_options['fields']['delete']['field'] = 'delete';
  $handler->display->display_options['fields']['delete']['exclude'] = TRUE;
  /* Field: Global: Custom text */
  $handler->display->display_options['fields']['nothing']['id'] = 'nothing';
  $handler->display->display_options['fields']['nothing']['table'] = 'views';
  $handler->display->display_options['fields']['nothing']['field'] = 'nothing';
  $handler->display->display_options['fields']['nothing']['label'] = '';
  $handler->display->display_options['fields']['nothing']['alter']['text'] = '[delete] [crm_core_relationship_status_1]';
  $handler->display->display_options['fields']['nothing']['element_label_colon'] = FALSE;
  /* Field: Relation: endpoints */
  $handler->display->display_options['fields']['endpoints_2']['id'] = 'endpoints_2';
  $handler->display->display_options['fields']['endpoints_2']['table'] = 'field_data_endpoints';
  $handler->display->display_options['fields']['endpoints_2']['field'] = 'endpoints';
  $handler->display->display_options['fields']['endpoints_2']['label'] = 'Current contact';
  $handler->display->display_options['fields']['endpoints_2']['exclude'] = TRUE;
  $handler->display->display_options['fields']['endpoints_2']['click_sort_column'] = 'entity_type';
  $handler->display->display_options['fields']['endpoints_2']['type'] = 'contact_relationship_current';
  $handler->display->display_options['fields']['endpoints_2']['delta_offset'] = '0';
  $handler->display->display_options['defaults']['filter_groups'] = FALSE;
  $handler->display->display_options['defaults']['filters'] = FALSE;
  /* Filter criterion: Relation: Relation type */
  $handler->display->display_options['filters']['relation_type']['id'] = 'relation_type';
  $handler->display->display_options['filters']['relation_type']['table'] = 'relation';
  $handler->display->display_options['filters']['relation_type']['field'] = 'relation_type';
  $handler->display->display_options['filters']['relation_type']['value'] = array(
    'crm_employee' => 'crm_employee',
    'crm_friend' => 'crm_friend',
    'crm_member' => 'crm_member',
  );
  $handler->display->display_options['filters']['relation_type']['exposed'] = TRUE;
  $handler->display->display_options['filters']['relation_type']['expose']['operator_id'] = 'relation_type_op';
  $handler->display->display_options['filters']['relation_type']['expose']['label'] = 'Filter by:';
  $handler->display->display_options['filters']['relation_type']['expose']['operator'] = 'relation_type_op';
  $handler->display->display_options['filters']['relation_type']['expose']['identifier'] = 'relation_type';
  $handler->display->display_options['filters']['relation_type']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
  );
  $handler->display->display_options['filters']['relation_type']['expose']['reduce'] = TRUE;
  /* Filter criterion: Relation: Status (crm_core_relationship_status) */
  $handler->display->display_options['filters']['crm_core_relationship_status_value']['id'] = 'crm_core_relationship_status_value';
  $handler->display->display_options['filters']['crm_core_relationship_status_value']['table'] = 'field_data_crm_core_relationship_status';
  $handler->display->display_options['filters']['crm_core_relationship_status_value']['field'] = 'crm_core_relationship_status_value';
  $handler->display->display_options['filters']['crm_core_relationship_status_value']['value'] = array(
    0 => '0',
  );
  $handler->display->display_options['path'] = 'crm-core/contact/%/relationships/inactive';
  $handler->display->display_options['menu']['type'] = 'tab';
  $handler->display->display_options['menu']['title'] = 'Inactive Relationships';
  $handler->display->display_options['menu']['weight'] = '0';
  $handler->display->display_options['menu']['context'] = 0;

  $views['crm_relationships_by_contact'] = $view;

  return $views;
}


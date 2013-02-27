<?php

/**
 * Implements hook_views_default_views().
 */
function crm_core_activity_ui_views_default_views() {
  $view = new view();
  $view->name = 'crm_activity_types';
  $view->description = '';
  $view->tag = 'default';
  $view->base_table = 'crm_core_activity_type';
  $view->human_name = 'CRM Activity types';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Select activity type to create';
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'none';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['query']['options']['query_comment'] = FALSE;
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['style_plugin'] = 'list';
  $handler->display->display_options['style_options']['class'] = 'links type-list';
  $handler->display->display_options['row_plugin'] = 'fields';
  /* No results behavior: Global: Text area */
  $handler->display->display_options['empty']['area']['id'] = 'area';
  $handler->display->display_options['empty']['area']['table'] = 'views';
  $handler->display->display_options['empty']['area']['field'] = 'area';
  $handler->display->display_options['empty']['area']['content'] = 'It is not possible to create Activities yet. Please create Activity Type fist.';
  $handler->display->display_options['empty']['area']['format'] = 'filtered_html';
  /* Field: CRM Activity: Type */
  $handler->display->display_options['fields']['type']['id'] = 'type';
  $handler->display->display_options['fields']['type']['table'] = 'crm_core_activity_type';
  $handler->display->display_options['fields']['type']['field'] = 'type';
  $handler->display->display_options['fields']['type']['label'] = '';
  $handler->display->display_options['fields']['type']['exclude'] = TRUE;
  $handler->display->display_options['fields']['type']['alter']['text'] = 'hello world';
  $handler->display->display_options['fields']['type']['alter']['activity_create_link'] = 1;
  $handler->display->display_options['fields']['type']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['type']['hide_alter_empty'] = FALSE;
  /* Field: CRM Activity: Label */
  $handler->display->display_options['fields']['label']['id'] = 'label';
  $handler->display->display_options['fields']['label']['table'] = 'crm_core_activity_type';
  $handler->display->display_options['fields']['label']['field'] = 'label';
  $handler->display->display_options['fields']['label']['label'] = '';
  $handler->display->display_options['fields']['label']['alter']['make_link'] = TRUE;
  $handler->display->display_options['fields']['label']['alter']['path'] = '[type]';
  $handler->display->display_options['fields']['label']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['label']['hide_alter_empty'] = FALSE;
  /* Field: CRM Activity: Description */
  $handler->display->display_options['fields']['description']['id'] = 'description';
  $handler->display->display_options['fields']['description']['table'] = 'crm_core_activity_type';
  $handler->display->display_options['fields']['description']['field'] = 'description';
  $handler->display->display_options['fields']['description']['label'] = '';
  $handler->display->display_options['fields']['description']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['description']['hide_alter_empty'] = FALSE;
  /* Sort criterion: CRM Activity: Label */
  $handler->display->display_options['sorts']['label']['id'] = 'label';
  $handler->display->display_options['sorts']['label']['table'] = 'crm_core_activity_type';
  $handler->display->display_options['sorts']['label']['field'] = 'label';
  /* Contextual filter: Global: Null */
  $handler->display->display_options['arguments']['null']['id'] = 'null';
  $handler->display->display_options['arguments']['null']['table'] = 'views';
  $handler->display->display_options['arguments']['null']['field'] = 'null';
  $handler->display->display_options['arguments']['null']['default_action'] = 'not found';
  $handler->display->display_options['arguments']['null']['default_argument_type'] = 'node';
  $handler->display->display_options['arguments']['null']['summary']['number_of_records'] = '0';
  $handler->display->display_options['arguments']['null']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['null']['summary_options']['items_per_page'] = '25';

  /* Display: Page */
  $handler = $view->new_display('page', 'Page', 'page_1');
  $handler->display->display_options['defaults']['hide_admin_links'] = FALSE;
  $handler->display->display_options['path'] = 'crm/contact/%/activity/add';
  $handler->display->display_options['menu']['title'] = 'Add Activity';
  $handler->display->display_options['menu']['weight'] = '0';

  $views['crm_activity_types'] = $view;

  $view = new view();
  $view->name = 'crm_activities_by_contact';
  $view->description = 'List of activities of the contact';
  $view->tag = 'default';
  $view->base_table = 'crm_core_activity';
  $view->human_name = 'CRM Activities by contact';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Activities for [contact_name]';
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'none';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['query']['options']['query_comment'] = FALSE;
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '50';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['pager']['options']['id'] = '0';
  $handler->display->display_options['pager']['options']['quantity'] = '9';
  $handler->display->display_options['style_plugin'] = 'table';
  /* Header: Global: Result summary */
  $handler->display->display_options['header']['result']['id'] = 'result';
  $handler->display->display_options['header']['result']['table'] = 'views';
  $handler->display->display_options['header']['result']['field'] = 'result';
  $handler->display->display_options['header']['result']['content'] = 'Displaying @start - @end of @total activities for contact.';
  /* No results behavior: Global: Text area */
  $handler->display->display_options['empty']['area']['id'] = 'area';
  $handler->display->display_options['empty']['area']['table'] = 'views';
  $handler->display->display_options['empty']['area']['field'] = 'area';
  $handler->display->display_options['empty']['area']['content'] = 'This contact has no activity, add an activity above.';
  $handler->display->display_options['empty']['area']['format'] = 'filtered_html';
  /* Relationship: Entity Reference: Referenced Entity */
  $handler->display->display_options['relationships']['field_activity_participants_target_id']['id'] = 'field_activity_participants_target_id';
  $handler->display->display_options['relationships']['field_activity_participants_target_id']['table'] = 'field_data_field_activity_participants';
  $handler->display->display_options['relationships']['field_activity_participants_target_id']['field'] = 'field_activity_participants_target_id';
  /* Field: CRM Activity: Activity ID */
  $handler->display->display_options['fields']['activity_id']['id'] = 'activity_id';
  $handler->display->display_options['fields']['activity_id']['table'] = 'crm_core_activity';
  $handler->display->display_options['fields']['activity_id']['field'] = 'activity_id';
  $handler->display->display_options['fields']['activity_id']['label'] = '';
  $handler->display->display_options['fields']['activity_id']['exclude'] = TRUE;
  $handler->display->display_options['fields']['activity_id']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['activity_id']['hide_alter_empty'] = FALSE;
  /* Field: CRM Activity: Date */
  $handler->display->display_options['fields']['field_activity_date']['id'] = 'field_activity_date';
  $handler->display->display_options['fields']['field_activity_date']['table'] = 'field_data_field_activity_date';
  $handler->display->display_options['fields']['field_activity_date']['field'] = 'field_activity_date';
  $handler->display->display_options['fields']['field_activity_date']['label'] = 'Activity Date';
  $handler->display->display_options['fields']['field_activity_date']['settings'] = array(
    'format_type' => 'long',
    'fromto' => 'both',
    'multiple_number' => '',
    'multiple_from' => '',
    'multiple_to' => '',
  );
  /* Field: CRM Activity: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'crm_core_activity';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['alter']['make_link'] = TRUE;
  $handler->display->display_options['fields']['title']['alter']['path'] = 'crm/activity/[activity_id]';
  $handler->display->display_options['fields']['title']['hide_alter_empty'] = FALSE;
  /* Field: CRM Activity: Type */
  $handler->display->display_options['fields']['type']['id'] = 'type';
  $handler->display->display_options['fields']['type']['table'] = 'crm_core_activity';
  $handler->display->display_options['fields']['type']['field'] = 'type';
  $handler->display->display_options['fields']['type']['label'] = 'Activity Type';
  /* Field: CRM Activity: Link */
  $handler->display->display_options['fields']['view_crm_activity']['id'] = 'view_crm_activity';
  $handler->display->display_options['fields']['view_crm_activity']['table'] = 'crm_core_activity';
  $handler->display->display_options['fields']['view_crm_activity']['field'] = 'view_crm_activity';
  $handler->display->display_options['fields']['view_crm_activity']['label'] = '';
  $handler->display->display_options['fields']['view_crm_activity']['exclude'] = TRUE;
  $handler->display->display_options['fields']['view_crm_activity']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['view_crm_activity']['hide_alter_empty'] = FALSE;
  $handler->display->display_options['fields']['view_crm_activity']['text'] = 'View';
  /* Field: CRM Activity: Edit link */
  $handler->display->display_options['fields']['edit_crm_activity']['id'] = 'edit_crm_activity';
  $handler->display->display_options['fields']['edit_crm_activity']['table'] = 'crm_core_activity';
  $handler->display->display_options['fields']['edit_crm_activity']['field'] = 'edit_crm_activity';
  $handler->display->display_options['fields']['edit_crm_activity']['label'] = '';
  $handler->display->display_options['fields']['edit_crm_activity']['exclude'] = TRUE;
  $handler->display->display_options['fields']['edit_crm_activity']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['edit_crm_activity']['hide_alter_empty'] = FALSE;
  $handler->display->display_options['fields']['edit_crm_activity']['text'] = 'Edit';
  $handler->display->display_options['fields']['edit_crm_activity']['redirect'] = 1;
  /* Field: CRM Activity: Delete link */
  $handler->display->display_options['fields']['delete_crm_activity']['id'] = 'delete_crm_activity';
  $handler->display->display_options['fields']['delete_crm_activity']['table'] = 'crm_core_activity';
  $handler->display->display_options['fields']['delete_crm_activity']['field'] = 'delete_crm_activity';
  $handler->display->display_options['fields']['delete_crm_activity']['label'] = '';
  $handler->display->display_options['fields']['delete_crm_activity']['exclude'] = TRUE;
  $handler->display->display_options['fields']['delete_crm_activity']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['delete_crm_activity']['hide_alter_empty'] = FALSE;
  $handler->display->display_options['fields']['delete_crm_activity']['text'] = 'Delete';
  $handler->display->display_options['fields']['delete_crm_activity']['redirect'] = 1;
  /* Field: Global: Custom text */
  $handler->display->display_options['fields']['nothing']['id'] = 'nothing';
  $handler->display->display_options['fields']['nothing']['table'] = 'views';
  $handler->display->display_options['fields']['nothing']['field'] = 'nothing';
  $handler->display->display_options['fields']['nothing']['label'] = 'Operations';
  $handler->display->display_options['fields']['nothing']['alter']['text'] = '[view_crm_activity] | [edit_crm_activity] | [delete_crm_activity]';
  /* Field: CRM Contact: Household name */
  $handler->display->display_options['fields']['contact_name']['id'] = 'contact_name';
  $handler->display->display_options['fields']['contact_name']['table'] = 'field_data_contact_name';
  $handler->display->display_options['fields']['contact_name']['field'] = 'contact_name';
  $handler->display->display_options['fields']['contact_name']['relationship'] = 'field_activity_participants_target_id';
  $handler->display->display_options['fields']['contact_name']['exclude'] = TRUE;
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
  /* Contextual filter: CRM Activity: Participants (field_activity_participants) */
  $handler->display->display_options['arguments']['field_activity_participants_target_id']['id'] = 'field_activity_participants_target_id';
  $handler->display->display_options['arguments']['field_activity_participants_target_id']['table'] = 'field_data_field_activity_participants';
  $handler->display->display_options['arguments']['field_activity_participants_target_id']['field'] = 'field_activity_participants_target_id';
  $handler->display->display_options['arguments']['field_activity_participants_target_id']['default_action'] = 'not found';
  $handler->display->display_options['arguments']['field_activity_participants_target_id']['default_argument_type'] = 'fixed';
  $handler->display->display_options['arguments']['field_activity_participants_target_id']['summary']['number_of_records'] = '0';
  $handler->display->display_options['arguments']['field_activity_participants_target_id']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['field_activity_participants_target_id']['summary_options']['items_per_page'] = '25';
  /* Filter criterion: CRM Activity: Type */
  $handler->display->display_options['filters']['type']['id'] = 'type';
  $handler->display->display_options['filters']['type']['table'] = 'crm_core_activity';
  $handler->display->display_options['filters']['type']['field'] = 'type';
  $handler->display->display_options['filters']['type']['value'] = array(
    'meeting' => 'meeting',
    'phone_call' => 'phone_call',
  );
  $handler->display->display_options['filters']['type']['exposed'] = TRUE;
  $handler->display->display_options['filters']['type']['expose']['operator_id'] = 'type_op';
  $handler->display->display_options['filters']['type']['expose']['label'] = 'Activity type';
  $handler->display->display_options['filters']['type']['expose']['operator'] = 'type_op';
  $handler->display->display_options['filters']['type']['expose']['identifier'] = 'type';
  $handler->display->display_options['filters']['type']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
  );
  $handler->display->display_options['filters']['type']['expose']['reduce'] = TRUE;

  /* Display: Page */
  $handler = $view->new_display('page', 'Page', 'page');
  $handler->display->display_options['defaults']['hide_admin_links'] = FALSE;
  $handler->display->display_options['path'] = 'crm/contact/%/activity';

  $views['crm_activities_by_contact'] = $view;

  return $views;
}


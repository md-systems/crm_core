<?php

/**
 * Implements hook_views_default_views().
 */
function crm_core_activity_views_default_views() {
  $view = new view;
  $view->name = 'crm_activity_types';
  $view->description = '';
  $view->tag = 'default';
  $view->base_table = 'crm_core_activity_type';
  $view->human_name = 'CRM Activity types';
  $view->core = 7;
  $view->api_version = '3.0-alpha1';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Select activity type to create';
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
  $handler->display->display_options['empty']['area']['empty'] = FALSE;
  $handler->display->display_options['empty']['area']['content'] = 'It is not possible to create Activities yet. Please create Activity Type fist.';
  $handler->display->display_options['empty']['area']['format'] = 'filtered_html';
  $handler->display->display_options['empty']['area']['tokenize'] = 0;
  /* Field: CRM Activity: Type */
  $handler->display->display_options['fields']['type']['id'] = 'type';
  $handler->display->display_options['fields']['type']['table'] = 'crm_core_activity_type';
  $handler->display->display_options['fields']['type']['field'] = 'type';
  $handler->display->display_options['fields']['type']['label'] = '';
  $handler->display->display_options['fields']['type']['exclude'] = TRUE;
  $handler->display->display_options['fields']['type']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['type']['alter']['text'] = 'hello world';
  $handler->display->display_options['fields']['type']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['type']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['type']['alter']['external'] = 0;
  $handler->display->display_options['fields']['type']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['type']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['type']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['type']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['type']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['type']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['type']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['type']['alter']['html'] = 0;
  $handler->display->display_options['fields']['type']['alter']['activity_create_link'] = 1;
  $handler->display->display_options['fields']['type']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['type']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['type']['hide_empty'] = 0;
  $handler->display->display_options['fields']['type']['empty_zero'] = 0;
  $handler->display->display_options['fields']['type']['hide_alter_empty'] = 0;
  /* Field: CRM Activity: Label */
  $handler->display->display_options['fields']['label']['id'] = 'label';
  $handler->display->display_options['fields']['label']['table'] = 'crm_core_activity_type';
  $handler->display->display_options['fields']['label']['field'] = 'label';
  $handler->display->display_options['fields']['label']['label'] = '';
  $handler->display->display_options['fields']['label']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['label']['alter']['make_link'] = 1;
  $handler->display->display_options['fields']['label']['alter']['path'] = '[type]';
  $handler->display->display_options['fields']['label']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['label']['alter']['external'] = 0;
  $handler->display->display_options['fields']['label']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['label']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['label']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['label']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['label']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['label']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['label']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['label']['alter']['html'] = 0;
  $handler->display->display_options['fields']['label']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['label']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['label']['hide_empty'] = 0;
  $handler->display->display_options['fields']['label']['empty_zero'] = 0;
  $handler->display->display_options['fields']['label']['hide_alter_empty'] = 0;
  /* Field: CRM Activity: Description */
  $handler->display->display_options['fields']['description']['id'] = 'description';
  $handler->display->display_options['fields']['description']['table'] = 'crm_core_activity_type';
  $handler->display->display_options['fields']['description']['field'] = 'description';
  $handler->display->display_options['fields']['description']['label'] = '';
  $handler->display->display_options['fields']['description']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['description']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['description']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['description']['alter']['external'] = 0;
  $handler->display->display_options['fields']['description']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['description']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['description']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['description']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['description']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['description']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['description']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['description']['alter']['html'] = 0;
  $handler->display->display_options['fields']['description']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['description']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['description']['hide_empty'] = 0;
  $handler->display->display_options['fields']['description']['empty_zero'] = 0;
  $handler->display->display_options['fields']['description']['hide_alter_empty'] = 0;
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
  $handler->display->display_options['arguments']['null']['default_argument_skip_url'] = 0;
  $handler->display->display_options['arguments']['null']['summary']['number_of_records'] = '0';
  $handler->display->display_options['arguments']['null']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['null']['summary_options']['items_per_page'] = '25';
  $handler->display->display_options['arguments']['null']['must_not_be'] = 0;

  /* Display: Page */
  $handler = $view->new_display('page', 'Page', 'page_1');
  $handler->display->display_options['path'] = 'crm/contact/%/activity/add';
  $handler->display->display_options['menu']['title'] = 'Add Activity';
  $handler->display->display_options['menu']['weight'] = '0';

  $views['crm_activity_types'] = $view;

  $view = new view;
  $view->name = 'crm_activities_by_contact';
  $view->description = 'List of activities of the contact';
  $view->tag = 'default';
  $view->base_table = 'crm_core_activity';
  $view->human_name = 'CRM Activities by contact';
  $view->core = 7;
  $view->api_version = '3.0-alpha1';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Activities by contact';
  $handler->display->display_options['access']['type'] = 'none';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['query']['options']['query_comment'] = FALSE;
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '10';
  $handler->display->display_options['style_plugin'] = 'table';
  /* No results behavior: Global: Text area */
  $handler->display->display_options['empty']['area']['id'] = 'area';
  $handler->display->display_options['empty']['area']['table'] = 'views';
  $handler->display->display_options['empty']['area']['field'] = 'area';
  $handler->display->display_options['empty']['area']['empty'] = FALSE;
  $handler->display->display_options['empty']['area']['content'] = 'This contact has no activity, add an activity above.';
  $handler->display->display_options['empty']['area']['format'] = 'filtered_html';
  $handler->display->display_options['empty']['area']['tokenize'] = 0;
  /* Field: Global: View result counter */
  $handler->display->display_options['fields']['counter']['id'] = 'counter';
  $handler->display->display_options['fields']['counter']['table'] = 'views';
  $handler->display->display_options['fields']['counter']['field'] = 'counter';
  $handler->display->display_options['fields']['counter']['label'] = '';
  $handler->display->display_options['fields']['counter']['exclude'] = TRUE;
  $handler->display->display_options['fields']['counter']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['counter']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['counter']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['counter']['alter']['external'] = 0;
  $handler->display->display_options['fields']['counter']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['counter']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['counter']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['counter']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['counter']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['counter']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['counter']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['counter']['alter']['html'] = 0;
  $handler->display->display_options['fields']['counter']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['counter']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['counter']['hide_empty'] = 0;
  $handler->display->display_options['fields']['counter']['empty_zero'] = 0;
  $handler->display->display_options['fields']['counter']['hide_alter_empty'] = 0;
  $handler->display->display_options['fields']['counter']['counter_start'] = '1';
  /* Field: Activity: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'crm_core_activity';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['title']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['title']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['title']['alter']['external'] = 0;
  $handler->display->display_options['fields']['title']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['title']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['title']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['title']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['title']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['title']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['title']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['title']['alter']['html'] = 0;
  $handler->display->display_options['fields']['title']['element_label_colon'] = 1;
  $handler->display->display_options['fields']['title']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['title']['hide_empty'] = 0;
  $handler->display->display_options['fields']['title']['empty_zero'] = 0;
  $handler->display->display_options['fields']['title']['hide_alter_empty'] = 0;
  /* Field: Activity: Activity ID */
  $handler->display->display_options['fields']['activity_id']['id'] = 'activity_id';
  $handler->display->display_options['fields']['activity_id']['table'] = 'crm_core_activity';
  $handler->display->display_options['fields']['activity_id']['field'] = 'activity_id';
  $handler->display->display_options['fields']['activity_id']['label'] = '';
  $handler->display->display_options['fields']['activity_id']['exclude'] = TRUE;
  $handler->display->display_options['fields']['activity_id']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['activity_id']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['activity_id']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['activity_id']['alter']['external'] = 0;
  $handler->display->display_options['fields']['activity_id']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['activity_id']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['activity_id']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['activity_id']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['activity_id']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['activity_id']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['activity_id']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['activity_id']['alter']['html'] = 0;
  $handler->display->display_options['fields']['activity_id']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['activity_id']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['activity_id']['hide_empty'] = 0;
  $handler->display->display_options['fields']['activity_id']['empty_zero'] = 0;
  $handler->display->display_options['fields']['activity_id']['hide_alter_empty'] = 0;
  $handler->display->display_options['fields']['activity_id']['format_plural'] = 0;
  /* Field: Activity: Participants */
  $handler->display->display_options['fields']['field_activity_participants']['id'] = 'field_activity_participants';
  $handler->display->display_options['fields']['field_activity_participants']['table'] = 'field_data_field_activity_participants';
  $handler->display->display_options['fields']['field_activity_participants']['field'] = 'field_activity_participants';
  $handler->display->display_options['fields']['field_activity_participants']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['field_activity_participants']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['field_activity_participants']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['field_activity_participants']['alter']['external'] = 0;
  $handler->display->display_options['fields']['field_activity_participants']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['field_activity_participants']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['field_activity_participants']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['field_activity_participants']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['field_activity_participants']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['field_activity_participants']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['field_activity_participants']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['field_activity_participants']['alter']['html'] = 0;
  $handler->display->display_options['fields']['field_activity_participants']['element_label_colon'] = 1;
  $handler->display->display_options['fields']['field_activity_participants']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['field_activity_participants']['hide_empty'] = 0;
  $handler->display->display_options['fields']['field_activity_participants']['empty_zero'] = 0;
  $handler->display->display_options['fields']['field_activity_participants']['hide_alter_empty'] = 0;
  $handler->display->display_options['fields']['field_activity_participants']['click_sort_column'] = 'target_id';
  $handler->display->display_options['fields']['field_activity_participants']['settings'] = array(
    'link' => 0,
  );
  $handler->display->display_options['fields']['field_activity_participants']['group_rows'] = 1;
  $handler->display->display_options['fields']['field_activity_participants']['delta_offset'] = '0';
  $handler->display->display_options['fields']['field_activity_participants']['delta_reversed'] = 0;
  $handler->display->display_options['fields']['field_activity_participants']['field_api_classes'] = 0;
  /* Field: Activity: Link */
  $handler->display->display_options['fields']['view_crm_activity']['id'] = 'view_crm_activity';
  $handler->display->display_options['fields']['view_crm_activity']['table'] = 'crm_core_activity';
  $handler->display->display_options['fields']['view_crm_activity']['field'] = 'view_crm_activity';
  $handler->display->display_options['fields']['view_crm_activity']['label'] = '';
  $handler->display->display_options['fields']['view_crm_activity']['exclude'] = TRUE;
  $handler->display->display_options['fields']['view_crm_activity']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['view_crm_activity']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['view_crm_activity']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['view_crm_activity']['alter']['external'] = 0;
  $handler->display->display_options['fields']['view_crm_activity']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['view_crm_activity']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['view_crm_activity']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['view_crm_activity']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['view_crm_activity']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['view_crm_activity']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['view_crm_activity']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['view_crm_activity']['alter']['html'] = 0;
  $handler->display->display_options['fields']['view_crm_activity']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['view_crm_activity']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['view_crm_activity']['hide_empty'] = 0;
  $handler->display->display_options['fields']['view_crm_activity']['empty_zero'] = 0;
  $handler->display->display_options['fields']['view_crm_activity']['hide_alter_empty'] = 0;
  $handler->display->display_options['fields']['view_crm_activity']['text'] = 'View';
  /* Field: Activity: Edit link */
  $handler->display->display_options['fields']['edit_crm_activity']['id'] = 'edit_crm_activity';
  $handler->display->display_options['fields']['edit_crm_activity']['table'] = 'crm_core_activity';
  $handler->display->display_options['fields']['edit_crm_activity']['field'] = 'edit_crm_activity';
  $handler->display->display_options['fields']['edit_crm_activity']['label'] = '';
  $handler->display->display_options['fields']['edit_crm_activity']['exclude'] = TRUE;
  $handler->display->display_options['fields']['edit_crm_activity']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['edit_crm_activity']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['edit_crm_activity']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['edit_crm_activity']['alter']['external'] = 0;
  $handler->display->display_options['fields']['edit_crm_activity']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['edit_crm_activity']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['edit_crm_activity']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['edit_crm_activity']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['edit_crm_activity']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['edit_crm_activity']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['edit_crm_activity']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['edit_crm_activity']['alter']['html'] = 0;
  $handler->display->display_options['fields']['edit_crm_activity']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['edit_crm_activity']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['edit_crm_activity']['hide_empty'] = 0;
  $handler->display->display_options['fields']['edit_crm_activity']['empty_zero'] = 0;
  $handler->display->display_options['fields']['edit_crm_activity']['hide_alter_empty'] = 0;
  $handler->display->display_options['fields']['edit_crm_activity']['text'] = 'Edit';
  $handler->display->display_options['fields']['edit_crm_activity']['redirect'] = 1;
  /* Field: Activity: Delete link */
  $handler->display->display_options['fields']['delete_crm_activity']['id'] = 'delete_crm_activity';
  $handler->display->display_options['fields']['delete_crm_activity']['table'] = 'crm_core_activity';
  $handler->display->display_options['fields']['delete_crm_activity']['field'] = 'delete_crm_activity';
  $handler->display->display_options['fields']['delete_crm_activity']['label'] = '';
  $handler->display->display_options['fields']['delete_crm_activity']['exclude'] = TRUE;
  $handler->display->display_options['fields']['delete_crm_activity']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['delete_crm_activity']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['delete_crm_activity']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['delete_crm_activity']['alter']['external'] = 0;
  $handler->display->display_options['fields']['delete_crm_activity']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['delete_crm_activity']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['delete_crm_activity']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['delete_crm_activity']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['delete_crm_activity']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['delete_crm_activity']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['delete_crm_activity']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['delete_crm_activity']['alter']['html'] = 0;
  $handler->display->display_options['fields']['delete_crm_activity']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['delete_crm_activity']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['delete_crm_activity']['hide_empty'] = 0;
  $handler->display->display_options['fields']['delete_crm_activity']['empty_zero'] = 0;
  $handler->display->display_options['fields']['delete_crm_activity']['hide_alter_empty'] = 0;
  $handler->display->display_options['fields']['delete_crm_activity']['text'] = 'Delete';
  $handler->display->display_options['fields']['delete_crm_activity']['redirect'] = 1;
  /* Field: Global: Custom text */
  $handler->display->display_options['fields']['nothing']['id'] = 'nothing';
  $handler->display->display_options['fields']['nothing']['table'] = 'views';
  $handler->display->display_options['fields']['nothing']['field'] = 'nothing';
  $handler->display->display_options['fields']['nothing']['label'] = 'Operations';
  $handler->display->display_options['fields']['nothing']['alter']['text'] = '<div class="ctools-no-js ctools-dropbutton ctools-button" id="ctools-button-[counter]">
  <div class="ctools-link"><a href="#" class="ctools-twisty ctools-text">open</a></div>
  <div class="ctools-content">
  <ul>
          <li>[view_crm_activity]</li>
          <li>[edit_crm_activity]</li>
          <li>[delete_crm_activity]</li>
  </ul>
  </div>
  </div>';
  $handler->display->display_options['fields']['nothing']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['nothing']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['nothing']['alter']['external'] = 0;
  $handler->display->display_options['fields']['nothing']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['nothing']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['nothing']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['nothing']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['nothing']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['nothing']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['nothing']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['nothing']['alter']['html'] = 0;
  $handler->display->display_options['fields']['nothing']['element_label_colon'] = 1;
  $handler->display->display_options['fields']['nothing']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['nothing']['hide_empty'] = 0;
  $handler->display->display_options['fields']['nothing']['empty_zero'] = 0;
  $handler->display->display_options['fields']['nothing']['hide_alter_empty'] = 0;
  /* Contextual filter: Activity: Participants (field_activity_participants:target_id) */
  $handler->display->display_options['arguments']['field_activity_participants_target_id']['id'] = 'field_activity_participants_target_id';
  $handler->display->display_options['arguments']['field_activity_participants_target_id']['table'] = 'field_data_field_activity_participants';
  $handler->display->display_options['arguments']['field_activity_participants_target_id']['field'] = 'field_activity_participants_target_id';
  $handler->display->display_options['arguments']['field_activity_participants_target_id']['default_action'] = 'not found';
  $handler->display->display_options['arguments']['field_activity_participants_target_id']['default_argument_type'] = 'fixed';
  $handler->display->display_options['arguments']['field_activity_participants_target_id']['default_argument_skip_url'] = 0;
  $handler->display->display_options['arguments']['field_activity_participants_target_id']['summary']['number_of_records'] = '0';
  $handler->display->display_options['arguments']['field_activity_participants_target_id']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['field_activity_participants_target_id']['summary_options']['items_per_page'] = '25';
  $handler->display->display_options['arguments']['field_activity_participants_target_id']['break_phrase'] = 0;
  $handler->display->display_options['arguments']['field_activity_participants_target_id']['not'] = 0;

  /* Display: Page */
  $handler = $view->new_display('page', 'Page', 'page');
  $handler->display->display_options['path'] = 'crm/contact/%/activity';

  $views['crm_activities_by_contact'] = $view;

  return $views;
}


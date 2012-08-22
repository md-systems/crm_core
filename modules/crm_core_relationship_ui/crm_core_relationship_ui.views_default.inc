<?php

/**
 * Implements hook_views_default_views().
 */
function crm_core_relationship_ui_views_default_views() {
  $view = new view;
  $view->name = 'crm_relationships_by_contact';
  $view->description = '';
  $view->tag = 'default';
  $view->base_table = 'relation';
  $view->human_name = 'CRM Relationships by contact';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Relationships';
  $handler->display->display_options['access']['type'] = 'none';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['query']['options']['query_comment'] = FALSE;
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '10';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['pager']['options']['id'] = '0';
  $handler->display->display_options['pager']['options']['expose']['items_per_page_options_all'] = 0;
  $handler->display->display_options['style_plugin'] = 'table';
  /* Field: Relation: endpoints */
  $handler->display->display_options['fields']['endpoints']['id'] = 'endpoints';
  $handler->display->display_options['fields']['endpoints']['table'] = 'field_data_endpoints';
  $handler->display->display_options['fields']['endpoints']['field'] = 'endpoints';
  $handler->display->display_options['fields']['endpoints']['label'] = 'Contact';
  $handler->display->display_options['fields']['endpoints']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['endpoints']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['endpoints']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['endpoints']['alter']['external'] = 0;
  $handler->display->display_options['fields']['endpoints']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['endpoints']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['endpoints']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['endpoints']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['endpoints']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['endpoints']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['endpoints']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['endpoints']['alter']['html'] = 0;
  $handler->display->display_options['fields']['endpoints']['element_label_colon'] = 1;
  $handler->display->display_options['fields']['endpoints']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['endpoints']['hide_empty'] = 0;
  $handler->display->display_options['fields']['endpoints']['empty_zero'] = 0;
  $handler->display->display_options['fields']['endpoints']['hide_alter_empty'] = 0;
  $handler->display->display_options['fields']['endpoints']['click_sort_column'] = 'entity_type';
  $handler->display->display_options['fields']['endpoints']['type'] = 'contact_relationship_view';
  $handler->display->display_options['fields']['endpoints']['group_rows'] = 1;
  $handler->display->display_options['fields']['endpoints']['delta_offset'] = '0';
  $handler->display->display_options['fields']['endpoints']['delta_reversed'] = 0;
  $handler->display->display_options['fields']['endpoints']['field_api_classes'] = 0;
  /* Field: Relation: Relation type */
  $handler->display->display_options['fields']['relation_type']['id'] = 'relation_type';
  $handler->display->display_options['fields']['relation_type']['table'] = 'relation';
  $handler->display->display_options['fields']['relation_type']['field'] = 'relation_type';
  $handler->display->display_options['fields']['relation_type']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['relation_type']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['relation_type']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['relation_type']['alter']['external'] = 0;
  $handler->display->display_options['fields']['relation_type']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['relation_type']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['relation_type']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['relation_type']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['relation_type']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['relation_type']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['relation_type']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['relation_type']['alter']['html'] = 0;
  $handler->display->display_options['fields']['relation_type']['element_label_colon'] = 1;
  $handler->display->display_options['fields']['relation_type']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['relation_type']['hide_empty'] = 0;
  $handler->display->display_options['fields']['relation_type']['empty_zero'] = 0;
  $handler->display->display_options['fields']['relation_type']['hide_alter_empty'] = 0;
  /* Field: Relation: Create Date */
  $handler->display->display_options['fields']['created']['id'] = 'created';
  $handler->display->display_options['fields']['created']['table'] = 'relation';
  $handler->display->display_options['fields']['created']['field'] = 'created';
  $handler->display->display_options['fields']['created']['label'] = 'Created';
  $handler->display->display_options['fields']['created']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['created']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['created']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['created']['alter']['external'] = 0;
  $handler->display->display_options['fields']['created']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['created']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['created']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['created']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['created']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['created']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['created']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['created']['alter']['html'] = 0;
  $handler->display->display_options['fields']['created']['element_label_colon'] = 1;
  $handler->display->display_options['fields']['created']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['created']['hide_empty'] = 0;
  $handler->display->display_options['fields']['created']['empty_zero'] = 0;
  $handler->display->display_options['fields']['created']['hide_alter_empty'] = 0;
  $handler->display->display_options['fields']['created']['date_format'] = 'short';
  /* Field: Relation: Rid */
  $handler->display->display_options['fields']['rid']['id'] = 'rid';
  $handler->display->display_options['fields']['rid']['table'] = 'relation';
  $handler->display->display_options['fields']['rid']['field'] = 'rid';
  $handler->display->display_options['fields']['rid']['label'] = '';
  $handler->display->display_options['fields']['rid']['exclude'] = TRUE;
  $handler->display->display_options['fields']['rid']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['rid']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['rid']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['rid']['alter']['external'] = 0;
  $handler->display->display_options['fields']['rid']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['rid']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['rid']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['rid']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['rid']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['rid']['alter']['more_link'] = 0;
  $handler->display->display_options['fields']['rid']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['rid']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['rid']['alter']['html'] = 0;
  $handler->display->display_options['fields']['rid']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['rid']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['rid']['hide_empty'] = 0;
  $handler->display->display_options['fields']['rid']['empty_zero'] = 0;
  $handler->display->display_options['fields']['rid']['hide_alter_empty'] = 0;
  $handler->display->display_options['fields']['rid']['separator'] = '';
  $handler->display->display_options['fields']['rid']['format_plural'] = 0;
  /* Field: Global: Custom text */
  $handler->display->display_options['fields']['nothing']['id'] = 'nothing';
  $handler->display->display_options['fields']['nothing']['table'] = 'views';
  $handler->display->display_options['fields']['nothing']['field'] = 'nothing';
  $handler->display->display_options['fields']['nothing']['label'] = 'Actions';
  $handler->display->display_options['fields']['nothing']['alter']['text'] = 'edit';
  $handler->display->display_options['fields']['nothing']['alter']['make_link'] = 1;
  $handler->display->display_options['fields']['nothing']['alter']['path'] = 'relation/[rid]/edit';
  $handler->display->display_options['fields']['nothing']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['nothing']['alter']['external'] = 0;
  $handler->display->display_options['fields']['nothing']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['nothing']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['nothing']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['nothing']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['nothing']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['nothing']['alter']['more_link'] = 0;
  $handler->display->display_options['fields']['nothing']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['nothing']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['nothing']['alter']['html'] = 0;
  $handler->display->display_options['fields']['nothing']['element_label_colon'] = 1;
  $handler->display->display_options['fields']['nothing']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['nothing']['hide_empty'] = 0;
  $handler->display->display_options['fields']['nothing']['empty_zero'] = 0;
  $handler->display->display_options['fields']['nothing']['hide_alter_empty'] = 0;
  /* Contextual filter: Relation: endpoints (endpoints:entity_id) */
  $handler->display->display_options['arguments']['endpoints_entity_id']['id'] = 'endpoints_entity_id';
  $handler->display->display_options['arguments']['endpoints_entity_id']['table'] = 'field_data_endpoints';
  $handler->display->display_options['arguments']['endpoints_entity_id']['field'] = 'endpoints_entity_id';
  $handler->display->display_options['arguments']['endpoints_entity_id']['default_action'] = 'default';
  $handler->display->display_options['arguments']['endpoints_entity_id']['default_argument_type'] = 'php';
  $handler->display->display_options['arguments']['endpoints_entity_id']['default_argument_options']['code'] = 'return arg(2);';
  $handler->display->display_options['arguments']['endpoints_entity_id']['default_argument_skip_url'] = 0;
  $handler->display->display_options['arguments']['endpoints_entity_id']['summary']['number_of_records'] = '0';
  $handler->display->display_options['arguments']['endpoints_entity_id']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['endpoints_entity_id']['summary_options']['items_per_page'] = '25';
  $handler->display->display_options['arguments']['endpoints_entity_id']['break_phrase'] = 0;
  $handler->display->display_options['arguments']['endpoints_entity_id']['not'] = 0;
  /* Contextual filter: Relation: Relation type */
  $handler->display->display_options['arguments']['relation_type']['id'] = 'relation_type';
  $handler->display->display_options['arguments']['relation_type']['table'] = 'relation';
  $handler->display->display_options['arguments']['relation_type']['field'] = 'relation_type';
  $handler->display->display_options['arguments']['relation_type']['default_action'] = 'default';
  $handler->display->display_options['arguments']['relation_type']['default_argument_type'] = 'php';
  $handler->display->display_options['arguments']['relation_type']['default_argument_options']['code'] = 'return implode(\',\',array_keys(crm_core_relationship_get_relationships()));';
  $handler->display->display_options['arguments']['relation_type']['default_argument_skip_url'] = 0;
  $handler->display->display_options['arguments']['relation_type']['summary']['number_of_records'] = '0';
  $handler->display->display_options['arguments']['relation_type']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['relation_type']['summary_options']['items_per_page'] = '25';
  $handler->display->display_options['arguments']['relation_type']['glossary'] = 0;
  $handler->display->display_options['arguments']['relation_type']['limit'] = '0';
  $handler->display->display_options['arguments']['relation_type']['transform_dash'] = 0;
  $handler->display->display_options['arguments']['relation_type']['break_phrase'] = 1;

  /* Display: Block. Active Relationships. */
  $handler = $view->new_display('block', 'Block. Active Relationships.', 'block_1');

  /* Display: Block. Inactive Relationships. */
  $handler = $view->new_display('block', 'Block. Inactive Relationships.', 'block_2');
  $handler->display->display_options['defaults']['filters'] = FALSE;

  $views['crm_relationships_by_contact'] = $view;

  return $views;
}


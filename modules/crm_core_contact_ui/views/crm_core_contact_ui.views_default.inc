<?php
// $Id$

/**
 * Views for the default CRM UI.
 */

/**
 * Implements hook_views_default_views().
 */
function crm_core_contact_ui_views_default_views() {

$view = new view;
$view->name = 'crm_core_contacts';
$view->description = 'A list of all contacts in the system';
$view->tag = '';
$view->base_table = 'crm_core_contact';
$view->human_name = '';
$view->core = 0;
$view->api_version = '3.0';
$view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

/* Display: Defaults */
$handler = $view->new_display('default', 'Defaults', 'default');
$handler->display->display_options['title'] = 'CRM Contact';
$handler->display->display_options['access']['type'] = 'perm';
$handler->display->display_options['access']['perm'] = 'view any crm_core_contact entity';
$handler->display->display_options['cache']['type'] = 'none';
$handler->display->display_options['query']['type'] = 'views_query';
$handler->display->display_options['query']['options']['query_comment'] = FALSE;
$handler->display->display_options['exposed_form']['type'] = 'basic';
$handler->display->display_options['pager']['type'] = 'full';
$handler->display->display_options['style_plugin'] = 'table';
$handler->display->display_options['style_options']['columns'] = array(
  'contact_id' => 'contact_id',
  'contact_image' => 'contact_image',
  'entity_id' => 'entity_id',
  'type' => 'type',
  'created' => 'created',
);
$handler->display->display_options['style_options']['default'] = 'created';
$handler->display->display_options['style_options']['info'] = array(
  'contact_id' => array(
    'sortable' => 0,
    'default_sort_order' => 'asc',
    'align' => '',
    'separator' => '',
  ),
  'contact_image' => array(
    'sortable' => 0,
    'default_sort_order' => 'asc',
    'align' => '',
    'separator' => '',
  ),
  'entity_id' => array(
    'sortable' => 1,
    'default_sort_order' => 'asc',
    'align' => '',
    'separator' => '',
  ),
  'type' => array(
    'sortable' => 1,
    'default_sort_order' => 'asc',
    'align' => '',
    'separator' => '',
  ),
  'created' => array(
    'sortable' => 1,
    'default_sort_order' => 'desc',
    'align' => '',
    'separator' => '',
  ),
);
$handler->display->display_options['style_options']['override'] = 1;
$handler->display->display_options['style_options']['sticky'] = 0;
$handler->display->display_options['style_options']['empty_table'] = 0;
/* No results behavior: Global: Text area */
$handler->display->display_options['empty']['area']['id'] = 'area';
$handler->display->display_options['empty']['area']['table'] = 'views';
$handler->display->display_options['empty']['area']['field'] = 'area';
$handler->display->display_options['empty']['area']['empty'] = FALSE;
$handler->display->display_options['empty']['area']['content'] = 'There are no contacts in the CRM.  Add one by clicking above.';
$handler->display->display_options['empty']['area']['format'] = 'filtered_html';
$handler->display->display_options['empty']['area']['tokenize'] = 0;
/* Field: CRM Contact: Contact ID */
$handler->display->display_options['fields']['contact_id']['id'] = 'contact_id';
$handler->display->display_options['fields']['contact_id']['table'] = 'crm_core_contact';
$handler->display->display_options['fields']['contact_id']['field'] = 'contact_id';
$handler->display->display_options['fields']['contact_id']['exclude'] = TRUE;
$handler->display->display_options['fields']['contact_id']['alter']['alter_text'] = 0;
$handler->display->display_options['fields']['contact_id']['alter']['make_link'] = 0;
$handler->display->display_options['fields']['contact_id']['alter']['absolute'] = 0;
$handler->display->display_options['fields']['contact_id']['alter']['word_boundary'] = 1;
$handler->display->display_options['fields']['contact_id']['alter']['ellipsis'] = 1;
$handler->display->display_options['fields']['contact_id']['alter']['strip_tags'] = 0;
$handler->display->display_options['fields']['contact_id']['alter']['trim'] = 0;
$handler->display->display_options['fields']['contact_id']['alter']['html'] = 0;
$handler->display->display_options['fields']['contact_id']['hide_empty'] = 0;
$handler->display->display_options['fields']['contact_id']['empty_zero'] = 0;
$handler->display->display_options['fields']['contact_id']['link_to_contact'] = 0;
/* Field: Contact: Image */
$handler->display->display_options['fields']['contact_image']['id'] = 'contact_image';
$handler->display->display_options['fields']['contact_image']['table'] = 'field_data_contact_image';
$handler->display->display_options['fields']['contact_image']['field'] = 'contact_image';
$handler->display->display_options['fields']['contact_image']['alter']['alter_text'] = 0;
$handler->display->display_options['fields']['contact_image']['alter']['make_link'] = 0;
$handler->display->display_options['fields']['contact_image']['alter']['absolute'] = 0;
$handler->display->display_options['fields']['contact_image']['alter']['external'] = 0;
$handler->display->display_options['fields']['contact_image']['alter']['replace_spaces'] = 0;
$handler->display->display_options['fields']['contact_image']['alter']['trim_whitespace'] = 0;
$handler->display->display_options['fields']['contact_image']['alter']['nl2br'] = 0;
$handler->display->display_options['fields']['contact_image']['alter']['word_boundary'] = 1;
$handler->display->display_options['fields']['contact_image']['alter']['ellipsis'] = 1;
$handler->display->display_options['fields']['contact_image']['alter']['strip_tags'] = 0;
$handler->display->display_options['fields']['contact_image']['alter']['trim'] = 0;
$handler->display->display_options['fields']['contact_image']['alter']['html'] = 0;
$handler->display->display_options['fields']['contact_image']['element_label_colon'] = 1;
$handler->display->display_options['fields']['contact_image']['element_default_classes'] = 1;
$handler->display->display_options['fields']['contact_image']['hide_empty'] = 0;
$handler->display->display_options['fields']['contact_image']['empty_zero'] = 0;
$handler->display->display_options['fields']['contact_image']['hide_alter_empty'] = 0;
$handler->display->display_options['fields']['contact_image']['click_sort_column'] = 'fid';
$handler->display->display_options['fields']['contact_image']['settings'] = array(
  'image_style' => 'thumbnail',
  'image_link' => 'content',
);
$handler->display->display_options['fields']['contact_image']['field_api_classes'] = 0;
/* Field: Contact: Organization name */
$handler->display->display_options['fields']['entity_id']['id'] = 'entity_id';
$handler->display->display_options['fields']['entity_id']['table'] = 'field_data_contact_name';
$handler->display->display_options['fields']['entity_id']['field'] = 'contact_name';
$handler->display->display_options['fields']['entity_id']['alter']['alter_text'] = 0;
$handler->display->display_options['fields']['entity_id']['alter']['make_link'] = 1;
$handler->display->display_options['fields']['entity_id']['alter']['path'] = 'crm/contact/[contact_id]';
$handler->display->display_options['fields']['entity_id']['alter']['absolute'] = 0;
$handler->display->display_options['fields']['entity_id']['alter']['word_boundary'] = 1;
$handler->display->display_options['fields']['entity_id']['alter']['ellipsis'] = 1;
$handler->display->display_options['fields']['entity_id']['alter']['strip_tags'] = 0;
$handler->display->display_options['fields']['entity_id']['alter']['trim'] = 0;
$handler->display->display_options['fields']['entity_id']['alter']['html'] = 0;
$handler->display->display_options['fields']['entity_id']['hide_empty'] = 0;
$handler->display->display_options['fields']['entity_id']['empty_zero'] = 0;
/* Field: CRM Contact: Contact Type */
$handler->display->display_options['fields']['type']['id'] = 'type';
$handler->display->display_options['fields']['type']['table'] = 'crm_core_contact';
$handler->display->display_options['fields']['type']['field'] = 'type';
$handler->display->display_options['fields']['type']['alter']['alter_text'] = 0;
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
$handler->display->display_options['fields']['type']['element_label_colon'] = 1;
$handler->display->display_options['fields']['type']['element_default_classes'] = 1;
$handler->display->display_options['fields']['type']['hide_empty'] = 0;
$handler->display->display_options['fields']['type']['empty_zero'] = 0;
$handler->display->display_options['fields']['type']['hide_alter_empty'] = 0;
$handler->display->display_options['fields']['type']['link_to_contact'] = 0;
/* Field: CRM Contact: Created date */
$handler->display->display_options['fields']['created']['id'] = 'created';
$handler->display->display_options['fields']['created']['table'] = 'crm_core_contact';
$handler->display->display_options['fields']['created']['field'] = 'created';
$handler->display->display_options['fields']['created']['label'] = 'Added';
$handler->display->display_options['fields']['created']['alter']['alter_text'] = 0;
$handler->display->display_options['fields']['created']['alter']['make_link'] = 0;
$handler->display->display_options['fields']['created']['alter']['absolute'] = 0;
$handler->display->display_options['fields']['created']['alter']['word_boundary'] = 1;
$handler->display->display_options['fields']['created']['alter']['ellipsis'] = 1;
$handler->display->display_options['fields']['created']['alter']['strip_tags'] = 0;
$handler->display->display_options['fields']['created']['alter']['trim'] = 0;
$handler->display->display_options['fields']['created']['alter']['html'] = 0;
$handler->display->display_options['fields']['created']['hide_empty'] = 0;
$handler->display->display_options['fields']['created']['empty_zero'] = 0;
/* Sort criterion: CRM Contact: Updated date */
$handler->display->display_options['sorts']['changed']['id'] = 'changed';
$handler->display->display_options['sorts']['changed']['table'] = 'crm_core_contact';
$handler->display->display_options['sorts']['changed']['field'] = 'changed';
$handler->display->display_options['sorts']['changed']['order'] = 'DESC';
/* Filter criterion: CRM Contact Types: Disabled contact type */
$handler->display->display_options['filters']['disabled']['id'] = 'disabled';
$handler->display->display_options['filters']['disabled']['table'] = 'crm_core_contact_type';
$handler->display->display_options['filters']['disabled']['field'] = 'disabled';
$handler->display->display_options['filters']['disabled']['value']['value'] = '0';
/* Filter criterion: CRM Contact: Contact Type */
$handler->display->display_options['filters']['type']['id'] = 'type';
$handler->display->display_options['filters']['type']['table'] = 'crm_core_contact';
$handler->display->display_options['filters']['type']['field'] = 'type';
$handler->display->display_options['filters']['type']['exposed'] = TRUE;
$handler->display->display_options['filters']['type']['expose']['operator_id'] = 'type_op';
$handler->display->display_options['filters']['type']['expose']['label'] = 'Contact Type';
$handler->display->display_options['filters']['type']['expose']['operator'] = 'type_op';
$handler->display->display_options['filters']['type']['expose']['identifier'] = 'type';
$handler->display->display_options['filters']['type']['expose']['reduce'] = 0;
/* Filter criterion: Search: Search Terms */
$handler->display->display_options['filters']['keys']['id'] = 'keys';
$handler->display->display_options['filters']['keys']['table'] = 'search_index';
$handler->display->display_options['filters']['keys']['field'] = 'keys';
$handler->display->display_options['filters']['keys']['exposed'] = TRUE;
$handler->display->display_options['filters']['keys']['expose']['operator_id'] = 'keys_op';
$handler->display->display_options['filters']['keys']['expose']['label'] = 'Keywords';
$handler->display->display_options['filters']['keys']['expose']['operator'] = 'keys_op';
$handler->display->display_options['filters']['keys']['expose']['identifier'] = 'keys';
$handler->display->display_options['filters']['keys']['expose']['multiple'] = FALSE;

/* Display: Page */
$handler = $view->new_display('page', 'Page', 'page_1');
$handler->display->display_options['path'] = 'crm/contact';
$handler->display->display_options['menu']['type'] = 'tab';
$handler->display->display_options['menu']['title'] = 'Contacts';
$handler->display->display_options['menu']['weight'] = '0';

  $views['crm_core_contact_revision'] = $view; 
  return $views;
}

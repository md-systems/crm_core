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
  $view->api_version = '3.0-alpha1';
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
  
  /* Display: Page */
  $handler = $view->new_display('page', 'Page', 'page_1');
  $handler->display->display_options['path'] = 'crm/contact';
  $handler->display->display_options['menu']['type'] = 'tab';
  $handler->display->display_options['menu']['title'] = 'Contacts';
  $handler->display->display_options['menu']['weight'] = '0';
 
  $views['crm_core_contacts'] = $view;

  $view = new view;
  $view->name = 'crm_core_contacts_revisions';
  $view->description = '';
  $view->tag = 'default';
  $view->base_table = 'crm_core_contact_revision';
  $view->human_name = 'CRM Contact Revisions';
  $view->core = 7;
  $view->api_version = '3.0-alpha1';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Revisions';
  $handler->display->display_options['access']['type'] = 'none';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['query']['options']['query_comment'] = FALSE;
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '10';
  $handler->display->display_options['style_plugin'] = 'table';
  $handler->display->display_options['style_options']['columns'] = array(
    'vid' => 'vid',
    'contact_id' => 'contact_id',
    'changed' => 'changed',
    'contact_name-revision_id' => 'contact_name-revision_id',
  );
  $handler->display->display_options['style_options']['default'] = '-1';
  $handler->display->display_options['style_options']['info'] = array(
    'vid' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
    ),
    'contact_id' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
    ),
    'changed' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
    ),
    'contact_name-revision_id' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
    ),
  );
  $handler->display->display_options['style_options']['override'] = 1;
  $handler->display->display_options['style_options']['sticky'] = 0;
  $handler->display->display_options['style_options']['empty_table'] = 0;
  /* Relationship: CRM Contacts Revisions: Creator */
  $handler->display->display_options['relationships']['uid']['id'] = 'uid';
  $handler->display->display_options['relationships']['uid']['table'] = 'crm_core_contact_revision';
  $handler->display->display_options['relationships']['uid']['field'] = 'uid';
  $handler->display->display_options['relationships']['uid']['required'] = 0;
  /* Field: CRM Contacts Revisions: Contact revision ID */
  $handler->display->display_options['fields']['vid']['id'] = 'vid';
  $handler->display->display_options['fields']['vid']['table'] = 'crm_core_contact_revision';
  $handler->display->display_options['fields']['vid']['field'] = 'vid';
  $handler->display->display_options['fields']['vid']['label'] = 'Revision ID';
  $handler->display->display_options['fields']['vid']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['vid']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['vid']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['vid']['alter']['external'] = 0;
  $handler->display->display_options['fields']['vid']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['vid']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['vid']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['vid']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['vid']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['vid']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['vid']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['vid']['alter']['html'] = 0;
  $handler->display->display_options['fields']['vid']['element_label_colon'] = 1;
  $handler->display->display_options['fields']['vid']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['vid']['hide_empty'] = 0;
  $handler->display->display_options['fields']['vid']['empty_zero'] = 0;
  $handler->display->display_options['fields']['vid']['hide_alter_empty'] = 0;
  /* Field: CRM Contacts Revisions: Updated date */
  $handler->display->display_options['fields']['changed']['id'] = 'changed';
  $handler->display->display_options['fields']['changed']['table'] = 'crm_core_contact_revision';
  $handler->display->display_options['fields']['changed']['field'] = 'changed';
  $handler->display->display_options['fields']['changed']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['changed']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['changed']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['changed']['alter']['external'] = 0;
  $handler->display->display_options['fields']['changed']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['changed']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['changed']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['changed']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['changed']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['changed']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['changed']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['changed']['alter']['html'] = 0;
  $handler->display->display_options['fields']['changed']['element_label_colon'] = 1;
  $handler->display->display_options['fields']['changed']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['changed']['hide_empty'] = 0;
  $handler->display->display_options['fields']['changed']['empty_zero'] = 0;
  $handler->display->display_options['fields']['changed']['hide_alter_empty'] = 0;
  $handler->display->display_options['fields']['changed']['date_format'] = 'long';
  /* Field: User: Name */
  $handler->display->display_options['fields']['name']['id'] = 'name';
  $handler->display->display_options['fields']['name']['table'] = 'users';
  $handler->display->display_options['fields']['name']['field'] = 'name';
  $handler->display->display_options['fields']['name']['relationship'] = 'uid';
  $handler->display->display_options['fields']['name']['label'] = 'Updated By';
  $handler->display->display_options['fields']['name']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['name']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['name']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['name']['alter']['external'] = 0;
  $handler->display->display_options['fields']['name']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['name']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['name']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['name']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['name']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['name']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['name']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['name']['alter']['html'] = 0;
  $handler->display->display_options['fields']['name']['element_label_colon'] = 1;
  $handler->display->display_options['fields']['name']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['name']['hide_empty'] = 0;
  $handler->display->display_options['fields']['name']['empty_zero'] = 0;
  $handler->display->display_options['fields']['name']['hide_alter_empty'] = 0;
  $handler->display->display_options['fields']['name']['link_to_user'] = 1;
  $handler->display->display_options['fields']['name']['overwrite_anonymous'] = 0;
  /* Field: CRM Contacts Revisions: Contact revision ID */
  $handler->display->display_options['fields']['vid_1']['id'] = 'vid_1';
  $handler->display->display_options['fields']['vid_1']['table'] = 'crm_core_contact_revision';
  $handler->display->display_options['fields']['vid_1']['field'] = 'vid';
  $handler->display->display_options['fields']['vid_1']['label'] = 'Revert';
  $handler->display->display_options['fields']['vid_1']['alter']['alter_text'] = 1;
  $handler->display->display_options['fields']['vid_1']['alter']['text'] = 'Revert';
  $handler->display->display_options['fields']['vid_1']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['vid_1']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['vid_1']['alter']['external'] = 0;
  $handler->display->display_options['fields']['vid_1']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['vid_1']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['vid_1']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['vid_1']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['vid_1']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['vid_1']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['vid_1']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['vid_1']['alter']['html'] = 0;
  $handler->display->display_options['fields']['vid_1']['element_label_colon'] = 1;
  $handler->display->display_options['fields']['vid_1']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['vid_1']['hide_empty'] = 0;
  $handler->display->display_options['fields']['vid_1']['empty_zero'] = 0;
  $handler->display->display_options['fields']['vid_1']['hide_alter_empty'] = 0;
  $handler->display->display_options['fields']['vid_1']['link_to_revision'] = 1;
  /* Sort criterion: CRM Contacts Revisions: Updated date */
  $handler->display->display_options['sorts']['changed']['id'] = 'changed';
  $handler->display->display_options['sorts']['changed']['table'] = 'crm_core_contact_revision';
  $handler->display->display_options['sorts']['changed']['field'] = 'changed';
  $handler->display->display_options['sorts']['changed']['order'] = 'DESC';
  /* Contextual filter: CRM Contacts Revisions: Contact ID */
  $handler->display->display_options['arguments']['contact_id']['id'] = 'contact_id';
  $handler->display->display_options['arguments']['contact_id']['table'] = 'crm_core_contact_revision';
  $handler->display->display_options['arguments']['contact_id']['field'] = 'contact_id';
  $handler->display->display_options['arguments']['contact_id']['default_action'] = 'empty';
  $handler->display->display_options['arguments']['contact_id']['title'] = '%1 - Revisions';
  $handler->display->display_options['arguments']['contact_id']['default_argument_type'] = 'fixed';
  $handler->display->display_options['arguments']['contact_id']['default_argument_options']['argument'] = 'There are no revisions for this contact.';
  $handler->display->display_options['arguments']['contact_id']['default_argument_skip_url'] = 0;
  $handler->display->display_options['arguments']['contact_id']['summary']['number_of_records'] = '0';
  $handler->display->display_options['arguments']['contact_id']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['contact_id']['summary_options']['items_per_page'] = '25';
  $handler->display->display_options['arguments']['contact_id']['break_phrase'] = 0;
  $handler->display->display_options['arguments']['contact_id']['not'] = 0;

  /* Display: CRM Contacts Revisions */
  $handler = $view->new_display('page', 'CRM Contacts Revisions', 'page');
  $handler->display->display_options['path'] = 'crm/contact/%/revisions';
  $handler->display->display_options['menu']['type'] = 'tab';
  $handler->display->display_options['menu']['title'] = 'Revisions';
  $handler->display->display_options['menu']['description'] = 'A list of revisions or this contact.';
  $handler->display->display_options['menu']['weight'] = '0';

  $views['crm_core_contact_revision'] = $view; 
  return $views;
}

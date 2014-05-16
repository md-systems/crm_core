<?php

/**
 * @file
 * Creates views used by default for displaying activities with contact records.
 */

/**
 * Implements hook_views_data_alter().
 */
function crm_core_activity_views_data_alter(&$data) {
  $data['crm_core_activity']['table']['group'] = t('CRM Core Activity');
  $data['crm_core_activity_type']['type']['field']['handler'] = 'views_handler_field_activity_type';
  $data['crm_core_activity']['type']['filter']['handler'] = 'views_handler_filter_activity_type';

  // View, Edit and Delete links of activity.
  $data['crm_core_activity']['view_crm_activity'] = array(
    'field' => array(
      'title' => t('Link'),
      'help' => t('Provide a simple link to the activity.'),
      'handler' => 'crm_core_activity_handler_field_link',
    ),
  );

  $data['crm_core_activity']['edit_crm_activity'] = array(
    'field' => array(
      'title' => t('Edit link'),
      'help' => t('Provide a simple link to edit the activity.'),
      'handler' => 'crm_core_activity_handler_field_link_edit',
    ),
  );
  $data['crm_core_activity']['delete_crm_activity'] = array(
    'field' => array(
      'title' => t('Delete link'),
      'help' => t('Provide a simple link to delete the activity.'),
      'handler' => 'crm_core_activity_handler_field_link_delete',
    ),
  );

  // Expose the creator uid.
  $data['crm_core_activity']['uid'] = array(
    'title' => t('Author'),
    'help' => t('Relate CRM Core Activities to the user who created it.'),
    'field' => array(
      'handler' => 'views_handler_field_numeric',
      'click sortable' => TRUE,
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_numeric',
      'numeric' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'relationship' => array(
      'label' => 'crm_core_activity author',
      'title' => t('Author'),
      'base' => 'users',
      'base field' => 'uid',
    ),
  );

  // Integration with Drupal search index.
  $data['search_index']['table']['join']['crm_core_activity'] = array(
    'left_field' => 'activity_id ',
    'field' => 'sid',
  );

  $data['search_total']['table']['join']['crm_core_activity'] = array(
    'left_table' => 'search_index',
    'left_field' => 'word',
    'field' => 'word',
  );

  $data['search_dataset']['table']['join']['crm_core_activity'] = array(
    'left_table' => 'search_index',
    'left_field' => 'sid',
    'field' => 'sid',
    'extra' => 'search_index.type = search_dataset.type',
    'type' => 'INNER',
  );

  $data['crm_core_activity_type']['table']['join'] = array(
    'crm_core_activity' => array(
      'left_field' => 'type',
      'field' => 'type',
    ),
  );

  $data['crm_core_activity_type']['activity_string'] = array(
    'field' => array(
      'title' => t('Activity string'),
      'help' => t('Text describing the relationship between the contact and this activity.'),
      'handler' => 'views_handler_field',
    ),
  );
}

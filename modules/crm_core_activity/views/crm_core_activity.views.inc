<?php

/**
 * Implements hook_views_data_alter().
 */
function crm_core_activity_views_data_alter(&$data) {
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
}
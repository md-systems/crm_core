<?php

/**
 * Implements hook_views_data_alter().
 */
function crm_activity_views_data_alter(&$data) {
  $data['crm_activity_type']['type']['field']['handler'] = 'views_handler_field_activity_type';

  // View, Edit and Delete links of activity.
  $data['crm_activity']['view_crm_activity'] = array(
    'field' => array(
      'title' => t('Link'),
      'help' => t('Provide a simple link to the activity.'),
      'handler' => 'crm_activity_handler_field_link',
    ),
  );

  $data['crm_activity']['edit_crm_activity'] = array(
    'field' => array(
      'title' => t('Edit link'),
      'help' => t('Provide a simple link to edit the activity.'),
      'handler' => 'crm_activity_handler_field_link_edit',
    ),
  );
  $data['crm_activity']['delete_crm_activity'] = array(
    'field' => array(
      'title' => t('Delete link'),
      'help' => t('Provide a simple link to delete the activity.'),
      'handler' => 'crm_activity_handler_field_link_delete',
    ),
  );
}
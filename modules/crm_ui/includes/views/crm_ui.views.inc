<?php

/**
 * Implements hook_views_data_alter().
 */
function crm_ui_views_data_alter(&$data) {
  $data['crm_activity_type']['type']['field']['handler'] = 'views_handler_field_activity_type';
}

<?php

/**
 * Implements hook_views_data_alter().
 */
function crm_core_case_views_data_alter(&$data) {
  $data['crm_core_case']['status']['filter']['handler'] = 'crm_core_case_handler_filter_crm_core_case_status';
}
  
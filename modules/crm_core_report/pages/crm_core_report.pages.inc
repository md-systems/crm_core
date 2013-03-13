<?php 

/**
 * Provides a listing of all reports registered for CRM Core
 */
function crm_core_report_list(){
  drupal_set_title(t('Reports'));
  $reports = hook_crm_core_report_register();
  return empty($reports)
    ? t('There are no reports to display at this time.') : theme('crm_core_report_index', array('reports' => $reports));
}



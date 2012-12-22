<?php 

/**
 * Provides a listing of all reports registered for CRM Core
 */
function crm_core_report_list(){
  $reports = hook_crm_core_report_register();
  return theme('crm_core_report_index', array('reports' => $reports));
}



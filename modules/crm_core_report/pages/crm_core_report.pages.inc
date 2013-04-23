<?php

/**
 * @file
 * Reports listing page.
 */

/**
 * Provides a listing of all reports registered for CRM Core.
 */
function crm_core_report_list() {
  drupal_set_title(t('Reports'));
  drupal_set_breadcrumb(array(
    l(t('Home'), '<front>'),
    l(t('CRM Core'), 'crm-core'),
    l(t('Reports'), 'crm-core/reports'),
  ));
  $reports = hook_crm_core_report_register();

  return empty($reports)
    ? t('There are no reports to display at this time.') : theme('crm_core_report_index', array('reports' => $reports));
}

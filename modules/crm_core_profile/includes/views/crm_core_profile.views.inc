<?php

/**
 * @file
 * Export CRM Core Profile to views.
 */


/**
 * Implements hook_views_data().
 */
function crm_core_profile_views_data() {
  $data = array();

  $data['crm_core_profile']['table']['group'] = t('CRM Profile');
  $data['crm_core_profile']['table']['base'] = array(
    'field' => 'name',
    'title' => t('CRM Core Profile'),
    'help' => t('Machine name of the profile form'),
  );

  $data['crm_core_profile']['name'] = array(
    'title' => t('Profile form name'),
    'help' => t('The unqiue internal identifier for a profile form.'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );

  $data['crm_core_profile']['label'] = array(
    'title' => t('Profile form label'),
    'help' => t('The human-readable name of the type of a profile form.'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );
  
  $data['crm_core_profile']['path'] = array(
    'title' => t('Path'),
    'help' => t('The site path to the form'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );

  $data['crm_core_profile']['created'] = array(
    'title' => t('Created date'),
    'help' => t('The date the profile form was first added.'),
    'field' => array(
      'handler' => 'views_handler_field_date',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort_date',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_date',
    ),
  );
  
  return $data;
}
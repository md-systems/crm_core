<?php

/**
 * @file
 * Export CRM data to views.
 */

/**
 * Implements hook_views_data().
 */
function crm_activity_views_data() {
  $data = array();

  $data['crm_activity']['table']['group'] = t('CRM Activity');

  $data['crm_activity']['table']['base'] = array(
    'field' => 'contact_id',
    'title' => t('CRM Activity'),
    'help' => t('Activity data from the CRM.'),
  );

  $data['crm_activity']['activity_id'] = array(
    'title' => t('Activity ID'),
    'help' => t('The unqiue internal identifier for an activity.'),
    'field' => array(
      'handler' => 'crm_activity_handler_field_activity',
      'click sortable' => TRUE,
    ),
    'fitler' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_numeric',
    ),
  );

  $data['crm_activity']['title'] = array(
    'title' => t('Activity Title'),
    'help' => t('The title for an activity.'),
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

  $data['crm_activity']['type'] = array(
    'title' => t('Activity Type'),
    'help' => t('The human-readable name of the type of an activity.'),
    'field' => array(
      'handler' => 'crm_activity_handler_field_activity_type',
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

  // Expose the creator uid.
  $data['crm_activity']['uid'] = array(
    'title' => t('Creator'),
    'help' => t('Relate an activity to the user who performed it.'),
    'relationship' => array(
      'handler' => 'views_handler_relationship',
      'base' => 'users',
      'field' => 'uid',
      'label' => t('Activity performer'),
    ),
  );

  // Expose the created and changed timestamps.
  $data['crm_activity']['created'] = array(
    'title' => t('Created date'),
    'help' => t('The date the activity was first added.'),
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

  $data['crm_activity']['changed'] = array(
    'title' => t('Updated date'),
    'help' => t('The date the activity was last updated.'),
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

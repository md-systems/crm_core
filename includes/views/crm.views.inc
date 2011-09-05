<?php

/**
 * @file
 * Export CRM data to views.
 */

/**
 * Implements hook_views_data().
 */
function crm_views_data() {
  $data = array();

  $data['crm_contact']['table']['group'] = t('CRM Contact');

  $data['crm_contact']['table']['base'] = array(
    'field' => 'contact_id',
    'title' => t('CRM Contact'),
    'help' => t('Contacts from the CRM.'),
  );

  $data['crm_contact']['contact_id'] = array(
    'title' => t('Contact ID'),
    'help' => t('The unqiue internal identifier for a contact.'),
    'field' => array(
      'handler' => 'crm_contact_handler_field_contact',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_numeric',
    ),
  );

  $data['crm_contact']['type'] = array(
    'title' => t('Contact Type'),
    'help' => t('The human-readable name of the type of a contact.'),
    'field' => array(
      'handler' => 'crm_contact_handler_field_contact_type',
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
  $data['crm_contact']['uid'] = array(
    'title' => t('Creator'),
    'help' => t('Relate a contact to the user who created it.'),
    'relationship' => array(
      'handler' => 'views_handler_relationship',
      'base' => 'users',
      'field' => 'uid',
      'label' => t('Contact creator'),
    ),
  );

  // Expose the created and changed timestamps.
  $data['crm_contact']['created'] = array(
    'title' => t('Created date'),
    'help' => t('The date the contact was first added.'),
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

  $data['crm_contact']['changed'] = array(
    'title' => t('Updated date'),
    'help' => t('The date the contact was last updated.'),
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

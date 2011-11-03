<?php

/**
 * @file
 * Export CRM Core Contact to views.
 */


/**
 * Implements hook_views_data().
 */
function crm_core_contact_views_data() {
  $data = array();

  $data['crm_core_contact']['table']['group'] = t('CRM Contact');

  $data['crm_core_contact']['table']['base'] = array(
    'field' => 'contact_id',
    'title' => t('CRM Contact'),
    'help' => t('Contacts from the CRM.'),
  );

  $data['crm_core_contact']['contact_id'] = array(
    'title' => t('Contact ID'),
    'help' => t('The unqiue internal identifier for a contact.'),
    'field' => array(
      'handler' => 'crm_core_contact_handler_field_contact',
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

  $data['crm_core_contact']['type'] = array(
    'title' => t('Contact Type'),
    'help' => t('The human-readable name of the type of a contact.'),
    'field' => array(
      'handler' => 'crm_core_contact_handler_field_contact_type',
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
  $data['crm_core_contact']['uid'] = array(
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
  $data['crm_core_contact']['created'] = array(
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

  $data['crm_core_contact']['changed'] = array(
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
  
  $data['crm_core_contact_type']['table']['group'] = t('CRM Contact Types');

  $data['crm_core_contact_type']['table']['base'] = array(
    'field' => 'type',
    'title' => t('CRM Contact Type'),
    'help' => t('CRM Contact Types'),
  );

  // contact type table join for filtering purposes
  $data['crm_core_contact_type']['table']['join']['crm_core_contact'] = array(
      'left_field' => 'type',
      'field' => 'type',
  );
  
  $data['crm_core_contact_type']['disabled'] = array(
  'title' => t('Disabled contact type'),
    'help' => t('If the contact type is disabled, mainly for filtering purposes'),
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
  

  $data['crm_core_contact_revision']['table']['group'] = t('CRM Contacts Revisions');

  $data['crm_core_contact_revision']['table']['base'] = array(
    'field' => 'vid',
    'title' => t('CRM Contacts Revisions'),
    'help' => t('Contacts revisions from the CRM.'),
  );

  $data['crm_core_contact_revision']['vid'] = array(
    'title' => t('Contact revision ID'),
    'help' => t('The unqiue internal identifier for a contact revision.'),
    'field' => array(
      'handler' => 'crm_core_contact_handler_field_contact_revision',
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
  
   $data['crm_core_contact_revision']['revert_link'] = array(
   'field' => array(
     'title' => t('Revision Link'),
     'help' => t('Provide a simple link to revert the contact'),
     'handler' => 'crm_core_contact_handler_field_contact_revert',
   ),
 );
  
  $data['crm_core_contact_revision']['contact_id'] = array(
    'title' => t('Contact ID'),
    'help' => t('The unqiue internal identifier for a contact revision.'),
    'field' => array(
      'handler' => 'crm_core_contact_handler_field_contact',
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

  $data['crm_core_contact_revision']['table']['join'] = array(
    // Index this array by the table name to which this table refers.
    // 'left_field' is the primary key in the referenced table.
    // 'field' is the foreign key in this table.
    'users' => array(
      'left_field' => 'uid',
      'field' => 'uid',
    ),
  );  
  
  // Expose the creator uid.
  $data['crm_core_contact_revision']['uid'] = array(
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
  $data['crm_core_contact_revision']['created'] = array(
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

  $data['crm_core_contact_revision']['changed'] = array(
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

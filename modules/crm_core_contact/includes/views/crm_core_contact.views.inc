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

  $data['crm_core_contact']['table']['group'] = t('CRM Core Contact');

  $data['crm_core_contact']['table']['base'] = array(
    'field' => 'contact_id',
    'title' => t('CRM Core Contact'),
    'help' => t('Contacts from the CRM Core.'),
  );

  $data['crm_core_contact']['contact_id'] = array(
    'title' => t('Contact ID'),
    'help' => t('The unique internal identifier for a contact.'),
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
    'help' => t('The human-readable(or machine) name of the type of contact.'),
    'field' => array(
      'handler' => 'crm_core_contact_handler_field_contact_type',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_in_operator',
      'options callback' => 'crm_core_contact_type_get_name',
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
    'title' => t('Author'),
    'help' => t('Relate CRM Core Contacts to the user who created it.'),
    'field' => array(
      'handler' => 'views_handler_field_numeric',
      'click sortable' => TRUE,
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_numeric',
      'numeric' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'relationship' => array(
      'label' => 'crm_core_contact author',
      'title' => t('Author'),
      'base' => 'users',
      'base field' => 'uid',
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

  $data['crm_core_contact']['table']['entity type'] = 'crm_core_contact';

  $data['crm_core_contact_type']['table']['group'] = t('CRM Core Contact Types');

  $data['crm_core_contact_type']['table']['base'] = array(
    'field' => 'type',
    'title' => t('CRM Core Contact Type'),
    'help' => t('CRM Core Contact Types'),
  );

  // Contact type table join for filtering purposes.
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

  $data['crm_core_contact_revision']['table']['group'] = t('CRM Core Contact revision');

  $data['crm_core_contact_revision']['table']['base'] = array(
    'field' => 'vid',
    'title' => t('CRM Core Contact revision'),
    'help' => t('Contact revisions from CRM Core.'),
  );

  $data['crm_core_contact_revision']['vid'] = array(
    'title' => t('Contact revision ID'),
    'help' => t('The unique internal identifier for a contact revision.'),
    'field' => array(
      'handler' => 'crm_core_contact_handler_field_contact_revision',
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

  $data['crm_core_contact_revision']['revert_link'] = array(
    'field' => array(
      'title' => t('Revision Link'),
      'help' => t('Provide a simple link to revert the contact'),
      'handler' => 'crm_core_contact_handler_field_contact_revert',
    ),
  );

  $data['crm_core_contact_revision']['contact_id'] = array(
    'title' => t('Contact ID'),
    'help' => t('The unique internal identifier for a contact.'),
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

  $data['crm_core_contact_revision']['table']['join'] = array(
    // Index this array by the table name to which this table refers.
    // 'left_field' is the primary key in the referenced table.
    // 'field' is the foreign key in this table.
    'users' => array(
      'left_field' => 'uid',
      'field' => 'uid',
    ),
  );

  $data['crm_core_contact_revision']['table']['join'] = array(
    'crm_core_contact' => array(
      'left_field' => 'contact_id',
      'field' => 'contact_id',
      'extra' => 'crm_core_contact.vid = crm_core_contact_revision.vid',
      // Can use AND or OR, not really needed here since it defaults to AND.
      'extra type' => 'AND',
    ),
  );

  // Expose the creator uid.
  $data['crm_core_contact_revision']['uid'] = array(
    'title' => t('User'),
    'help' => t('Relate CRM Core Contacts to the user who created it.'),
    'field' => array(
      'handler' => 'views_handler_field_numeric',
      'click sortable' => TRUE,
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_numeric',
      'numeric' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'relationship' => array(
      'label' => 'crm_core_contact user',
      'title' => t('User'),
      'base' => 'users',
      'base field' => 'uid',
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

/**
 * Implements hook_views_data_alter().
 */
function crm_core_contact_views_data_alter(&$data) {
  // Integration with Drupal search index.
  $data['search_index']['table']['join']['crm_core_contact'] = array(
    'left_field' => 'contact_id ',
    'field' => 'sid',
  );

  $data['search_total']['table']['join']['crm_core_contact'] = array(
    'left_table' => 'search_index',
    'left_field' => 'word',
    'field' => 'word',
  );

  $data['search_dataset']['table']['join']['crm_core_contact'] = array(
    'left_table' => 'search_index',
    'left_field' => 'sid',
    'field' => 'sid',
    'extra' => 'search_index.type = search_dataset.type',
    'type' => 'INNER',
  );
}

<?php

/**
 * @file
 * Resource definitions for services module.
 */

/**
 * Determine whether the current user can access a crm_core_contact resource.
 *
 * @param string $op
 *   Operation, access for which should be checked.
 * @param mixed $arg
 *   Contact ID or contact data array, in case $op is 'create'.
 *
 * @return bool
 *   Boolean.
 *
 * @see crm_core_contact_access()
 */
function _crm_core_contact_resource_access($op, $arg) {
  if (is_array($arg[0])) {
    $contact_data = array_shift($arg);

    return crm_core_contact_access($op, $contact_data['type']);
  }
  elseif (is_numeric($arg[0])) {
    $crm_core_contact = crm_core_contact_load($arg[0]);

    return crm_core_contact_access($op, $crm_core_contact);
  }
  elseif (!isset($arg)) {
    return (user_access('administer crm_core_contact entities') || user_access('view any crm_core_contact entity'));
  }

  return FALSE;
}

/**
 * Return an array of crm_core_contacts.
 *
 * @param int $page
 *   Page number of results to return (in pages of 20).
 * @param int $page_size
 *   Integer number of items to be returned.
 *
 * @return array
 *   An array of node objects.
 */
function _crm_core_contact_resource_index($page, $page_size) {
  $cids = db_select('crm_core_contact', 'c')
    ->orderBy('created', 'DESC')
    ->fields('c', array('contact_id'))
    ->range($page * $page_size, $page_size)
    ->execute()
    ->fetchCol();

  $contacts = crm_core_contact_load_multiple($cids);

  return services_resource_build_index_list($contacts, 'crm_core_contact', 'contact_id');
}

/**
 * Callback for the 'create' method.
 */
function _crm_core_contact_resource_create($contact_data) {
  try {
    $contact = entity_create('crm_core_contact', $contact_data);
    unset($contact->is_new);
    crm_core_contact_save($contact);

    return $contact;
  }
  catch (Exception $exception) {
    watchdog_exception('crm_core_contact', $exception);
    return services_error($exception, 406, $contact_data);
  }
}

/**
 * Callback for the 'update' method.
 */
function _crm_core_contact_resource_update($contact_id, $contact_data) {
  return _crm_core_contact_resource_create($contact_data);
}

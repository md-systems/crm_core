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
 * @param array $args
 *   Resource arguments passed through from the original request.
 *
 * @return bool
 *   Boolean.
 *
 * @see crm_core_contact_access()
 */
function _crm_core_contact_resource_access($op = 'view', $args = array()) {
  $crm_core_contact = crm_core_contact_load($args[0]);

  return crm_core_contact_access($op, $crm_core_contact);
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
  dpm(func_get_args(), 'Index params');
  $results = db_select('crm_core_contact', 'c')
    ->orderBy('created', 'DESC')
    ->fields('c')
    ->range($page * $page_size, $page_size)
    ->execute();

  return services_resource_build_index_list($results, 'crm_core_contact', 'contact_id');
}

<?php

/**
 * @file
 * Resource definitions for services module.
 */

/**
 * Determine whether the current user can access a crm_core_activity resource.
 *
 * @param string $op
 *   Operation, access for which should be checked.
 * @param mixed $arg
 *   Activity ID or activity data array, in case $op is 'create'.
 *
 * @return bool
 *   Boolean.
 *
 * @see crm_core_activity_access()
 */
function _crm_core_activity_resource_access($op, $arg = NULL) {
  if (is_array($arg[0])) {
    $activity_data = array_shift($arg);

    return crm_core_activity_access($op, $activity_data['type']);
  }
  elseif (is_numeric($arg[0])) {
    $crm_core_activity = crm_core_activity_load($arg[0]);

    return crm_core_activity_access($op, $crm_core_activity);
  }
  elseif (!isset($arg)) {
    return (user_access('administer crm_core_activity entities') || user_access('view any crm_core_activity entity'));
  }

  return FALSE;
}

/**
 * Return an array of crm_core_activities.
 *
 * @param int $page
 *   Page number of results to return (in pages of 20).
 * @param int $page_size
 *   Integer number of items to be returned.
 *
 * @return array
 *   An array of node objects.
 */
function _crm_core_activity_resource_index($page, $page_size) {
  $aids = db_select('crm_core_activity', 'c')
    ->orderBy('created', 'DESC')
    ->fields('c', array('activity_id'))
    ->range($page * $page_size, $page_size)
    ->execute()
    ->fetchCol();

  $activities = crm_core_activity_load_multiple($aids);

  return services_resource_build_index_list($activities, 'crm_core_activity', 'activity_id');
}

/**
 * Callback for the 'create' method.
 */
function _crm_core_activity_resource_create($activity_data) {
  try {
    $activity = entity_create('crm_core_activity', $activity_data);
    unset($activity->is_new);
    crm_core_activity_save($activity);

    return $activity;
  }
  catch (Exception $exception) {
    watchdog_exception('crm_core_activity', $exception);
    return services_error($exception, 406, $activity_data);
  }
}

/**
 * Callback for the 'update' method.
 */
function _crm_core_activity_resource_update($activity_id, $activity_data) {
  return _crm_core_activity_resource_create($activity_data);
}

/**
 * Callback for the 'delete' method.
 */
function _crm_core_activity_resource_delete($activity_id) {
  return crm_core_activity_delete_multiple(array($activity_id));
}

<?php


/**
 * @file
 * Provides support for the Views module.
 */

/**
 * Implements hook_views_plugins().
 */
function crm_core_user_sync_views_plugins() {
  return array(
    'argument default' => array(
      'crm_core_user_sync' => array(
        'title' => t('CRM Contact ID (from user id from URL)'),
        'handler' => 'views_plugin_argument_default_contact_id',
        'parent' => 'views_plugin_argument_default',
      ),
    ),
  );
}

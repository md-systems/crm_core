<?php

/**
 * @file
 * Integration with context module.
 */

/**
 * Implements hook_context_plugins().
 */
function crm_core_case_context_plugins() {
  $plugins = array();
  $plugins['crm_core_case_context_condition_case_type'] = array(
    'handler' => array(
      'path' => drupal_get_path('module', 'crm_core_case') . '/includes/context',
      'file' => 'crm_core_case_context_condition_case_type.inc',
      'class' => 'crm_core_case_context_condition_case_type',
      'parent' => 'context_condition',
    ),
  );
  return $plugins;
}

/**
 * Implements hook_context_registry().
 */
function crm_core_case_context_registry() {
  return array(
    'conditions' => array(
      'crm_core_case_context_condition_case_type' => array(
        'title' => t('CRM Case Type'),
        'plugin' => 'crm_core_case_context_condition_case_type',
      ),
    ),
  );
}

/**
 * Implements hook_context_page_condition().
 */
function crm_core_case_context_page_condition() {
  if ($plugin = context_get_plugin('condition', 'crm_core_case_context_condition_case_type')) {
    $contact = menu_get_object('crm_core_case', 2);
    $op = '';
    if ($contact) {
      switch (arg(3)) {
        case '':
          $op = 'view';
          break;
        case 'edit':
          $op = 'form';
          break;
      }
    }
    // Also execute context on case add page.
    if (strpos($_GET['q'], 'crm/case/add/') !== FALSE) {
      $contact = entity_create('crm_core_case', array('type' => arg(3)));
      $op = 'edit';
    }

    if ($op) {
      $plugin->execute($contact, $op);
    }
  }
}

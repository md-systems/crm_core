<?php
/**
 * @file
 * Rules integration for crm.
 */

/**
 * Implements hook_rules_event_info().
 */
function crm_rules_event_info() {
  $events = array();

  $events['crm_contact_update'] = array(
    'label' => t('After updating the contact'),
    'group' => t('CRM'),
    'variables' => crm_rules_event_variables(),
    'access callback' => '',
  );

  $events['crm_contact_insert'] = array(
    'label' => t('After creating new contact'),
    'group' => t('CRM'),
    'variables' => crm_rules_event_variables(),
    'access callback' => '',
  );

  $events['crm_contact_delete'] = array(
    'label' => t('After deleting the contact'),
    'group' => t('CRM'),
    'variables' => crm_rules_event_variables(),
    'access callback' => '',
  );

  return $events;
}

/**
 * Returns a variables array for CRM events.
 */
function crm_rules_event_variables() {
  $variables = array(
    'crm_contact' => array(
      'type' => 'crm_contact',
      'label' => t('CRM Contact'),
    ),
  );
  return $variables;
}

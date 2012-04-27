<?php

/**
 * @file
 * Class for determining if the user is in the right contact context.
 */

/**
 * Trigger context on node view only.
 */
define('CONTEXT_CRM_CONTACT_VIEW', 0);

/**
 * Trigger context on node view and node form.
 */
define('CONTEXT_CRM_CONTACT_NODE_FORM', 1);

/**
 * Trigger context on node form only.
 */
define('CONTEXT_CRM_CONTACT_NODE_FORM_ONLY', 2);

/**
 * Expose node views/node forms of specific node types as a context condition.
 */
class crm_core_contact_context_condition_contact_type extends context_condition {

  function condition_values() {
    $values = array();
    foreach (crm_core_contact_types() as $type) {
      $values[$type->type] = check_plain($type->name);
    }
    return $values;
  }

  function options_form($context) {
    $defaults = $this->fetch_from_context($context, 'options');
    return array(
      'contact_form' => array(
        '#title' => t('Set on contact form'),
        '#type' => 'select',
        '#options' => array(
           CONTEXT_CRM_CONTACT_VIEW => t('No'),
           CONTEXT_CRM_CONTACT_NODE_FORM => t('Yes'),
           CONTEXT_CRM_CONTACT_NODE_FORM_ONLY => t('Only on contact form')
        ),
        '#description' => t('Set this context on contact forms'),
        '#default_value' => isset($defaults['contact_form']) ? $defaults['contact_form'] : TRUE,
      ),
    );
  }

  function execute($contact, $op) {
    foreach ($this->get_contexts($contact->type) as $context) {

      $options = $this->fetch_from_context($context, 'options');
      if ($op === 'form') {
        $options = $this->fetch_from_context($context, 'options');
        if (!empty($options['contact_form']) && in_array($options['contact_form'], array(CONTEXT_CRM_CONTACT_NODE_FORM, CONTEXT_CRM_CONTACT_NODE_FORM_ONLY))) {
          $this->condition_met($context, $contact->type);
        }
      }
      elseif (empty($options['contact_form']) || $options['contact_form'] != CONTEXT_CRM_CONTACT_NODE_FORM_ONLY) {
        $this->condition_met($context, $contact->type);
      }
    }
  }
}

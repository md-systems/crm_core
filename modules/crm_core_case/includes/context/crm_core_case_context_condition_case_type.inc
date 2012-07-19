<?php

/**
 * @file
 * Class for determining if the user is in the right case context.
 */

/**
 * Trigger context on case view only.
 */
define('CONTEXT_CRM_CASE_VIEW', 0);

/**
 * Trigger context on case view and node form.
 */
define('CONTEXT_CRM_CASE_NODE_FORM', 1);

/**
 * Trigger context on case form only.
 */
define('CONTEXT_CRM_CASE_NODE_FORM_ONLY', 2);

/**
 * Expose case views/case forms of specific case types as a context condition.
 */
class crm_core_case_context_condition_case_type extends context_condition {

  function condition_values() {
    $values = array();
    foreach (crm_core_case_types() as $type) {
      $values[$type->type] = check_plain(entity_label('crm_core_case_type', $type));
    }
    return $values;
  }

  function options_form($context) {
    $defaults = $this->fetch_from_context($context, 'options');
    return array(
      'case_form' => array(
        '#title' => t('Set on case form'),
        '#type' => 'select',
        '#options' => array(
           CONTEXT_CRM_CASE_VIEW => t('No'),
           CONTEXT_CRM_CASE_NODE_FORM => t('Yes'),
           CONTEXT_CRM_CASE_NODE_FORM_ONLY => t('Only on case form')
        ),
        '#description' => t('Set this context on case forms'),
        '#default_value' => isset($defaults['case_form']) ? $defaults['case_form'] : TRUE,
      ),
    );
  }

  function execute($case, $op) {
    foreach ($this->get_contexts($case->type) as $context) {

      $options = $this->fetch_from_context($context, 'options');
      if ($op === 'form') {
        $options = $this->fetch_from_context($context, 'options');
        if (!empty($options['case_form']) && in_array($options['case_form'], array(CONTEXT_CRM_CASE_NODE_FORM, CONTEXT_CRM_CASE_NODE_FORM_ONLY))) {
          $this->condition_met($context, $case->type);
        }
      }
      elseif (empty($options['case_form']) || $options['case_form'] != CONTEXT_CRM_CASE_NODE_FORM_ONLY) {
        $this->condition_met($context, $case->type);
      }
    }
  }
}

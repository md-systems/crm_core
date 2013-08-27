<?php

/**
 * Implements a filter handler for status field of crm_core_case entity.
 */
class crm_core_case_handler_filter_crm_core_case_status extends views_handler_filter_in_operator {
  function get_value_options() {
    if (!isset($this->value_options)) {
      $this->value_title = t('CRM Case Status');
      $options = crm_core_case_status();
      $this->value_options = $options;
    }
  }
}

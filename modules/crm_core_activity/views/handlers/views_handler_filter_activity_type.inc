<?php

/**
 * @file
 * Views support. Allows users to filter by activity type
 */

class views_handler_filter_activity_type extends views_handler_filter_in_operator {
  function get_value_options() {
    if (!isset($this->value_options)) {
      $this->value_title = t('Activity types');
      $types = crm_core_activity_types();
      foreach ($types as $type => $info) {
        $options[$type] = $info->label;
      }
      asort($options);
      $this->value_options = $options;
    }
  }
}

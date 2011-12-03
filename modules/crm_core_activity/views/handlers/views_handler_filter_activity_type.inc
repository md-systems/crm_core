<?php
/**
 * Filter by activity type
 */
class views_handler_filter_activity_type extends views_handler_filter_in_operator {
  function get_value_options() {
    if (!isset($this->value_options)) {
      $this->value_title = t('Activity Types');
      $types = crm_core_activity_types();
      foreach ($types as $type => $info) {
        $options[$type] = $info->label;
      }
      asort($options);
// $options = array('this' => t('that'));
      $this->value_options = $options;
    }
  }
}

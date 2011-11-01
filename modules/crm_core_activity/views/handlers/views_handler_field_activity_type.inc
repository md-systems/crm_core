<?php

/**
 * Custom field handler for activity_type.
 *
 * Needed for building view crm_activity_types to show the list of available
 * activity types that we can create activity.
 *
 * Link to be build: 'crm/contact/' . [node nid] . '/activity/add/' . [activity type machine name].
 */
class views_handler_field_activity_type extends views_handler_field {
  function option_definition() {
    $options = parent::option_definition();
    $options['alter']['contains']['activity_create_link'] = array('default' => FALSE);
    return $options;
  }

  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);
    $form['alter']['activity_create_link'] = array(
      '#type' => 'checkbox',
      '#title' => t('New Activity create link'),
      '#default_value' => $this->options['alter']['activity_create_link'],
      '#description' => t('Show type as link to create Activity of this type.'),
      '#weight' => -200,
    );
  }

  function render_text($alter) {
    if ($this->options['alter']['activity_create_link']) {
      $value = $this->last_render;
      if (isset($this->view->args[0])) {
        return 'crm/contact/' . $this->view->args[0] . '/activity/add/' . $value;
      }
    }
    return parent::render_text($alter);
  }
}

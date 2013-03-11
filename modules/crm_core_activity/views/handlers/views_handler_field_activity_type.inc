<?php

/**
 * @file
 * Views support.
 */

/**
 * Provides a field handler for activity_type.
 *
 * Used in views that provide the ability to add an activity for a contact.
 *
 * The link is strucuted like so:
 * 'crm-core/contact/' . [node nid] . '/activity/add/' . [activity machine name]
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
        return 'crm-core/contact/' . $this->view->args[0] . '/activity/add/' . $value;
      }
    }
    return parent::render_text($alter);
  }
}

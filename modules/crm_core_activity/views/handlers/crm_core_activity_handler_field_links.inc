<?php

/**
 * @file
 * Provides views handlers for View, Edit, Delete links.
 */

class crm_core_activity_handler_field_link extends views_handler_field {
  function construct() {
    parent::construct();

    $this->additional_fields['activity_id'] = 'activity_id';
  }

  function option_definition() {
    $options = parent::option_definition();

    $options['text'] = array('default' => '', 'translatable' => TRUE);

    return $options;
  }

  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);

    $form['text'] = array(
      '#type' => 'textfield',
      '#title' => t('Text to display'),
      '#default_value' => $this->options['text'],
    );
  }

  function query() {
    $this->ensure_my_table();
    $this->add_additional_fields();
  }

  function render($values) {
    $text = !empty($this->options['text']) ? $this->options['text'] : t('view');
    $activity_id = $this->get_value($values, 'activity_id');

    return l($text, 'crm-core/activity/' . $activity_id);
  }
}

class crm_core_activity_handler_field_link_edit extends crm_core_activity_handler_field_link {
  function option_definition() {
    $options = parent::option_definition();

    $options['redirect'] = array('default' => FALSE);

    return $options;
  }

  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);

    $form['redirect'] = array(
      '#type' => 'checkbox',
      '#title' => t('Redirect user'),
      '#description' => t('Add "destination" key to the link so user will be redirected on the page where he clicked on the link'),
      '#default_value' => $this->options['redirect'],
    );
  }

  function render($values) {
    $text = !empty($this->options['text']) ? $this->options['text'] : t('view');
    $activity_id = $this->get_value($values, 'activity_id');

    if ($this->options['redirect']) {
      return l($text, 'crm-core/activity/' . $activity_id . '/edit', array('query' => array('destination' => $_GET['q'])));
    }
    else {
      return l($text, 'crm-core/activity/' . $activity_id . '/edit');
    }
  }
}

class crm_core_activity_handler_field_link_delete extends crm_core_activity_handler_field_link_edit {
  function render($values) {
    $text = !empty($this->options['text']) ? $this->options['text'] : t('view');
    $activity_id = $this->get_value($values, 'activity_id');

    if ($this->options['redirect']) {
      return l($text, 'crm-core/activity/' . $activity_id . '/delete', array('query' => array('destination' => $_GET['q'])));
    }
    else {
      return l($text, 'crm-core/activity/' . $activity_id . '/delete');
    }
  }
}

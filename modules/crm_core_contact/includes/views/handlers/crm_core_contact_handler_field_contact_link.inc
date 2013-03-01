<?php

/**
 * A link to a CRM Core Contact.
 */
class crm_core_contact_handler_field_contact extends views_handler_field {
  function construct() {
    parent::construct();

    $this->additional_fields['contact_id'] = 'contact_id';
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
    $contact_id = $values->{$this->aliases['contact_id']};

    return l($text, 'crm-core/contact/' . $contact_id);
  }
}

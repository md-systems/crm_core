<?php

/**
 * Field handler to provide simple renderer that allows linking to a contact.
 */
class crm_contact_handler_field_contact_revision extends views_handler_field {
  function init(&$view, $options) {
    parent::init($view, $options);    
  }


  function option_definition() {
    $options = parent::option_definition();    
    return $options;
  }


  /**
   * Provide the link to contact option.
   */
  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);

    // TODO: perhaps we need some link to reviosion edit page or so
  }
  

  function render($values) {
    return $values->vid;
  }
}
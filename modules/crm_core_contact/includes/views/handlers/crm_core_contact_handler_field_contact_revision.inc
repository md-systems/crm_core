<?php

/**
 * Field handler to provide simple renderer that allows linking to a contact.
 */
class crm_core_contact_handler_field_contact_revision extends views_handler_field {
  function init(&$view, &$options) {
    parent::init($view, $options);    
  
    if (!empty($this->options['link_to_revision'])) {
      $this->additional_fields['contact_id'] = 'contact_id';
      $this->additional_fields['vid'] = 'vid';
    }
  
  }
  
  function option_definition() {
    $options = parent::option_definition();
    $options['link_to_revision'] = array('default' => FALSE);    
    return $options;
  }

  /**
   * Provide the link to contact option.
   */
  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);

    // TODO: perhaps we need some link to reviosion edit page or so
    $form['link_to_revision'] = array(
      '#title' => t('Link this field to the revert the contact to previous version.'),
      '#description' => t('This will override any other link you have set.'),
      '#type' => 'checkbox',
      '#default_value' => !empty($this->options['link_to_revision']),
    );
    
    
  }
  
  /**
   * Render whatever the data is as a link to the contact.
   *
   * Data should be made XSS safe prior to calling this function.
   */
  function render_link($data, $values) {
    if (!empty($this->options['link_to_revision']) && $data !== NULL && $data !== '') {
      $this->options['alter']['make_link'] = TRUE;
      $this->options['alter']['path'] = 'crm-core/contact/' . $values->{$this->aliases['contact_id']} . '/revert/' . $values->{$this->aliases['vid']};
    }

    return $data;
  }

  function render($values) {
    return $this->render_link(check_plain($values->{$this->field_alias}), $values);
  }
}

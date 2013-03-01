<?php

/**
 * Field handler to provide simple renderer that allows linking to a contact.
 */
class crm_core_contact_handler_field_contact_revert extends views_handler_field {
  function init(&$view, $options) {
    parent::init($view, $options);    
      $this->additional_fields['contact_id'] = 'contact_id';
      $this->additional_fields['vid'] = 'vid';
  }
 
  function access() {
    return user_access('administer contact types') || user_access('revert contact record');
  }
 
  function option_definition() {
    $options = parent::option_definition();

    $options['text'] = array('default' => '', 'translatable' => TRUE);

    return $options;
  }

  function options_form(&$form, &$form_state) {
    $form['text'] = array(
      '#type' => 'textfield',
      '#title' => t('Text to display'),
      '#default_value' => $this->options['text'],
    );
    parent::options_form($form, $form_state);
  }
  
  function query() {
    $this->ensure_my_table();
    $this->add_additional_fields();
  }
  
  /**
   * Render whatever the data is as a link to the contact.
   *
   * Data should be made XSS safe prior to calling this function.
   */
  function render_link($data, $values) {
    
    // do a db check to get the highest version for this contact
    $max_vid = db_query("SELECT max(vid) FROM {crm_core_contact_revision} WHERE contact_id = :contact_id", 
    array(':contact_id' => $values->{$this->aliases['contact_id']}))->fetchField();
    
    if ($values->{$this->aliases['vid']} == $max_vid) {
      return t('current');
    }
    
    $this->options['alter']['make_link'] = TRUE;
    $this->options['alter']['path'] = 'crm-core/contact/' . $values->{$this->aliases['contact_id']} . '/revert/' . $values->{$this->aliases['vid']};

    $text = !empty($this->options['text']) ? $this->options['text'] : t('revert');
    return $text;
  }

  function render($values) {
    
    return $this->render_link('', $values);
  }
}

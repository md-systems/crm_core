<?php
/**
 * @file
 * Field handler to provide simple renderer that allows linking to a contact.
 */

/**
 * Handler class.
 */
class crm_core_contact_handler_field_contact_type extends views_handler_field {

  /**
   * Options definitions.
   */
  function option_definition() {
    $options = parent::option_definition();
    $options['link_to_contact_type'] = array('default' => FALSE);
    $options['machine_name'] = array('default' => FALSE);

    return $options;
  }

  /**
   * Options form builder.
   */
  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);

    // Provide the link to contact type page.
    $form['link_to_contact_type'] = array(
      '#title' => t('Link this field to the contact type administrative page.'),
      '#description' => t('This will override any other link you have set.'),
      '#type' => 'checkbox',
      '#default_value' => !empty($this->options['link_to_contact_type']),
    );

    // Display contact type as machine readable name.
    $form['machine_name'] = array(
      '#title' => t('Display contact type as machine readable name?'),
      '#type' => 'checkbox',
      '#default_value' => !empty($this->options['machine_name']),
    );
  }

  /**
   * Render callback.
   */
  function render($values) {

    if ($this->options['link_to_contact_type']) {
      $this->options['alter']['make_link'] = TRUE;
      $this->options['alter']['path'] = 'admin/structure/crm-core/contact-types/manage/' . $values->{$this->field_alias};
    }

    $type = $this->options['machine_name'] ? $values->{$this->field_alias} : crm_core_contact_type_get_name($values->{$this->field_alias});

    return check_plain($type);
  }
}

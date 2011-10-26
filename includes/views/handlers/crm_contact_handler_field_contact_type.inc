<?php

/* 
 * Field handler to translate a contact type into its readable form.
 */
class crm_core_contact_handler_field_contact_type extends crm_core_contact_handler_field_contact {
  function render($values) {
    $value = crm_core_contact_type_get_name($values->{$this->field_alias});
    
    return $this->render_link((check_plain($value)), $values);
  }
}
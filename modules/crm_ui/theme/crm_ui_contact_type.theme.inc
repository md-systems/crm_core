<?php

function theme_crm_core_contact_type_overview($vars) {
  $contact_type = $vars['contact_type'];

  $output = check_plain($contact_type->name);
  $output .= ' <small> (Machine name: ' . check_plain($contact_type->type) . ')</small>';
  $output .= '<div class="description">' . filter_xss_admin($contact_type->description) . '</div>';

  return $output;
}

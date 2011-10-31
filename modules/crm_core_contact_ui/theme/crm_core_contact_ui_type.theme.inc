<?php

function theme_crm_core_contact_ui_type_overview($vars) {
  $contact_type = $vars['contact_type'];
  $output = '';

  if ((bool) $contact_type->disabled) {
    $output .= '<span style="text-decoration:line-through">';
  }

  $output .= check_plain($contact_type->name);
  $output .= ' <small> (Machine name: ' . check_plain($contact_type->type) . ')</small>';
  $output .= '<div class="description">' . filter_xss_admin($contact_type->description) . '</div>';
  
  if ((bool) $contact_type->disabled) {
    $output .= '</span>';
  }

  return $output;
}

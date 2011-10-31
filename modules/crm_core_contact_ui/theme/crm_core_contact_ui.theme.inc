<?php

function theme_crm_core_contact_ui_add_list($variables) {
  $content = $variables['content'];
  $output = '';

  if ($content) {
    $output .= '<dl>';
    foreach ($content as $item) {
      $output .= '<dt>' . l($item['title'], $item['href'], $item['localized_options']) . '</dt>';
      $output .= '<dd>' . filter_xss_admin($item['description']) . '</dd>';
    }
    $output .= '</dl>';
  }

  return $output;
}

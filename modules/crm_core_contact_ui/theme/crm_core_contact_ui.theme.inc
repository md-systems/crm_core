<?php

/**
 * @file
 * Contains theme implementation.
 */

/**
 * Renders list of contact types available for contact creation.
 */
function theme_crm_core_contact_ui_add_list($variables) {
  $content = $variables['content'];
  $output = '';

  if ($content) {
    $output .= '<dl>';
    foreach ($content as $item) {
      $output .= '<dt>' . l(t($item['title']), $item['href'], $item['localized_options']) . '</dt>';
      $output .= '<dd>' . t($item['description']) . '</dd>';
    }
    $output .= '</dl>';
  }

  return $output;
}

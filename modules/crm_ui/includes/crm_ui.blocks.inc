<?php

/**
 * Subject for CRM navigation block.
 */
function crm_ui_block_navigation_subject() {
 return t('CRM Navigation');
}

/**
 * Content for CRM navigation block.
 */
function crm_ui_block_navigation_content() {
  $items = array(
    'contacts' => l(t('Contacts'), 'crm/contact'),
    'add_contact' => l(t('Add Contact'), 'crm/contact/add'),
  );

  return array(
    '#theme' => 'item_list',
    '#items' => $items,
  );
}

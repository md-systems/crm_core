<?php

/**
 * Subject for CRM navigation block.
 */
function crm_core_ui_block_navigation_subject() {
 return t('CRM Navigation');
}

/**
 * Content for CRM navigation block.
 */
function crm_core_ui_block_navigation_content() {
  
  global $user;
  
  if(!user_access('View any contact of any type', $user)){
    return;
  }
  
  $items = array(
    'contacts' => l(t('Contacts'), 'crm-core/contact'),
    'add_contact' => l(t('Add Contact'), 'crm-core/contact/add'),
  );

  return array(
    '#theme' => 'item_list',
    '#items' => $items,
  );
}

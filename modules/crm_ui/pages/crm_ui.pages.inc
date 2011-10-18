<?php

/**
 * The dashboard for the CRM.  Currently
 */
function crm_dashboard() {
  $block = block_load('crm_ui', 'navigation');
  $content = _block_render_blocks(array($block)); 
  $build = _block_get_renderable_array($content);
  return $build;
}

/**
 * Page callback for CRM Administration dashboard.
 */
function crm_ui_admin_dashboard() {
  $build['contacts'] = array(
    '#theme' => 'item_list__crm_ui_admin_dashboard',
    '#title' => t('Contacts'),
    '#items' => array(
      'contact_types' => array(
        'data' => l(t('Contact Types'), 'crm/admin/contact/types'),
        'class' => array('admin-dashboard', 'contact-types'),
        ),
      ),
  );

  $build['activities'] = array(
    '#theme' => 'item_list__crm_ui_admin_dashboard',
    '#title' => t('Activities'),
    '#items' => array(
      'activity_types' => array(
        'data' => l(t('Activity Types'), 'crm/admin/activity/types'),
        'class' => array('admin-dashboard', 'activity-types'),
        ),
      ),
  );

  $build['relationships'] = array(
    '#theme' => 'item_list__crm_ui_admin_dashboard',
    '#title' => t('Relationships'),
    '#items' => array(
      'contact_types' => array(
        'data' => l(t('Relationship Types'), 'crm/admin/relationship/types'),
        'class' => array('admin-dashboard', 'relationship-types'),
        ),
      ),
  );
  
  return $build;
}

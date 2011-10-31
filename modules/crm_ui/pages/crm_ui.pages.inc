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
  $local_tasks = menu_local_tasks();
  $content = array();
  foreach ($local_tasks['tabs']['output'] as $task) {
    $content[] = $task['#link'];
  }

  if ($content) {
    $output = theme('admin_block_content', array('content' => $content));
  }
  else {
    $output = t('You do not have any CRM Core items.');
  }
  return $output;
}
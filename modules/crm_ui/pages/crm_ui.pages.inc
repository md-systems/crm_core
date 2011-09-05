<?php

/**
 * The dashboard for the CRM.  Currently
 */
function crm_dashboard() {
  $block = block_load('crm', 'navigation');
  $content = _block_render_blocks(array($block)); 
  $build = _block_get_renderable_array($content);
  return $build;
}

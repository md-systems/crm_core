<?php

/**
 * Case view callback.
 */
function crm_core_case_view($case) {
  $content = entity_get_controller('crm_core_case')->view(array($case->cid => $case), 'full');

  // We don't want to run output through entity theming function.
  // To avoid this hack apply patch from http://drupal.org/node/1259916
  // Then we can load content in following way
  // $content = entity_build_content('crm_core_case', $case);
  unset($content['crm_core_case'][$case->cid]['#theme']);
  return $content;
}

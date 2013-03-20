<?php

/**
 * @file
 * Contains functionality for testing purposes.
 */


function crm_core_match_testing_page_title($contact) {
  return t('Matching Rules for @contact_name', array('@contact_name' => $contact->label()));
}

function crm_core_match_testing_page($contact) {
  dpm($contact);
  $output = '';

  $rows = array();
  $engines = crm_core_match_get_engines();
  foreach ($engines as $engine) {
    if ($engine->getStatus()) {
      $rows[] = array('data' => array(
        $engine->getID(),
        $engine->getName(),
        $engine->getDescription(),
        $engine->getMachineName(),
        $engine->getWeight(),
      ));
    }
  }
  $header = array(
    t('Engine ID'),
    t('Engine mame'),
    t('Engine description'),
    t('Engine machine name'),
    t('Weight'),
  );

  $output .= theme('table', array(
    'header' => $header,
    'rows' => $rows,
    'empty' => t('No matching engines enabled or associated with contacts of this type.'),
  ));

  return $output;
}

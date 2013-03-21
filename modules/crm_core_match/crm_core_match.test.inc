<?php

/**
 * @file
 * Contains functionality for testing purposes.
 */


function crm_core_match_testing_page_title($contact) {
  return t('Matching Rules for @contact_name', array('@contact_name' => $contact->label()));
}

function crm_core_match_testing_page($contact) {
  $output = '';
  $engines = crm_core_match_get_engines();

  // Display engines table.
  $rows = array();
  foreach ($engines as $engine) {
    if ($engine->getStatus()) {
      $rows[] = array(
        'data' => array(
          $engine->getID(),
          $engine->getName(),
          $engine->getDescription(),
          $engine->getMachineName(),
          $engine->getWeight(),
        ),
      );
    }
  }
  $header = array(
    t('Engine ID'),
    t('Engine name'),
    t('Engine description'),
    t('Engine machine name'),
    t('Weight'),
  );
  $output .= theme('table', array(
    'header' => $header,
    'rows' => $rows,
    'empty' => t('No matching engines enabled or associated with contacts of this type.'),
  ));

  foreach ($engines as $engine) {
    $matches = array();
    $engine->execute($contact, $matches);
  }

  // @TODO: Add matches list.
  $output .= '<h1>Matches:</h1>';
  $output .= theme('item_list', array(
    'items' => $matches,
    'title' => 'Ids of matched contacts',
    'type' => 'ol'
  ));

  return $output;
}

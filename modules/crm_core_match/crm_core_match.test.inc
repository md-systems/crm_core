<?php

/**
 * @file
 * Contains functionality for testing purposes.
 */


function crm_core_match_testing_page_title($contact) {
  return t('Matching Rules for @contact_name', array('@contact_name' => crm_core_contact_title($contact)));
}

/**
 * Page callback to display match debug info.
 *
 * @param object $contact
 *   CRM Core Contact.
 *
 * @return string
 */
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

  if (empty($matches)) {
    $output .= '<h1>No matches currently available.</h1>';
  }
  else {
    $output .= '<h1>Matches:</h1>';

    // Display matched contacts table.
    $rows = array();
    $contacts = crm_core_contact_load_multiple($matches);
    foreach ($contacts as $matched_contact) {
      $uri = $matched_contact->uri();
      $link = l($matched_contact->label(), $uri['path']);
      $rows[] = array(
        'data' => array(
          $matched_contact->contact_id,
          $link,
        ),
      );
    }
    $header = array(
      t('Contact ID'),
      t('Contact Name'),
    );
    $output .= theme('table', array(
      'header' => $header,
      'rows' => $rows,
    ));
  }

  return $output;
}

function crm_core_match_info_page() {
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

  if (module_exists('crm_core_default_matching_engine')) {
    $contact_types = crm_core_contact_types(TRUE);
    foreach (array_keys($contact_types) as $contact_type) {
      $rules = crm_core_default_matching_engine_load_field_config($contact_type);
      // Display rules table.
      $rows = array();
      foreach ($rules as $rule) {
        $rows[] = array(
          'data' => array(
            $rule->mrid,
            $rule->field_name,
            $rule->field_type,
            $rule->field_item,
            $rule->operator,
            $rule->options,
            $rule->score,
            $rule->weight,
          ),
        );
      }

      $header = array(
        t('Rule ID'),
        t('Field name'),
        t('Field type'),
        t('Field item'),
        t('Field operator'),
        t('Field options'),
        t('Field score'),
        t('Field weight'),
      );
      $output .= theme('table', array(
        'header' => $header,
        'caption' => t('Matching rules for @contact_type', array(
          '@contact_type' => $contact_types[$contact_type]->name)
        ),
        'rows' => $rows,
        'empty' => t('Matching is not enabled for this contact type.'),
      ));
    }
  }

  return $output;
}

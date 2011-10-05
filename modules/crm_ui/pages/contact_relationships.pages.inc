<?php

/**
 * Returns a list of relatinships for this contact.
 */
function crm_contact_relationships($crm_contact) {
  return "This is a stub";
}

/**
 * Returns list of links to add relationship for specific contact.
 */
function crm_contact_relationships_add($crm_contact) {

  $relations_links = array();
  $reverse_relations_links = array();

  $bundle = 'crm_contact:' . $crm_contact->type;
  $bundle_any = 'crm_contact:*';

  $relation_types = relation_get_types();
  foreach ($relation_types as $relation_type) {
    if (in_array($bundle, $relation_type->source_bundles) || in_array($bundle_any, $relation_type->source_bundles)) {
      $relations_links[$relation_type->relation_type] = array(
        'href' => 'crm/contact/' . $crm_contact->contact_id . '/relationships/add/' . $relation_type->relation_type . '/0',
        'title' => $relation_type->label,
      );
    }
    if (in_array($bundle, $relation_type->target_bundles) || in_array($bundle_any, $relation_type->target_bundles)) {
      $reverse_relations_links[$relation_type->relation_type] = array(
        'href' => 'crm/contact/' . $crm_contact->contact_id . '/relationships/add/' . $relation_type->relation_type . '/1',
        'title' => $relation_type->reverse_label,
      );
    }
  }
  
  $ret = theme('links', array(
    'links' => $relations_links,
    'heading' => array('text' => t('Relationship types'), 'level' => 'h2'),
  ));

  $ret .= theme('links', array(
    'links' => $reverse_relations_links,
    'heading' => array('text' => t('Reverse relationship types'), 'level' => 'h2'),
  ));

  return $ret;
}

function crm_contact_relationships_add_form($form, &$form_state, $crm_contact, $relation_type, $reverse) {
  $form = array();

  // TODO: we need to take into account directional and non-directional relations
  
  $form['relationship_type'] = array(
    '#type' => 'item',
    '#title' => t('Relationship type') . ': ' . $relation_type->label,
    '#value' => "",
  );
  
  $form['source_contact'] = array(
    '#title' => t('Source contact name'),
    '#type' => 'textfield',
    '#default_value' => !$reverse ? $crm_contact->contact_name[LANGUAGE_NONE][0]['safe_value'] . " [cid:$crm_contact->contact_id]" : '',
    '#disabled' => !$reverse,
    '#required' => true,
    '#autocomplete_path' => 'crm/contact/' . $crm_contact->contact_id . '/relationships/add/' . $relation_type->relation_type . '/' . $reverse . '/autocomplete',
  );

  $form['destination_contact'] = array(
    '#title' => t('Destination contact name'),
    '#type' => 'textfield',
    '#default_value' => $reverse ? $crm_contact->contact_name[LANGUAGE_NONE][0]['safe_value'] . " [cid:$crm_contact->contact_id]" : '',
    '#disabled' => $reverse,
    '#required' => true,
    '#autocomplete_path' => 'crm/contact/' . $crm_contact->contact_id . '/relationships/add/' . $relation_type->relation_type . '/' . $reverse . '/autocomplete',
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Add relationship'),
  );
    
  return $form;
}

function crm_contact_relationships_add_autocomplete($crm_contact, $relation_type, $reverse, $string) {
  $matches = array();

  // $bundle = 'crm_contact:' . $crm_contact->type;
    
  $query = new EntityFieldQuery;

  $result = $query
    ->entityCondition('entity_type', 'crm_contact')
    ->propertyCondition('contact_id', $crm_contact->contact_id, '<>')
    ->fieldCondition('contact_name', 'value', $string, 'STARTS_WITH')
    ->execute();

  $contacts = entity_load('crm_contact', array_keys($result['crm_contact']));

  foreach ($contacts as $contact) {
    $matches[$contact->contact_name[LANGUAGE_NONE][0]['safe_value'] . " [cid:$contact->contact_id]"] = $contact->contact_name[LANGUAGE_NONE][0]['safe_value'];
  }
  // return for JS
  drupal_json_output($matches);
}
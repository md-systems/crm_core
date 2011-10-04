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
  $bundle = 'crm_contact:' . $crm_contact->type;
  $relations = relation_get_types();

  $relations_links = array();
  $reverse_relations_links = array();

  foreach ($relations as $relation) {
    if (in_array($bundle, $relation->source_bundles)) {
      $relations_links[$relation->relation_type] = array(
        'href' => 'crm/contact/' . $crm_contact->contact_id . '/relationships/add/' . $relation->relation_type,
        'title' => $relation->label,
      );
    }
    if (in_array($bundle, $relation->target_bundles)) {
      $reverse_relations_links[$relation->relation_type] = array(
        'href' => 'crm/contact/' . $crm_contact->contact_id . '/relationships/add/' . $relation->relation_type,
        'title' => $relation->reverse_label,
      );
    }
  }
  
  $ret = theme('links', array(
    'links' => $relations_links,
    'heading' => array('text' => t('Relations'), 'level' => 'h2'),
  ));

  $ret .= theme('links', array(
    'links' => $reverse_relations_links,
    'heading' => array('text' => t('Reverse relations'), 'level' => 'h2'),
  ));

  return $ret;
}
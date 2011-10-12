<?php

/**
 * Return a list of relatinships for this contact.
 *
 * @param $crm_contact
 *   Entity object of crm_contact type.
 */
function crm_contact_relationships($crm_contact) {
  return "This is a stub";
}

/**
 * Return a list of links to add relationship for specific CRM contact.
 *
 * @param $crm_contact
 *   Entity object of crm_contact type.
 */
function crm_contact_relationships_add($crm_contact) {

  $relations_links = array();
  $reverse_relations_links = array();

  $bundle = 'crm_contact:' . $crm_contact->type;
  $bundle_any = 'crm_contact:*';

  $relation_types = relation_get_types();
  $crm_relationship_types = variable_get('crm_relationship_settings_types', array());
  foreach ($relation_types as $relation_type) {
    if (!in_array($relation_type->relation_type, $crm_relationship_types, true)) continue;
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

/**
 * Generate add relationship form.
 *
 * @param $crm_contact
 *   Entity object of crm_contact type.
 * @param $relation_type
 *   Relation type object.
 * @param $reverse
 *   Whether relation is reversed.
 */
function crm_contact_relationships_add_form($form, &$form_state, $crm_contact, $relation_type, $reverse) {
  $form = array();

  $form['relation_type'] = array(
    '#type' => 'value',
    '#value' => $relation_type,
  );
  $form['reverse'] = array(
    '#type' => 'value',
    '#value' => $reverse,
  );
  $form['relationship_type'] = array(
    '#type' => 'item',
    '#title' => t('Relationship type') . ': ' . $relation_type->label,
    '#value' => "",
  );

  $contact_type_names = array();
  foreach (_crm_contact_relationships_get_allowed_contact_types($relation_type, 0) as $contact_type) {
    $contact_type_names[] = t($contact_type->name);
  }
  $contact_type_names = implode(', ', $contact_type_names);
  
  $form['source_contact'] = array(
    '#title' => t('Source contact name'),
    '#description' => (t('Following contact types allowed: !types.', array('!types' => $contact_type_names))),
    '#type' => 'textfield',
    '#default_value' => !$reverse ? $crm_contact->contact_name[LANGUAGE_NONE][0]['safe_value'] . " [cid:$crm_contact->contact_id]" : '',
    '#disabled' => !$reverse,
    '#required' => true,
    '#autocomplete_path' => 'crm/contact/' . $crm_contact->contact_id . '/relationships/add/' . $relation_type->relation_type . '/0/autocomplete',
  );

  $contact_type_names = array();
  foreach (_crm_contact_relationships_get_allowed_contact_types($relation_type, 1) as $contact_type) {
    $contact_type_names[] = t($contact_type->name);
  }
  $contact_type_names = implode(', ', $contact_type_names);

  $form['destination_contact'] = array(
    '#title' => t('Destination contact name'),
    '#description' => (t('Following contact types allowed: !types.', array('!types' => $contact_type_names))),
    '#type' => 'textfield',
    '#default_value' => $reverse ? $crm_contact->contact_name[LANGUAGE_NONE][0]['safe_value'] . " [cid:$crm_contact->contact_id]" : '',
    '#disabled' => $reverse,
    '#required' => true,
    '#autocomplete_path' => 'crm/contact/' . $crm_contact->contact_id . '/relationships/add/' . $relation_type->relation_type . '/1/autocomplete',
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Add relationship'),
  );
  
  return $form;
}

/**
 * Perform validation for add relationship form.
 */
function crm_contact_relationships_add_form_validate($form, &$form_state) {
  $relation_type = $form_state['values']['relation_type'];
  $reverse = $form_state['values']['reverse'];
  $source_contact = _crm_contact_relationships_get_contact_from_autocomplete_field($form_state['values']['source_contact']);
  $destination_contact = _crm_contact_relationships_get_contact_from_autocomplete_field($form_state['values']['destination_contact']);

  if (!$relation_type || !is_object($relation_type)) {
    form_set_error('relation_type', t('Relation type is required.'));
  }

  if (!$source_contact) {
    form_set_error('source_contact', t('Please, input source contact.'));
  }
  if (!$destination_contact) {
    form_set_error('destination_contact', t('Please, input destination contact.'));
  }

  $source_bundles = array_keys(_crm_contact_relationships_get_allowed_contact_types($relation_type, 0));
  if (!in_array($source_contact->type, $source_bundles)) {
   form_set_error('source_contact', t('Please, input source contact of proper type.'));
  }

  $target_bundles = array_keys(_crm_contact_relationships_get_allowed_contact_types($relation_type, $relation_type->directional));
  if (!in_array($destination_contact->type, $target_bundles)) {
   form_set_error('destination_contact', t('Please, input destination contact of proper type.'));
  }

  if ($relation_type->r_unique) {
    if(relation_relation_exists(
      array(
        array('entity_type' => 'crm_contact', 'entity_id' => $source_contact->contact_id),
        array('entity_type' => 'crm_contact', 'entity_id' => $destination_contact->contact_id),
      ),
      $relation_type->relation_type
    )) {
      form_set_error($reverse ? 'source_contact' : 'destination_contact', t('This relationship exists. It should be unique.'));
    }
  }
}

/**
 * Perform submit for add relationship form. Save relationship.
 */
function crm_contact_relationships_add_form_submit($form, &$form_state) {
  $relation_type = $form_state['values']['relation_type'];
  $reverse = $form_state['values']['reverse'];
  $source_contact = _crm_contact_relationships_get_contact_from_autocomplete_field($form_state['values']['source_contact']);
  $destination_contact = _crm_contact_relationships_get_contact_from_autocomplete_field($form_state['values']['destination_contact']);

  // Create relationship between the order and the node
  $endpoints = array(
    0 => array(
      'entity_type' => 'crm_contact',
      'entity_id' => $source_contact->contact_id,
      'entity_bundle' => $source_contact->type,
      'r_index' => 0,
      'entity_key' => $source_contact->type . ': ' . $source_contact->contact_id
    ),
    1 => array(
      'entity_type' => 'crm_contact',
      'entity_id' => $destination_contact->contact_id,
      'entity_bundle' => $destination_contact->type,
      'r_index' => 1,
      'entity_key' => $destination_contact->type . ': ' . $destination_contact->contact_id
    ),
  );
  $relation = relation_create($relation_type->relation_type, $endpoints);
  $rid = relation_save($relation);

  $redirect_contact = $reverse ? $destination_contact : $source_contact;
  $form_state['redirect'] = 'crm/contact/' . $redirect_contact->contact_id . '/relationships/';
}

/**
 * Autocomplete function for add relationship form. Lookup for contacts.
 * 
 * @param $crm_contact
 *   Entity object of crm_contact type.
 * @param $relation_type
 *   Relation type object.
 * @param $reverse
 *   Whether relation is reversed.
 * @param $string
 *   String CRM contact is started with.
 */
function crm_contact_relationships_add_autocomplete($crm_contact, $relation_type, $reverse, $string) {
  $matches = array();
  
  $contact_types = array_keys(_crm_contact_relationships_get_allowed_contact_types($relation_type, $reverse));
  
  $query = new EntityFieldQuery;
  $result = $query
    ->entityCondition('entity_type', 'crm_contact')
    ->propertyCondition('contact_id', $crm_contact->contact_id, '<>')
    ->propertyCondition('type', $contact_types, 'IN')
    ->fieldCondition('contact_name', 'value', $string, 'STARTS_WITH')
    ->execute();

  $contacts = entity_load('crm_contact', array_keys($result['crm_contact']));
  foreach ($contacts as $contact) {
    $matches[$contact->contact_name[LANGUAGE_NONE][0]['safe_value'] . " [cid:$contact->contact_id]"] = $contact->contact_name[LANGUAGE_NONE][0]['safe_value'];
  }
  
  // return for JS
  drupal_json_output($matches);
}

/**
 * Return contact types available for specific relation.
 *
 * @param $relation_type
 *   Relation type object.
 * @param $reverse
 *   Whether relation is reversed.
 */
function _crm_contact_relationships_get_allowed_contact_types($relation_type, $reverse) {
  $allowed_contact_types = array();
  
  $contact_types = crm_contact_types();
  $bundles = $relation_type->directional && $reverse ? $relation_type->target_bundles : $relation_type->source_bundles;

  if (in_array('crm_contact:*', $bundles)) {
    return $contact_types;
  }
  
  foreach ($bundles as $bundle) {
    list($entity, $type) = explode(':', $bundle);
    if ($entity == 'crm_contact' && $contact_types[$type]) {
      $allowed_contact_types[$type] = $contact_types[$type];
    }
  }
  
  return $allowed_contact_types;
}

/**
 * Extract contact id from the value of autocomplete contact field.
 *
 * @param $string
 *   String that is processed.
 */
function _crm_contact_relationships_get_contact_from_autocomplete_field($string) {
  $matches = array();
  preg_match('/\[cid:([0-9]+)\]/', $string, $matches);
  if (!array_key_exists(1, $matches) || !is_numeric($matches[1])) {
    return false;
  }
  $contacts = entity_load('crm_contact', array($matches[1]));
  if (empty($contacts)) {
    return false;
  }
  return $contacts[$matches[1]];
}
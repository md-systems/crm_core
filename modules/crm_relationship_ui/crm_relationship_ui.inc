<?php

/**
 * Return contact types available for specific relation type.
 *
 * @param $relation_type
 *   Relation type object.
 * @param $reverse
 *   Whether relation is reversed.
 */
function crm_relationship_ui_load_relationship_type_contacts($relation_type, $reverse) {
  // Check if it is a relationship type  
  if (!crm_relationship_ui_is_relationship_type($relation_type->relation_type)) {
    return array();
  }
  
  $contact_types = crm_contact_types();
  $bundles = $relation_type->directional && $reverse ? $relation_type->target_bundles : $relation_type->source_bundles;
  
  if (in_array('crm_contact:*', $bundles)) {
    return $contact_types;
  }
  
  $available_contact_types = array();
  foreach ($bundles as $bundle) {
    list($entity, $type) = explode(':', $bundle);
    $available_contact_types[$type] = $contact_types[$type];
  }
  
  return $available_contact_types;
}

/**
 * Check if relation_type is CRM relationship_type.
 * @param $relation_type
 *   Relation type name provided by Relation module.
 */
function crm_relationship_ui_is_relationship_type($relation_type) {
  static $types = array();
  
  if (isset($types[$relation_type])) {
    return $types[$relation_type];
  } elseif ($relation_type_object = relation_type_load($relation_type)) {
    $ret = true;
    
    // It should be between CRM contact types only
    $ret = $ret && crm_relationship_ui_is_contact_bundles($relation_type_object->source_bundles);
    if ($relation_type_object->directional) {
      $ret = $ret && crm_relationship_ui_is_contact_bundles($relation_type_object->target_bundles);
    }
    // It should be unique
    $ret = $ret && $relation_type_object->r_unique;
    // Arity should be equal to 2
    $ret = $ret && ($relation_type_object->min_arity == 2) && ($relation_type_object->max_arity == 2);
    
    $types[$relation_type] = $ret;
    return $ret;
  }
  
  return false;
}

/**
 * Return true if there is only CRM contacts in relation type bundles. 
 * @param $bundles
 *   Array of relation type bundles.
 */
function crm_relationship_ui_is_contact_bundles($bundles) {    
  foreach ($bundles as $bundle) {
    list($entity, $type) = explode(':', $bundle);
    if ($entity != 'crm_contact') {
      return false;
    }
  }
  return true;
}
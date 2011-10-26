<?php

/**
 * Return contact types available for specific relationship type.
 *
 * @param $relation_type
 *   Relation type object.
 * @param $reverse
 *   Whether relationship is reversed.
 */
function crm_core_relationship_ui_load_contact_types($relation_type, $reverse = 0) {
  // Check if it is a relationship type  
  if (!crm_core_relationship_ui_is_relationship_type($relation_type->relation_type)) {
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
 * Return relationship types that can be applied to specific contact type.
 * 
 * @param type $contact_type
 *   CRM contact type name.
 * @param type $reverse 
 *   Whether relationship is reversed.
 */
function crm_core_relationship_ui_load_relationship_types($contact_type, $reverse = 0) {
  $relation_types = relation_get_types();
  
  $available_relationship_types = array();
  
  foreach ($relation_types as $relation_type) {
    if (!crm_core_relationship_ui_is_relationship_type($relation_type->relation_type)) continue;
    
    $bundles = $reverse ? $relation_type->target_bundles : $relation_type->source_bundles;
    if (in_array('crm_contact:' . $contact_type, $bundles) || in_array('crm_contact:*', $bundles)) {
      $available_relationship_types[] = $relation_type;  
    }
  }
  
  return $available_relationship_types;
}

/**
 * Check if relation_type is CRM relationship_type.
 * 
 * @param $relation_type
 *   Relation type name or relation object provided by Relation module.
 */
function crm_core_relationship_ui_is_relationship_type($relation_type) {
  static $types = array();
  
  if (is_object($relation_type)) {
    $relation_type_object = $relation_type;
    $relation_type = $relation_type_object->relation_type;
  }
  
  if (isset($types[$relation_type])) {
    return $types[$relation_type];
  }
  
  if (!isset($relation_type_object)) {
    $relation_type_object = relation_type_load($relation_type);
  }
  
  if ($relation_type_object) {
    $ret = true;
    
    // It should be between CRM contact types only
    $ret = $ret && crm_core_relationship_ui_is_contact_bundles($relation_type_object->source_bundles);
    if ($relation_type_object->directional) {
      $ret = $ret && crm_core_relationship_ui_is_contact_bundles($relation_type_object->target_bundles);
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
 * 
 * @param $bundles
 *   Array of relation type bundles.
 */
function crm_core_relationship_ui_is_contact_bundles($bundles) {    
  foreach ($bundles as $bundle) {
    list($entity, $type) = explode(':', $bundle);
    if ($entity != 'crm_contact') {
      return false;
    }
  }
  return true;
}
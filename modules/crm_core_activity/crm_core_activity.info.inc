<?php

/**
 * Implements hook_property_info().
 */
function crm_core_activity_entity_property_info() {
  $info = array();

  // Add meta-data about crm_activity properties.
  $properties = &$info['crm_core_activity']['properties'];

  $properties['activity_id'] = array(
    'label' => t('Activity ID'),
    'description' => t('The primary identifier for an activity.'),
    'type' => 'integer',
    'schema field' => 'activity_id',
  );
  $properties['type'] = array(
    'label' => t('Type'),
    'description' => t('The type (bundle) of the activity.'),
    'type' => 'token',
    'setter callback' => 'entity_property_verbatim_set',
    'setter permission' => 'administer site configuration',
    'options list' => 'crm_core_activity_type_get_name',
    'required' => TRUE,
    'schema field' => 'type',
  );
  $properties['title'] = array(
    'label' => t('Title'),
    'description' => t('Title of the activity.'),
    'type' => 'text',
    'setter callback' => 'entity_property_verbatim_set',
    'setter permission' => 'administer site configuration',
    'required' => TRUE,
    'schema field' => 'title',
  );
  $properties['uid'] = array(
    'label' => t('User ID'),
    'type' => 'integer',
    'description' => t("The unique ID of the user created activity."),
    'setter callback' => 'entity_property_verbatim_set',
    'setter permission' => 'administer site configuration',
    'schema field' => 'uid',
  );
  $properties['created'] = array(
    'label' => t('Date created'),
    'description' => t('The date the product was created.'),
    'type' => 'date',
    'setter callback' => 'entity_property_verbatim_set',
    'setter permission' => 'dminister site configuration',
    'schema field' => 'created',
  );
  $properties['changed'] = array(
    'label' => t('Date updated'),
    'description' => t('The date the product was last updated.'),
    'type' => 'date',
    'setter callback' => 'entity_property_verbatim_set',
    'setter permission' => 'Administer site configuration',
    'schema field' => 'changed',
  );


  // Metadata of Activity Type.
  $properties = &$info['crm_core_activity_type']['properties'];

  $properties['id'] = array(
    'label' => t('Activity Type ID'),
    'description' => t('Primary Key: Unique activity type ID.'),
    'type' => 'integer',
    'schema field' => 'id',
  );
  $properties['type'] = array(
    'label' => t('Type'),
    'description' => t('The machine-readable name of this type.'),
    'type' => 'token',
    'setter callback' => 'entity_property_verbatim_set',
    'setter permission' => 'administer site configuration',
    'required' => TRUE,
    'schema field' => 'type',
  );
  $properties['label'] = array(
    'label' => t('Label'),
    'description' => t('The human-readable name of this type.'),
    'type' => 'text',
    'setter callback' => 'entity_property_verbatim_set',
    'setter permission' => 'administer site configuration',
    'required' => TRUE,
    'schema field' => 'label',
  );
  $properties['description'] = array(
    'label' => t('Description'),
    'description' => t('A brief description of this type.'),
    'type' => 'text',
    'setter callback' => 'entity_property_verbatim_set',
    'setter permission' => 'administer site configuration',
    'schema field' => 'description',
  );

  return $info;
}


<?php

/**
 * @file
 * Hooks provided by the CRM Core module.
 */

/**
 * @addtogroup hooks
 * @{
 */

/**
 * Deny or allow access to entity CRUD before any other access check.
 *
 * Modules implementing this hook can return FALSE to provide a blanket
 * prevention for the user to perform the requested operation on the specified
 * entity. If no modules implementing this hook return FALSE but at least one
 * returns TRUE, then the operation will be allowed, even for a user without
 * role based permission to perform the operation.
 *
 * If no modules return FALSE but none return TRUE either, normal permission
 * based checking will apply.
 *
 * @param string $op
 *   The request operation: update, create, or delete.
 * @param object $entity
 *   The entity to perform the operation on.
 * @param object $account
 *   The user account whose access should be determined.
 * @param string $entity_type
 *   The machine-name of the entity type of the given $entity.
 *
 * @return bool
 *   TRUE or FALSE indicating an explicit denial of permission or a grant in the
 *   presence of no other denials; NULL to not affect the access check at all.
 */
function hook_crm_core_entity_access($op, $entity, $account, $entity_type) {
  // No example.
}

/**
 * Use a custom label for a contact of bundle CONTACT_BUNDLE.
 */
function crm_core_contact_CONTACT_BUNDLE_label($entity) {
  // No example.
}

/**
 * Respond to CRM Core contacts being merged.
 *
 * @param CRMCoreContactEntity $master_contact
 *   Contact to which data being merged.
 * @param array $merged_contacts
 *   Keyed by contact ID array of contacts being merged.
 *
 * @see crm_core_contact_merge_contacts_action()
 */
function hook_crm_core_contact_merge_contacts(CRMCoreContactEntity $master_contact, array $merged_contacts) {

}

/**
 * Provides possibility to change default fields that will be added to the
 * recently created bundle of activity.
 *
 * @param $fields
 *   Array with fields that are going to be added to the activity bundle.
 * @param CRMActivityType $activity_type
 *   Bundle of activity entity that was recently created.
 *
 * @see field_create_field()
 * @see _crm_core_activity_type_default_fields()
 */
function hook_crm_core_activity_type_add_fields_alter(&$fields, CRMActivityType $activity_type) {
  // Prevent field_activity_date from creation.
  foreach ($fields as $key => $field) {
    if ($field['field_name'] == 'field_activity_date') {
      unset($fields[$key]);
    }
  }
}

/**
 * Provides possibility to change default field instances that will be added to
 * the recently created bundle of activity.
 *
 * @param $instances
 *   Array with field instances that are going to be added to the activity
 *   bundle.
 * @param CRMActivityType $activity_type
 *   Bundle of activity entity that was recently created.
 *
 * @see field_create_instance()
 * @see _crm_core_activity_type_default_field_instances()
 */
function hook_crm_core_activity_type_add_field_instances_alter(&$instances, CRMActivityType $activity_type) {
  // Prevent field_activity_date from adding to an activity bundle.
  foreach ($instances as $key => $instance) {
    if ($instance['field_name'] == 'field_activity_date') {
      unset($instances[$key]);
    }
  }
}

/**
 * @} End of "addtogroup hooks".
 */

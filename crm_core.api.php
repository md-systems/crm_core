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
 * @} End of "addtogroup hooks".
 */

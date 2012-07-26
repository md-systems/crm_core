<?php

if (!module_exists('entityreference')) {
  exit();  
}

/**
 * Overwrite entity reference handler class so autocomplete widget works with crm_core_contact
 */
class EntityReference_SelectionHandler_Generic_crm_core_contact extends EntityReference_SelectionHandler_Generic {
  /**
   * Build an EntityFieldQuery to get referencable entities.
   */
  protected function buildEntityFieldQuery($match = NULL, $match_operator = 'CONTAINS') {
    $query = new EntityFieldQuery();
    $query->entityCondition('entity_type', $this->field['settings']['target_type']);
    if (!empty($this->field['settings']['handler_settings']['target_bundles'])) {
      $query->entityCondition('bundle', $this->field['settings']['handler_settings']['target_bundles'], 'IN');
    }
    if (isset($match)) {
      $group_name = 'crm_core_contact_search';
      $query->fieldCondition('contact_name', 'family', $match, $match_operator, $group_name);
      $query->addTag('crm_core_contact_search');
    }

    // Add a generic entity access tag to the query.
    $query->addTag($this->field['settings']['target_type'] . '_access');
    $query->addTag('entityreference');
    $query->addMetaData('field', $this->field);
    $query->addMetaData('entityreference_selection_handler', $this);

    // Add the sort option.
    if (!empty($this->field['settings']['handler_settings']['sort'])) {
      $sort_settings = $this->field['settings']['handler_settings']['sort'];
      if ($sort_settings['type'] == 'property') {
        $query->propertyOrderBy($sort_settings['property'], $sort_settings['direction']);
      }
      elseif ($sort_settings['type'] == 'field') {
        list($field, $column) = explode(':', $sort_settings['field'], 2);
        $query->fieldOrderBy($field, $column, $sort_settings['direction']);
      }
    }
    return $query;
  }
}

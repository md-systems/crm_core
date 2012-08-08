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
      $query->addMetaData('match', $match);
      $query->addMetaData('match_operator', $match_operator);
      $query->addTag(variable_get('crm_core_contact_search_query_tag', 'crm_core_contact_search'));

      // Fields are added in crm_core_contact_query_crm_core_contact_search_alter().
      // You can override variable crm_core_contact_search_query_tag to change hook name.
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

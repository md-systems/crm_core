<?php

class CRMCaseController extends EntityAPIController {
   public function buildContent($entity, $view_mode = 'full', $langcode = NULL, $content = array()) {
    // Remove previously built content, if exists.
    $entity->content = $content;
    $langcode = isset($langcode) ? $langcode : $GLOBALS['language_content']->language;

    // Make Description and Status themed like default fields.
    $entity->content['description'] = array(
      '#theme' => 'field',
      '#weight' => 0,
      '#title' =>t('Description'),
      '#access' => TRUE,
      '#label_display' => 'above',
      '#view_mode' => 'full',
      '#language' => LANGUAGE_NONE,
      '#field_name' => 'field_fake_description',
      '#field_type' => 'text',
      '#entity_type' => 'crm_core_case',
      '#bundle' => $entity->type,
      '#items' => array(array('value' => $entity->description)),
      '#formatter' => 'text_default',
      0 => array('#markup' => check_plain($entity->description))
    );

    $entity->content['status'] = array(
      '#theme' => 'field',
      '#weight' => 0,
      '#title' =>t('Status'),
      '#access' => TRUE,
      '#label_display' => 'above',
      '#view_mode' => 'full',
      '#language' => LANGUAGE_NONE,
      '#field_name' => 'field_fake_status',
      '#field_type' => 'text',
      '#entity_type' => 'crm_core_case',
      '#bundle' => $entity->type,
      '#items' => array(array('value' => $entity->status)),
      '#formatter' => 'text_default',
      0 => array('#markup' => crm_core_case_status($entity->status))
    );

    // Add in fields.
    if (!empty($this->entityInfo['fieldable'])) {
      $entity->content += field_attach_view($this->entityType, $entity, $view_mode, $langcode);
    }

    // Invoke hook_ENTITY_view() to allow modules to add their additions.
    if (module_exists('rules')) {
      rules_invoke_all($this->entityType . '_view', $entity, $view_mode, $langcode);
    }
    else {
      module_invoke_all($this->entityType . '_view', $entity, $view_mode, $langcode);
    }
    module_invoke_all('entity_view', $entity, $this->entityType, $view_mode, $langcode);
    $build = $entity->content;
    unset($entity->content);
    return $build;
  }
}

/**
 * UI controller for CRM Case Type.
 */
class CRMCaseTypeUIController extends EntityDefaultUIController {
  /**
   * Overrides hook_menu() defaults.
   */
  public function hook_menu() {
    $items = parent::hook_menu();
    $items[$this->path]['description'] = 'Manage Case types.';
    return $items;
  }
}
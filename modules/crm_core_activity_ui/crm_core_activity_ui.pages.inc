<?php

/**
 * @file
 * User page callbacks for the Activity UI module.
 */

use Drupal\Component\Utility\Xss;
use Drupal\Core\Url;

/**
 * Prepares variables for list of available activity type templates.
 *
 * Default template: crm-core-activity-ui-add-list.html.twig.
 *
 * @param array $variables
 *   An associative array containing:
 *   - type_entities: An array of activity types.
 */
function template_preprocess_crm_core_activity_ui_add_list(&$variables) {
  $variables['types'] = array();
  if (!empty($variables['type_entities'])) {
    /* @var \Drupal\crm_core_activity\Entity\ActivityType $type */
    foreach ($variables['type_entities'] as $type) {
      if (!empty($variables['contact'])) {
        /* @var \Drupal\crm_core_contact\Entity\Contact $contact */
        $contact = $variables['contact'];
        $path = $contact->urlInfo()->getPath() . '/activity/add/' . $type->id();
      }
      else {
        $path = 'crm-core/contact/activity-add/' . $type->id();
      }
      $variables['types'][$type->id()] = array(
        'type' => $type->id(),
        // @todo Replace path with route.
        'add_link' => \Drupal::l(t('Add @type', array('@type' => $type->label())), Url::fromUri("base:$path")),
        'description' => Xss::filterAdmin($type->description),
      );
    }
  }
}

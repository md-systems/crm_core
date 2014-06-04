<?php

/**
 * @file
 * User page callbacks for the Activity UI module.
 */
use Drupal\Component\Utility\Xss;

/**
 * Submit handler for Delete button.
 *
 * Redirect to delete confirmation page.
 */
function crm_core_activity_form_submit_delete($form, &$form_state) {
  $destination = array();
  if (isset($_GET['destination'])) {
    $destination = drupal_get_destination();
    unset($_GET['destination']);
  }
  $activity = $form_state['crm_core_activity'];
  $form_state['redirect'] = array(
    'crm-core/activity/' . $activity->activity_id . '/delete',
    array('query' => $destination)
  );
}

function crm_core_activity_form_delete_confirm($form, &$form_state, $activity) {
  $form_state['activity'] = $activity;
  // Always provide entity id in the same form key as in the entity edit form.
  $form['activity_id'] = array(
    '#type' => 'value',
    '#value' => $activity->activity_id,
  );

  return confirm_form($form,
    t('Are you sure you want to delete activity %title?', array('%title' => $activity->title)),
    'crm-core/activity/' . $activity->activity_id,
    t('This action cannot be undone.'),
    t('Delete'),
    t('Cancel')
  );
}

/**
 * Execute node deletion.
 */
function crm_core_activity_form_delete_confirm_submit($form, &$form_state) {
  if ($form_state['values']['confirm']) {
    $activity = $form_state['activity'];
    crm_core_activity_delete($activity);

    watchdog('crm_core_activity', '@type: deleted %title.', array(
      '@type' => $activity->type,
      '%title' => $activity->title
    ));
    drupal_set_message(t('@type %title has been deleted.', array(
      '@type' => crm_core_activity_type_get_name($activity->type),
      '%title' => $activity->title,
    )));
  }
  $destination = drupal_get_destination();
  $current_path = drupal_get_path_alias();
  if ($destination['destination'] == $current_path) {
    drupal_goto('crm-core');
  }
  else {
    drupal_goto($destination);
  }
}

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
        'add_link' => l(t('Add @type', array('@type' => $type->label())), $path),
        'description' => Xss::filterAdmin($type->description),
      );
    }
  }
}

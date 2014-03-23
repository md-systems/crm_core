<?php

/**
 * List crm relationship types
 *
 * @param
 * bool $reverse
 * TRUE means display all relations that are not CRM relationships
 * FALSE means display all relations that are CRM relationships
 *
 * @return
 * $build
 * A renderable array of table
 */
function crm_core_relationship_ui_types_overview($reverse = FALSE) {
  drupal_set_title(t('CRM Relationship Types'));
  $reverse = (bool) $reverse;

  module_load_include('inc', 'crm_core_relationship_ui');

  $relation_types = relation_list_types();

  $field_ui = module_exists('field_ui');
  $headers = array(
    array('data' => t('Name'), 'width' => '40%'),
    array('data' => t('Operations'))
  );
  $rows = array();


  crm_core_ui_ctools_add_dropbutton_files();
  foreach ($relation_types as $type => $label) {
    $links = array();
    if ($reverse) {
      if (crm_core_relationship_is_relationship_type($type)) continue;
      $url = 'admin/structure/relation/manage/' . $type;
    }
    else {
      if (!crm_core_relationship_is_relationship_type($type)) continue;
      $url = 'admin/structure/crm-core/relationship-types/manage/' . $type;
    }
    $row = array(l($type, $url));
    $links[] = l(t('edit'), $url . '/edit');
    if ($field_ui) {
      $links[] =  l(t('manage fields'), $url . '/fields');
      $links[] =  l(t('display fields'), $url . '/display');
    }
    $links[] =  l(t('delete'), $url . '/delete');
    $row[] = theme('crm_core_ui_ctools_dropbutton', array(
        'ops_count' => count($links),
        'links' => $links,
        'index' => $type,
      ));
    $rows[] = $row;
  }

  $build['crm_relationship_types_table'] = array(
    '#theme' => 'table',
    '#header' => $headers,
    '#rows' => $rows,
  );

  return $build;
}

/**
 * Relation type bundle settings form.
 *
 * @param $relation_type
 *   Relation type machine name. If this is not provided, assume that we're
 *   creating a new relation type.
 *
 * This function is originally copied from relation_type_form from relation.admin.inc
 */
function crm_core_relationship_ui_type_form($form, &$form_state, $relation_type = array(), $op = 'edit') {

  (empty($relation_type)) ? drupal_set_title(t('Add New Relationship type')) : drupal_set_title(t('Edit Relationship Type'));

  _crm_core_relationship_ui_type_set_breadcrumb();

  $form['#write_record_keys'] = array();
  if ($relation_type) {
    $relation_type = (object) $relation_type;
    if ($relation_type) {
      $form['#write_record_keys'][] = 'relation_type';
    }
  }
  else {
    $relation_type = (object) array(
      'relation_type' => '',
      'label' => '',
      'reverse_label' => '',
      'bundles' => array(),
      'directional' => FALSE,
      'transitive' => FALSE,
      'r_unique' => FALSE,
      'min_arity' => 2,
      'max_arity' => 2,
      'source_bundles' => array(),
      'target_bundles' => array(),
    );
  }
  $form['name'] = array( // use 'name' for /misc/machine-name.js
    '#type'          => 'textfield',
    '#title'         => t('Label'),
    '#description'   => t('Display name of the relationship type. This is also used as the predicate in natural language formatters (ie. if A is related to B, you get "A [label] B")'),
    '#default_value' => $relation_type->label,
    '#required'      => TRUE,
  );
  $form['relation_type'] = array(
    '#type'          => 'machine_name',
    '#default_value' => $relation_type->relation_type,
    '#maxlength' => 32,
    '#disabled'      => $relation_type->relation_type,
    '#machine_name' => array(
      'exists' => 'relation_type_load',
    ),
  );
  $form['directional'] = array(
    '#type'           => 'checkbox',
    '#title'          => 'Directional',
    '#description'   => t('A directional relationship is one that does not imply the reverse relationship. For example, a "likes" relationship is directional (A likes B does not neccesarily mean B likes A), whereas a "similar to" relationship is non-directional (A similar to B implies B similar to A. Non-directional relationship are also known as symmetric relationship.'),
    '#default_value'  => $relation_type->directional,
  );
  $form['reverse_label'] = array(
    '#type'          => 'textfield',
    '#title'         => t('Reverse label'),
    '#description'   => t('Reverse label of the relation type. This is used as the predicate by formatters of directional relations, when you need to display the reverse direction (ie. from the target entity to the source entity). If this is not supplied, the forward label is used.'),
    '#default_value' => $relation_type->reverse_label,
    '#states' => array(
      'visible' => array(   // action to take.
        ':input[name="directional"]' => array('checked' => TRUE),
      ),
    ),
  );
  // override from original form
  $form['transitive'] = array(
    '#type'           => 'value',
    '#default_value'  => 0,
  );
  $form['r_unique'] = array(
    '#type'           => 'checkbox',
    '#title'          => t('Unique'),
    '#description'    => t('Whether relations of this type are unique (ie. they can not contain exactly the same end points as other relations of this type).'),
    '#default_value'  => $relation_type->r_unique,
  );
  // these should probably be changed to numerical (validated) textfields.
  $options = array('2' => '2', '3' => '3', '4' => '4', '5' => '5', '8' => '8');
  $form['min_arity'] = array(
    '#type' => 'value',
    '#default_value' => 2,
  );

  $options = array('2' => '2', '3' => '3', '4' => '4', '5' => '5', '8' => '8', '0' => t('Infinite'));
  $form['max_arity'] = array(
    '#type' => 'value',
    '#default_value' => 2,
  );
  $counter = 0;
  foreach (entity_get_info() as $entity_type => $entity) {
    $bundles[$entity['label']]["$entity_type:*"] = 'all ' . $entity['label'] . ' bundles';
    $counter += 2;
    if (isset($entity['bundles'])) {
      foreach ($entity['bundles'] as $bundle_id => $bundle) {
        $bundles[$entity['label']]["$entity_type:$bundle_id"] = $bundle['label'];
        $counter++;
      }
    }
  }

  $contact_types = crm_core_contact_types();

  $options = array('crm_core_contact:*' => t('Any'));
  foreach ($contact_types as $contact_type) {
    $options['crm_core_contact:' . $contact_type->type] = t($contact_type->name);
  }

  $form['source_bundles'] = array(
    '#type'          => 'select',
    '#title'         => 'Source contact types',
    '#options'       => $options,
    '#size'          => count($options),
    '#default_value' => $relation_type->source_bundles,
    '#multiple'      => TRUE,
    '#description'   => 'Contact types that are not selected will not be available as sources for directional, or end points of non-directional relations relations. Ctrl+click to select multiple. Note that selecting all bundles also include bundles not yet created for that entity type.',
  );
  $form['target_bundles'] = array(
    '#type'          => 'select',
    '#title'         => 'Destination contact types',
    '#options'       => $options,
    '#size'          => count($options),
    '#default_value' => $relation_type->target_bundles,
    '#multiple'      => TRUE,
    '#description'   => 'Contact types that are not selected will not be available as targets for directional relations. Ctrl+click to select multiple.',
    '#states' => array(
      '!visible' => array(   // action to take.
        ':input[name="directional"]' => array('checked' => FALSE),
      ),
    ),
  );
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save')
  );
  return $form;
}

/**
 * Submit data from relationship type page.
 *
 * This function is originally copied from relation_type_form_submit from relation.admin.inc
 */
function crm_core_relationship_ui_type_form_submit($form, &$form_state) {
  $relation_type = $form_state['values']['relation_type'];
  $min_arity = $form_state['values']['directional'] ? 2 : $form_state['values']['min_arity'];
  $max_arity = $form_state['values']['directional'] ? 2 : $form_state['values']['max_arity'];
  $record = array(
    'relation_type'   => $relation_type,
    'min_arity'   => $min_arity,
    'max_arity'   => $max_arity,
    'label' => $form_state['values']['name'],
    'reverse_label' => $form_state['values']['reverse_label'],
    'directional' => $form_state['values']['directional'],
    'transitive' => $form_state['values']['transitive'],
    'r_unique' => $form_state['values']['r_unique'],
    'source_bundles' => $form_state['values']['source_bundles'],
    'target_bundles' => $form_state['values']['target_bundles'],
  );
  relation_type_save($record, $form['#write_record_keys']);
  $form_state['redirect'] = 'admin/structure/crm-core/relationship-types';

  drupal_set_message(t('The relationship type has been saved.'));
}

/**
 * Menu callback; deletes a single relation type.
 *
 * This function is originally copied from relation_type_delete_confirm from relation.admin.inc
 */
function crm_core_relationship_ui_type_delete_confirm($form, &$form_state, $relation_type) {

  _crm_core_relationship_ui_type_set_breadcrumb();

  $form['relation_type'] = array('#type' => 'value', '#value' => $relation_type->relation_type);
  $form['label'] = array('#type' => 'value', '#value' => $relation_type->label);

  $message = t('Are you sure you want to delete the %label relationship type?', array('%label' => $relation_type->label));
  $caption = '';

  $num_relations = relation_query()
    ->propertyCondition('relation_type', $relation_type->relation_type)
    ->count()
    ->execute();
  if ($num_relations) {
    $caption .= '<p>' . format_plural($num_relations,
      'The %label relation type is used by 1 relation on your site. If you remove this relation type, you will not be able to edit  %label relations and they may not display correctly.',
      'The %label relation type is used by @count relations on your site. If you remove %label, you will not be able to edit %label relations and they may not display correctly.',
      array('%label' => $relation_type->label, '@count' => $num_relations)) . '</p>';
  }

  $caption .= '<p>' . t('This action cannot be undone.') . '</p>';

  return confirm_form($form, $message, 'admin/structure/crm-core/relationship-types', $caption, t('Delete'));
}

/**
 * Process relation type delete confirm submissions.
 *
 *  * This function is originally copied from relation_type_delete_confirm_submit from relation.admin.inc
 */
function crm_core_relationship_ui_type_delete_confirm_submit($form, &$form_state) {
  relation_type_delete($form_state['values']['relation_type']);

  $t_args = array('%label' => $form_state['values']['label']);
  drupal_set_message(t('The %label relationship type has been deleted.', $t_args));
  watchdog('relation', 'Deleted the %label relationship type.', $t_args, WATCHDOG_NOTICE);

  // TODO: relation_types_rebuild() ?;
  menu_rebuild();

  $form_state['redirect'] = 'admin/structure/crm-core/relationship-types';
  return;
}

/**
 * Helper function sets breadcrumb on forms
 */
function _crm_core_relationship_ui_type_set_breadcrumb() {
  $breadcrumb = array(
    l(t('Home'), '<front>'),
    l(t('Administration'), 'admin'),
    l(t('Structure'), 'admin/structure'),
    l(t('CRM Core'), 'admin/structure/crm-core'),
    l(t('Relationship types'), 'admin/structure/crm-core/relationship-types')
  );

  drupal_set_breadcrumb($breadcrumb);
}



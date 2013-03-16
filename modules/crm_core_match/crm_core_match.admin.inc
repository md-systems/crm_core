<?php

/**
 * @file
 * Administrative forms.
 */

/**
 * Matching Engines form.
 */
function crm_core_match_admin_config_engines_form($form, &$form_state) {
 
  $form['#tree'] = TRUE;

  $form['header'] = array(
    '#prefix' => '<h2>',
    '#markup' => t('Matching Engines'),
    '#suffix' => '</h2>',
  );

  $form['description_wrapper'] = array(
    '#type' => 'container',
  );
  $description = 'Configure matching engines for contacts. Matching engines are used when new contacts are created,'
    . ' allowing CRM Core to identify potential duplicates and prevent additional records from being added to the'
    . ' system. Use this screen to activate / deactivate various matching engines and control the order in which they'
    .  ' are applied.';
  $form['description_wrapper']['description'] = array(
    '#markup' => t($description),
  );

  $form['engines_container'] = array(
    '#type' => 'container'
  );

  $engines = _crm_core_match_get_engines();
  
  foreach ($engines as $index => $engine) {
    $form['engines_container'][$engine->name]['name'] = array('#markup' => $engine->name);
    $form['engines_container'][$engine->name]['desc'] = array('#markup' => $engine->description);
    $form['engines_container'][$engine->name]['enabled'] = array(
      '#markup' => l('Disable', '<front>'),
    );
    // Operations links.
    $links = array();
    foreach ($engine->settings as $item) {
      $links[$item['name']] = array(
        'title' => $item['label'],
        'href' => $item['path'],
      );
    }
    $form['engines_container'][$engine->name]['settings'] = array(
      '#theme' => 'links',
      '#links' => $links,
      '#attributes' => array('class' => array('links', 'inline')),
    );
    $form['engines_container'][$engine->name]['weight'] = array(
      '#type' => 'weight',
      '#title_display' => 'invisible',
      '#default_value' => $index,
      '#attributes' => array(
        'class' => array('crm-core-match-engine-order-weight'),
      ),
    );
  }

  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
  );

  return $form;
}


/**
 * Returns a list of all matching engines registered with CRM Core Match.
 * 
 * This should return a list of matching engines registered with CRM Core 
 * Match, in order. Any matching engines added since the last time the list
 * of matching engines was changed should appear at the bottom.
 * 
 * @TODO: should this actually be sorted? Is it possible this should just
 *   be a list of the engines?
 *   
 * @TODO: should this be keyed by machine name?
 * 
 * @TODO: add caching.
 * 
 * @return array $engines: a list of engines for CRM Core
 */
function _crm_core_match_get_engines(){
  $engines = array();
  foreach (module_implements('crm_core_match_engine_register') as $module) {
    $function = $module . '_crm_core_match_engine_register';
    $engines[] = $function();
  }
  return $engines;
}



/**
 * Theme implementation of the crm_core_match_admin_config_engines_form form.
 */
function theme_crm_core_match_admin_config_engines_form($variables) {
  $form = $variables['form'];

  $rows = array();
  foreach (element_children($form['engines_container']) as $key) {
    $rows[] = array(
      'data' => array(
        drupal_render($form['engines_container'][$key]['name']),
        drupal_render($form['engines_container'][$key]['desc']),
        drupal_render($form['engines_container'][$key]['enabled']),
        drupal_render($form['engines_container'][$key]['settings']),
        drupal_render($form['engines_container'][$key]['weight']),
      ),
      'class' => array('draggable'),
    );
  }

  $header = array(t('Name'), t('Description'), t('Enabled'), t('Operations'), t('Weight'));
  $form['engines_container']['content']['#markup'] = theme('table', array(
    'header' => $header,
    'rows' => $rows,
    'attributes' => array('id' => 'crm-core-match-engine-order')
  ));
  $output = drupal_render_children($form);

  drupal_add_tabledrag('crm-core-match-engine-order', 'order', 'sibling', 'crm-core-match-engine-order-weight');

  return $output;
}

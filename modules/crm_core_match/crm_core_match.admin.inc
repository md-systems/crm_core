<?php

/**
 * @file
 * Administrative forms.
 */

/**
 * Matching Engines form.
 */
function crm_core_match_admin_config_engines_form($form, &$form_state) {

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
    . ' are applied.';
  $form['description_wrapper']['description'] = array(
    '#markup' => t($description),
  );

  $form['engines_container'] = array(
    '#type' => 'container',
    '#tree' => TRUE,
  );

  $engines = crm_core_match_get_engines();
  $form_state['engines'] = $engines;
  foreach ($engines as $engine) {
    $form['engines_container'][$engine->getMachineName()]['name'] = array('#markup' => $engine->getName());
    $form['engines_container'][$engine->getMachineName()]['description'] = array('#markup' => $engine->getDescription());
    $status_toggle_path = 'admin/config/crm-core/match/engines/' . $engine->getMachineName() . '/';
    $status_toggle_path .= $engine->getStatus() ? 'disable' : 'enable';
    $destination = drupal_get_destination();
    $form['engines_container'][$engine->getMachineName()]['enabled'] = array(
      '#markup' => l($engine->getStatus() ? t('Disable') : t('Enable'), $status_toggle_path, array(
        'query' => array(
          'destination' => $destination['destination'],
        ),
      )),
    );
    // Operations links.
    $links = array();
    foreach ($engine->getSettings() as $item) {
      $links[$item['name']] = array(
        'title' => $item['label'],
        'href' => $item['path'],
      );
    }
    $form['engines_container'][$engine->getMachineName()]['settings'] = array(
      '#theme' => 'links',
      '#links' => $links,
      '#attributes' => array('class' => array('links', 'inline')),
    );
    $form['engines_container'][$engine->getMachineName()]['weight'] = array(
      '#type' => 'weight',
      '#title_display' => 'invisible',
      '#default_value' => $engine->getWeight(),
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
 * Matching Engines form submit handler.
 */
function crm_core_match_admin_config_engines_form_submit($form, &$form_state) {
  $engines = $form_state['engines'];
  foreach ($engines as $engine) {
    db_merge('crm_core_match_engines')
      ->condition('machine_name', $engine->getMachineName())
      ->fields(array(
        'weight' => $form_state['values']['engines_container'][$engine->getMachineName()]['weight'],
        'machine_name' => $engine->getMachineName(),
      ))
      ->execute();
  }
  drupal_set_message(t('The configuration options have been saved.'));
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
        drupal_render($form['engines_container'][$key]['description']),
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
    'attributes' => array('id' => 'crm-core-match-engine-order'),
  ));
  $output = drupal_render_children($form);

  drupal_add_tabledrag('crm-core-match-engine-order', 'order', 'sibling', 'crm-core-match-engine-order-weight');

  return $output;
}

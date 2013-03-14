<?php

/**
 * Page callback for CRM Administration dashboard.
 */
function crm_core_ui_admin_dashboard() {
  $local_tasks = menu_local_tasks();
  $content = array();
  foreach ($local_tasks['tabs']['output'] as $task) {
    $content[] = $task['#link'];
  }

  if ($content) {
    $output = theme('admin_block_content', array('content' => $content));
  }
  else {
    $output = t('You do not have any CRM Core items.');
  }
  return $output;
}


/**
 * Form constructor for the CRM Core configuration form.
 */
function crm_core_ui_admin_config_form($form, &$form_state) {
  $form = array();
  $form['dashboard'] = array(
    '#type' => 'fieldset',
    '#title' => t('Dashboard'),
    '#weight' => -1,
  );
  $dashboard_enable_description = 'When checked, this will direct users to the specified dashboard when accessing '
    . 'the main link to CRM Core. When unchecked, it will direct users to the main list of contacts in CRM Core. '
    . 'Useful in situations where important information about contacts needs to be available at a glance. Use panels,'
    . ' homebox or some other tool to create the dashboard, and include the path to the dashboard in the box below.';
  $form['dashboard']['crm_core_dashboard_enabled'] = array(
    '#type' => 'checkbox',
    '#title' => t('Use a Dashboard for CRM Core?'),
    '#description' => t($dashboard_enable_description),
    '#default_value' => variable_get('crm_core_dashboard_enabled', FALSE),
    '#weight' => 0,
  );
  $form['dashboard']['crm_core_dashboard_path'] = array(
    '#type' => 'textfield',
    '#title' => t('Path to Dashboard'),
    '#default_value' => variable_get('crm_core_dashboard_path', ''),
    '#weight' => 1,
    '#states' => array(
      'visible' => array(
        ':input[name="crm_core_dashboard_enabled"]' => array('checked' => TRUE),
      ),
      'required' => array(
        ':input[name="crm_core_dashboard_enabled"]' => array('checked' => TRUE),
      ),
    ),
  );
  $form['theme'] = array(
    '#type' => 'fieldset',
    '#title' => t('CRM Core Theme'),
    '#weight' => 1,
  );
  $form['theme']['crm_core_theme_enabled'] = array(
    '#type' => 'checkbox',
    '#title' => t('Use a different theme for CRM Core?'),
    '#description' => t('When checked, all pages under the crm-core path will be displayed using a specified theme.'),
    '#default_value' => variable_get('crm_core_theme_enabled', FALSE),
    '#weight' => 0,
  );
  $form['theme']['crm_core_theme_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('CRM Core Theme settings'),
    '#weight' => 1,
    '#states' => array(
      'visible' => array(
        ':input[name="crm_core_theme_enabled"]' => array('checked' => TRUE),
      ),
    ),
  );
  // Prepare options for theme select element.
  $themes_list = list_themes();
  $options = array();
  foreach ($themes_list as $name => $theme_description) {
    $options[$name] = $theme_description->info['name'];
  }
  $form['theme']['crm_core_theme_settings']['crm_core_theme'] = array(
    '#type' => 'select',
    '#title' => t('Select theme to use'),
    '#default_value' => variable_get('crm_core_theme', ''),
    '#options' => $options,
    '#weight' => 0,
    '#empty_option' => t('- Please Select -'),
    '#empty_value' => '',
    '#states' => array(
      'required' => array(
        ':input[name="crm_core_theme_enabled"]' => array('checked' => TRUE),
      ),
    ),
  );
  $links_description = 'When checked, this allows you to use the CRM Core menu as the primary links for the site,' .
    ' when viewing a page in CRM Core.';
  $form['theme']['crm_core_theme_settings']['crm_core_theme_replace_links'] = array(
    '#type' => 'checkbox',
    '#title' => t('Replace primary links with the CRM Core menu?'),
    '#description' => t($links_description),
    '#default_value' => variable_get('crm_core_theme_replace_links', FALSE),
    '#weight' => 1,
  );
  $form['theme']['crm_core_theme_settings']['notes_container'] = array(
    '#type' => 'container',
    '#weight' => 2,
  );
  $form['theme']['crm_core_theme_settings']['notes_container']['notes'] = array(
    '#type' => 'item',
    '#markup' => t('Additional settings can be configured within the !theme_settings page.', array(
      '!theme_settings' => l(t('theme settings'), 'admin/appearance/settings'),
    )),
  );
  $form['#submit'][] = 'crm_core_ui_admin_config_form_submit';
  return system_settings_form($form);
}

/**
 * Validate CRM Core settings.
 */
function crm_core_ui_admin_config_form_validate($form, &$form_state) {
  // Dashboard path validation.
  $dashboard_enabled = $form_state['values']['crm_core_dashboard_enabled'];
  if ($dashboard_enabled) {
    $menu_item = menu_get_item($form_state['values']['crm_core_dashboard_path']);
    if (!$menu_item) {
      form_set_error('crm_core_dashboard_path', t('You must enter VALID path(it must exist already).'));
    }
  }
  // Theme settings validation.
  $theme_enabled = $form_state['values']['crm_core_theme_enabled'];
  $theme = $form_state['values']['crm_core_theme'];
  if ($theme_enabled && empty($theme)) {
    form_set_error('crm_core_theme', t('You must select a theme to use.'));
  }
}

/**
 * Submit handler for CRM Core settings form.
 */
function crm_core_ui_admin_config_form_submit($form, &$form_state) {
  $dashboard_enabled_current = $form_state['values']['crm_core_dashboard_enabled'];
  $dashboard_enabled_saved = variable_get('crm_core_dashboard_enabled', FALSE);
  if ($dashboard_enabled_current && !$dashboard_enabled_saved) {
    $item = array(
      'link_path' => 'crm-core/dashboard',
      'link_title' => 'Dashboard',
      'menu_name' => 'crm-core-menu',
      'weight' => 0,
    );
    menu_link_save($item);
    variable_set('crm_core_dashboard_enabled', TRUE);
  }
  elseif (!$dashboard_enabled_current) {
    $links = menu_load_links('crm-core-menu');
    foreach ($links as $link) {
      if ($link['link_path'] == 'crm-core/dashboard') {
        menu_link_delete($link['mlid']);
      }
    }
    variable_set('crm_core_dashboard_enabled', FALSE);
  }
  menu_rebuild();
}

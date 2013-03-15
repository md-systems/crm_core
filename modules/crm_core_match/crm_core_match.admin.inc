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

  return $form;
}

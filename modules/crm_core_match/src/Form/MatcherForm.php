<?php
/**
 * @file
 * Contains \Drupal\crm_core_match\Form\MatcherForm.
 */

namespace Drupal\crm_core_match\Form;

use Drupal\Core\Entity\EntityForm;
use Drupal\Core\Form\FormStateInterface;

/**
 * Provides a form elements for Matcher.
 */
class MatcherForm extends EntityForm {

  /**
   * The entity being used by this form.
   *
   * @var \Drupal\crm_core_match\Matcher\MatcherConfigInterface
   */
  protected $entity;

  /**
   * {@inheritdoc}
   */
  public function form(array $form, FormStateInterface $form_state) {
    $form = parent::form($form, $form_state);

    $matcher = $this->entity;

    $form['label'] = array(
      '#type' => 'textfield',
      '#title' => $this->t('Label'),
      '#maxlength' => 255,
      '#default_value' => $matcher->label(),
      '#required' => TRUE,
    );

    $form['id'] = array(
      '#type' => 'machine_name',
      '#title' => $this->t('ID'),
      '#maxlength' => 255,
      '#default_value' => $matcher->id(),
      '#description' => $this->t('ID of the matcher.'),
      '#required' => TRUE,
      '#machine_name' => array(
        'exists' => 'Drupal\crm_core_match\Entity\Matcher::load',
      ),
      '#disabled' => !$matcher->isNew(),
    );

    $form['description'] = array(
      '#type' => 'textfield',
      '#title' => $this->t('Description'),
      '#default_value' => $matcher->getDescription(),
      '#maxlength' => 255,
      '#description' => $this->t('Description of the matcher.')
    );

    // Get all plugins.
    if ($matcher->isNew()) {
      $plugin_types = array();
      foreach (crm_core_match_matcher_manager()->getDefinitions() as $plugin_id => $definition) {
        $plugin_types[$plugin_id] = $definition['label'];
      }

      // If there is only one plugin (matching engine) available, set it as
      // default option and hide the select menu.
      $default_value = NULL;
      $single_plugin = count($plugin_types) == 1;
      if ($single_plugin) {
        $default_value = key($plugin_types);
        $matcher->set('plugin_id', $default_value);
      }
      $form['plugin_id'] = array(
        '#type' => 'select',
        '#default_value' => $default_value,
        '#options' => $plugin_types,
        '#empty_value' => $this->t('- Select -'),
        '#title' => $this->t('Matcher Plugin'),
        '#limit_validation_errors' => array(array('plugin_id')),
        '#submit' => array('::submitSelectPlugin'),
        '#required' => TRUE,
        '#executes_submit_callback' => TRUE,
        '#ajax' => array(
          'callback' => '::ajaxReplacePluginSpecificForm',
          'wrapper' => 'crm-core-match-match-engine-plugin',
          'method' => 'replace',
        ),
        '#access' => !$single_plugin,
      );
      $form['select_plugin'] = array(
        '#type' => 'submit',
        '#value' => $this->t('Select plugin'),
        '#limit_validation_errors' => array(array('plugin_id')),
        '#submit' => array('::submitSelectPlugin'),
        '#attributes' => array('class' => array('js-hide')),
        '#access' => !$single_plugin,
      );
    }
    else {
      $form['current_plugin_id'] = array(
        '#type' => 'item',
        '#title' => $this->t('Match engine'),
        '#markup' => $matcher->getPlugin()->getPluginDefinition()['label'],
      );
    }

    $form['plugin_container'] = array(
      '#type' => 'container',
      '#prefix' => '<div id="crm-core-match-match-engine-plugin">',
      '#suffix' => '</div>',
    );

    if ($plugin = $matcher->getPlugin()) {
      $form['plugin_container']['configuration'] = array(
        '#type' => 'details',
        '#open' => TRUE,
        '#title' => $this->t('@plugin configuration', ['@plugin' => $matcher->getPluginTitle()]),
        '#tree' => TRUE,
      );
      $form['plugin_container']['configuration'] += (array) $plugin->buildConfigurationForm($form['plugin_container']['configuration'], $form_state);
    }

    return $form;
  }

  /**
   * Handles submit call when sensor type is selected.
   */
  public function submitSelectPlugin(array $form, FormStateInterface $form_state) {
    $this->entity = $this->buildEntity($form, $form_state);
    $form_state->setRebuild();
  }

  /**
   * Handles switching the settings type selector.
   */
  public function ajaxReplacePluginSpecificForm($form, FormStateInterface $form_state) {
    return $form['plugin_container'];
  }

  /**
   * {@inheritdoc}
   */
  public function validateForm(array &$form, FormStateInterface $form_state) {
    parent::validateForm($form, $form_state);

    /** @var \Drupal\crm_core_match\Matcher\MatcherConfigInterface $matcher */
    $matcher = $this->entity;
    /** @var \Drupal\crm_core_match\Plugin\crm_core_match\engine\MatchEngineInterface $plugin */
    if ($matcher->isNew()) {
      $plugin_id = $form_state->getValue('plugin_id');
      $plugin = crm_core_match_matcher_manager()->createInstance($plugin_id, $matcher->getConfiguration());
    }
    else {
      $plugin = $matcher->getPlugin();
    }

    $plugin->validateConfigurationForm($form, $form_state);
  }

  /**
   * {@inheritdoc}
   */
  public function submitForm(array &$form, FormStateInterface $form_state) {
    parent::submitForm($form, $form_state);

    /** @var \Drupal\crm_core_match\Entity\Matcher $matcher */
    $matcher = $this->entity;
    $plugin = $matcher->getPlugin();
    $plugin->submitConfigurationForm($form, $form_state);

    drupal_set_message($this->t('The configuration has been saved.'));
    $form_state->setRedirect('entity.crm_core_match.edit_form', ['crm_core_match' => $matcher->id()]);
  }

}

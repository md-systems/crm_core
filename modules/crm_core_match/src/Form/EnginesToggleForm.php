<?php

/**
 * @file
 * Contains \Drupal\crm_core_match\Form\EnginesToggleForm.
 */

namespace Drupal\crm_core_match\Form;

use Drupal\Component\Plugin\PluginManagerInterface;
use Drupal\Core\Form\ConfirmFormBase;
use Drupal\Core\Url;
use Symfony\Component\DependencyInjection\ContainerInterface;

class EnginesToggleForm extends ConfirmFormBase {

  /**
   * The engine plugin manager.
   *
   * @var \Drupal\Component\Plugin\PluginManagerInterface
   */
  protected $pluginManager;

  /**
   * The engine definition.
   *
   * @var array
   */
  protected $engine;

  /**
   * The operation to execute.
   *
   * One of 'enabled' or 'disable'.
   *
   * @var string
   */
  protected $op;

  /**
   * Constructs an engine toggle form.
   *
   * @param \Drupal\Component\Plugin\PluginManagerInterface $plugin_manager
   *   The engine plugin manager.
   */
  public function __construct(PluginManagerInterface $plugin_manager) {
    $this->pluginManager = $plugin_manager;
  }

  /**
   * {@inheritdoc}
   */
  public static function create(ContainerInterface $container) {
    return new static(
      $container->get('plugin.manager.crm_core_match.engine')
    );
  }

  /**
   * {@inheritdoc}
   */
  public function buildForm(array $form, array &$form_state, $engine = NULL, $op = NULL) {
    // @todo Consider writing a converter.
    // The converter will ensure that the engine id will be converted to the
    // definition before it gets passed to this method.
    $this->engine = $this->pluginManager->getDefinition($engine);
    $this->op = $op;

    return parent::buildForm($form, $form_state);
  }

  /**
   * {@inheritdoc}
   */
  public function getQuestion() {
    return $this->t('Are you sure you want to %toggle the %engine engine?', array(
      '%toggle' => $this->getRequest()->get('op'),
      '%engine' => $this->engine['title'],
    ));
  }

  /**
   * {@inheritdoc}
   */
  public function getDescription() {
    return $this->t('Disabled engines will be ignored when fetching matches.');
  }

  /**
   * {@inheritdoc}
   */
  public function getConfirmText() {
    switch ($this->getRequest()->get('op')) {
      case 'disable':
        $text = $this->t('Disable');
        break;

      default:
      case 'enable':
        $text = $this->t('Enable');
        break;
    }

    return $text;
  }

  /**
   * {@inheritdoc}
   */
  public function getCancelUrl() {
    return new Url('crm_core_match.engines');
  }

  /**
   * {@inheritdoc}
   */
  public function submitForm(array &$form, array &$form_state) {
    $config = $this->config('crm_core_match.engines');
    $config_key = $this->engine['id'] . '.status';

    switch ($this->getRequest()->get('op')) {

      case 'disable':
        $action = $this->t('disabled');
        $config->set($config_key, FALSE);
        break;

      default:
      case 'enable':
        $action = $this->t('enabled');
        $config->set($config_key, TRUE);
        break;
    }

    $config->save();

    $t_args = array(
      '%name' => $this->engine['title'],
      '%toggle' => $action,
    );
    drupal_set_message($this->t('The contact type %name has been %toggle.', $t_args));

    $form_state['redirect_route']['route_name'] = 'crm_core_match.engines';
  }

  /**
   * {@inheritdoc}
   */
  public function getFormId() {
    return 'crm_core_match_engine_toggle';
  }
}

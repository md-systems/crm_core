<?php
/**
 * @file
 * Contains \Drupal\crm_core_match\Form\EnginesConfigForm.
 */

namespace Drupal\crm_core_match\Form;

use Drupal\Component\Plugin\PluginManagerInterface;
use Drupal\Core\Form\FormBase;
use Drupal\Core\Form\FormStateInterface;
use Symfony\Component\DependencyInjection\ContainerInterface;

class EnginesConfigForm extends FormBase {

  /**
   * The engine plugin manager.
   *
   * @var \Drupal\Component\Plugin\PluginManagerInterface
   */
  protected $pluginManager;

  /**
   * The match engines definitions.
   *
   * @var array|\mixed[]|null
   */
  protected $definitions;

  /**
   * Constructs an engine config form.
   *
   * @param \Drupal\Component\Plugin\PluginManagerInterface $plugin_manager
   *   An instance of an engine plugin manager.
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
  public function getFormId() {
    return 'crm_core_match_engines';
  }

  /**
   * {@inheritdoc}
   */
  public function buildForm(array $form, FormStateInterface $form_state) {
    $description = $this->t(<<<EOF
Configure matching engines for contacts. Matching engines are used when new
contacts are created, allowing CRM Core to identify potential duplicates and
prevent additional records from being added to the system. Use this screen to
activate / deactivate various matching engines and control the order in which
they are applied.
EOF
    );
    $form['header']['description'] = array(
      '#markup' => $description,
    );
    $form[$this->getFormId()] = array(
      '#type' => 'table',
      '#header' => $this->buildHeader(),
      '#empty' => $this->t('There is no matching engine available.'),
      '#tabledrag' => array(
        array(
          'action' => 'order',
          'relationship' => 'sibling',
          'group' => 'weight',
        ),
      ),
    );

    $this->definitions = $this->load();
    foreach ($this->definitions as $definition) {
      $row = $this->buildRow($definition);
      if (isset($row['label'])) {
        $row['label'] = array('#markup' => $row['label']);
      }
      $form[$this->getFormId()][$definition['id']] = $row;
    }

    $form['actions']['#type'] = 'actions';
    $form['actions']['submit'] = array(
      '#type' => 'submit',
      '#value' => $this->t('Save order'),
      '#button_type' => 'primary',
    );

    return $form;
  }

  /**
   * {@inheritdoc}
   */
  public function submitForm(array &$form, FormStateInterface $form_state) {
    $weights = $form_state->getValue($this->getFormId());
    // @todo Save engine priorities.
  }

  /**
   * Loads the matching engine definitions.
   *
   * @return array|\mixed[]|null
   *   The engine definitions.
   */
  protected function load() {
    return $this->pluginManager->getDefinitions();
  }

  /**
   * Builds the header row for the engine listing.
   *
   * @return array
   *   A render array structure of header strings.
   */
  public function buildHeader() {
    $header = array();

    $header['label'] = $this->t('Name');
    $header['description'] = array(
      'data' => $this->t('Description'),
      'class' => array(RESPONSIVE_PRIORITY_MEDIUM),
    );
    $header['weight'] = $this->t('Weight');
    $header['operations'] = $this->t('Operations');

    return $header;
  }


  /**
   * Builds a row for an entity in the entity listing.
   *
   * @param array $definition
   *   The engine definition for this row of the list.
   *
   * @return array
   *   A render array structure of fields for this entity.
   */
  public function buildRow(array $definition) {
    $row = array();

    $row['label'] = $definition['title'];

    $row['detail']['#markup'] = $definition['description'];

    $row['#attributes']['class'][] = 'draggable';
    $row['#weight'] = $definition['priority'];

    $row['weight'] = array(
      '#type' => 'weight',
      '#title' => $this->t('Weight for @title', array('@title' => $definition['title'])),
      '#title_display' => 'invisible',
      '#default_value' => $definition['priority'],
      '#attributes' => array('class' => array('weight')),
    );

    $operations = array();

    foreach ($definition['settings'] as $key => $data) {
      $operations[$key] = array(
        'title' => $data['label'],
        'route_name' => $data['route'],
      );
    }

    $status = $this->config('crm_core_match.engines')->get($definition['id'] . '.status');
    if (!$status) {
      $operations['enable'] = array(
        'title' => $this->t('Enable'),
        'route_name' => 'crm_core_match.enable',
        'route_parameters' => array(
          'engine' => $definition['id'],
        ),
      );
    }
    else {
      $operations['disable'] = array(
        'title' => $this->t('Disable'),
        'route_name' => 'crm_core_match.disable',
        'route_parameters' => array(
          'engine' => $definition['id'],
        ),
      );
    }

    $row['operations']['data'] = array(
      '#type' => 'operations',
      '#links' => $operations,
    );
    return $row;
  }
}

<?php

/**
 * @file
 * Contains \Drupal\crm_core_default_matching_engine\Plugin\crm_core_match\engine\DefaultMatchingEngine.
 */

namespace Drupal\crm_core_default_matching_engine\Plugin\crm_core_match\engine;

use Drupal\Component\Plugin\PluginManagerInterface;
use Drupal\Core\Entity\EntityManagerInterface;
use Drupal\crm_core_contact\ContactInterface;
use Drupal\crm_core_contact\Entity\Contact;
use Drupal\crm_core_match\Plugin\crm_core_match\engine\MatchEngineBase;
use Symfony\Component\DependencyInjection\ContainerInterface;

/**
 * DefaultMatchingEngine class
 *
 * Extends CrmCoreMatchEngine to provide rules for identifying duplicate
 * contacts.
 *
 * @CrmCoreMatchEngine(
 *   id = "default",
 *   title = @Translation("Default Matching Engine"),
 *   description = @Translation("This is a simple matching engine from CRM Core. Allows administrators to specify matching rules for individual contact types on a field-by-field basis."),
 *   priority = 0,
 *   settings = {
 *     "settings" = {
 *       "route" = "crm_core_default_matching_engine.config",
 *       "label" = @Translation("Configuration")
 *     }
 *   }
 * )
 */
class DefaultMatchingEngine extends MatchEngineBase {

  const MATCH_CHARS_DEFAULT = 3;

  /**
   * The match field plugin manager.
   *
   * @var \Drupal\Component\Plugin\PluginManagerInterface
   */
  protected $pluginManager;

  /**
   * The entity manager service.
   *
   * @var \Drupal\Core\Entity\EntityManagerInterface
   */
  protected $entityManager;

  /**
   * Constructs a default matching engine.
   */
  public function __construct($configuration, $id, $definition, PluginManagerInterface $plugin_manager, EntityManagerInterface $entity_manager) {
    parent::__construct($configuration, $id, $definition);
    $this->pluginManager = $plugin_manager;
    $this->entityManager = $entity_manager;
  }

  /**
   * {@inheritdoc}
   */
  public static function create(ContainerInterface $container, array $configuration, $plugin_id, $plugin_definition) {
    return new static(
      $configuration,
      $plugin_id,
      $plugin_definition,
      $container->get('plugin.manager.crm_core_match.match_field'),
      $container->get('entity.manager')
    );
  }

  /**
   * {@inheritdoc}
   */
  public function match(ContactInterface $contact) {
    $ids = array();
    /* @var \Drupal\crm_core_default_matching_engine\Entity\MatchingRule $matching_rule */
    $matching_rule = $this->entityManager->getStorage('crm_core_default_engine_rule')->load($contact->bundle());
    // Check if match is enabled for this contact type.
    if ($matching_rule->status()) {
      $fields = $contact->getFieldDefinitions();

      $results = array();
      foreach ($matching_rule->rules as $name => $rules) {
        if (isset($fields[$name])) {
          $rules['field'] = $fields[$name];

          if (!$this->pluginManager->hasDefinition($rules['field']->getType())) {
            continue;
          }

          /* @var \Drupal\crm_core_default_matching_engine\Plugin\crm_core_match\field\FieldHandlerInterface $field_handler */
          $field_handler = $this->pluginManager->createInstance($rules['field']->getType(), $rules);

          foreach ($field_handler->getPropertyNames() as $name) {
            $results += $field_handler->match($contact, $name);
          }
        }
      }
      foreach ($results as $id => $rule_matches) {
        $total_score = array_sum($rule_matches);
        if ($total_score >= $matching_rule->threshold) {
          $ids[] = $id;
        }
      }
    }
    return $ids;
  }
}

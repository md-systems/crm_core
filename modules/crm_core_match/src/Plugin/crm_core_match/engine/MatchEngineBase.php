<?php

/**
 * @file
 * Contains \Drupal\crm_core_match\Plugin\crm_core_match\engine\MatchEngineBase.
 */

namespace Drupal\crm_core_match\Plugin\crm_core_match\engine;

use Drupal\Core\Plugin\ContainerFactoryPluginInterface;
use Drupal\crm_core_contact\ContactInterface;
use Drupal\crm_core_contact\Entity\Contact;
use Symfony\Component\DependencyInjection\ContainerInterface;

/**
 * Default implementation of MatchEngineInterface
 *
 * Safe for use by most matching engines.
 */
abstract class MatchEngineBase implements MatchEngineInterface, ContainerFactoryPluginInterface {

  /**
   * The engine id.
   *
   * @var string
   */
  protected $id;

  /**
   * The engine definition.
   *
   * @var array
   */
  protected $definition;

  /**
   * The engine configuration.
   *
   * @var array
   */
  protected $configuration;

  /**
   * Constructs an plugin instance.
   */
  public function __construct($configuration, $id, $definition) {
    $this->configuration = $configuration;
    $this->definition = $definition;
    $this->id = $id;
  }

  /**
   * {@inheritdoc}
   */
  public static function create(ContainerInterface $container, array $configuration, $plugin_id, $plugin_definition) {
    return new static(
      $configuration,
      $plugin_id,
      $plugin_definition
    );
  }

  /**
   * Applies logical rules for identifying matches in the database.
   *
   * Any matching engine should implement this to apply it's unique matching
   * logic.
   *
   * @see MatchEngineInterface::match()
   */
  public abstract function match(ContactInterface $contact);
}

<?php
/**
 * @file
 * Contains \Drupal\crm_core_match\Entity\Matcher.
 */

namespace Drupal\crm_core_match\Entity;

use Drupal\Core\Config\Entity\ConfigEntityBase;
use Drupal\Core\Entity\EntityStorageInterface;
use Drupal\crm_core_contact\ContactInterface;
use Drupal\crm_core_match\Matcher\MatcherConfigInterface;

/**
 * CRM Matcher Entity Class.
 *
 * @ConfigEntityType(
 *   id = "crm_core_match",
 *   label = @Translation("Matcher"),
 *   config_prefix = "matcher",
 *   handlers = {
 *     "list_builder" = "Drupal\crm_core_match\Matcher\MatcherListBuilder",
 *     "form" = {
 *       "add" = "Drupal\crm_core_match\Form\MatcherForm",
 *       "edit" = "Drupal\crm_core_match\Form\MatcherForm",
 *       "delete" = "\Drupal\Core\Entity\EntityDeleteForm"
 *     },
 *   },
 *   admin_permission = "administer matchers",
 *   entity_keys = {
 *     "id" = "id",
 *     "label" = "label",
 *   },
 *   config_export = {
 *     "id",
 *     "label",
 *     "description",
 *     "plugin_id",
 *     "configuration",
 *   },
 *   links = {
 *     "collection" = "/admin/config/crm-core/match",
 *     "canonical" = "/admin/config/crm-core/match/{crm_core_match}",
 *     "add-form" = "/admin/config/crm-core/match/add",
 *     "edit-form" = "/admin/config/crm-core/match/{crm_core_match}",
 *     "delete-form" = "/admin/config/crm-core/match/{crm_core_match}/delete",
 *   }
 * )
 */
class Matcher extends ConfigEntityBase implements MatcherConfigInterface {

  /**
   * Primary identifier.
   *
   * Matches the id of a contact type.
   *
   * @var string
   */
  protected $id;

  /**
   * A brief description of this matcher.
   *
   * @var string
   */
  protected $description;


  /**
   * The plugin id.
   *
   * @var string
   */
  protected $plugin_id;

  /**
   * The plugin instance.
   *
   * @var \Drupal\crm_core_match\Plugin\crm_core_match\engine\MatchEngineInterface
   */
  protected $plugin;

  /**
   * The matcher plugin configuration.
   *
   * @var array
   */
  protected $configuration = array();

  /**
   * {@inheritdoc}
   */
  public function getConfiguration() {
    return $this->configuration;
  }

  /**
   * {@inheritdoc}
   */
  public function getDescription() {
    return $this->description;
  }

  /**
   * {@inheritdoc}
   */
  public function getPluginTitle() {
    return $this->getPlugin()->getPluginDefinition()['label'];
  }

  /**
   * {@inheritdoc}
   */
  public function getPlugin() {
    if (empty($this->plugin)) {
      $this->plugin = crm_core_match_matcher_manager()->createInstance($this->plugin_id, $this->configuration);
    }

    return $this->plugin;
  }

  /**
   * Finds matches for given contact.
   *
   * @param \Drupal\crm_core_contact\ContactInterface $contact
   *   A contact entity used to pass data for identifying a match.
   *
   * @return int[]
   *   An array of entity ids for potential matches.
   */
  public function match(ContactInterface $contact) {
    return $this->getPlugin()->match($contact);
  }

  /**
   * {@inheritdoc}
   */
  public function preSave(EntityStorageInterface $storage) {
    parent::preSave($storage);

    // @todo: Remove after implementing EntityWithPluginCollectionInterface.
    $this->set('configuration', $this->getPlugin()->getConfiguration());
  }

}

<?php
/**
 * @file
 * Contains \Drupal\crm_core_match\Entity\Matcher.
 */

namespace Drupal\crm_core_match\Entity;

use Drupal\Core\Config\Entity\ConfigEntityBase;
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
 *       "default" = "Drupal\crm_core_match\Form\MatcherForm",
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
  public $id;

  /**
   * The plugin id.
   *
   * @var string
   */
  public $plugin_id;

  /**
   * The entity label.
   *
   * @var string
   */
  protected $label;

  /**
   * The matcher plugin configuration.
   *
   * @var array
   */
  public $configuration = array();

  /**
   * {@inheritdoc}
   */
  public function label() {
    return parent::label();
  }

  /**
   * {@inheritdoc}
   */
  public function getConfiguration() {
    return $this->configuration;
  }

  /**
   * {@inheritdoc}
   */
  public function getSetting($key, $default = NULL) {
    return isset($this->configuration[$key]) ? $this->configuration[$key] : $default;
  }

  /**
   * {@inheritdoc}
   */
  public function getLabel() {
    return $this->label;
  }

  /**
   * {@inheritdoc}
   */
  public function getDescription() {
    return $this->getPlugin()->getPluginDefinition()['description'];
  }

  /**
   * {@inheritdoc}
   */
  public function getPluginTitle() {
    return $this->getPlugin()->getPluginDefinition()['title'];
  }

  /**
   * {@inheritdoc}
   */
  public function getPlugin() {
    $configuration = array('plugin_config' => $this);
    $plugin = crm_core_match_matcher_manager()->createInstance($this->plugin_id, $configuration);

    return $plugin;
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

}

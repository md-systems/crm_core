<?php
/**
 * @file
 * Contains \Drupal\crm_core_default_matching_engine\Entity\MatchingRule.
 */

namespace Drupal\crm_core_default_matching_engine\Entity;

use Drupal\Core\Config\Entity\ConfigEntityBase;
use Drupal\crm_core_contact\Entity\ContactType;

/**
 * CRM default match engine rule Entity Class.
 *
 * @ConfigEntityType(
 *   id = "crm_core_default_engine_rule",
 *   label = @Translation("Matching Rule"),
 *   config_prefix = "rule",
 *   handlers = {
 *     "form" = {
 *       "default" = "Drupal\crm_core_default_matching_engine\Form\MatchingRuleForm",
 *     },
 *   },
 *   entity_keys = {
 *     "id" = "type",
 *     "label" = "label",
 *   },
 *   config_export = {
 *     "label",
 *     "type",
 *     "threshold",
 *     "return_order",
 *     "strict",
 *     "rules",
 *   },
 *   links = {
 *     "canonical" = "crm_core_default_matching_engine.rule_edit",
 *     "add-form" = "crm_core_default_matching_engine.rule_add",
 *     "edit-form" = "admin/config/crm-core/match/default/edit/{crm_core_default_engine_rule}",
 *   }
 * )
 */
class MatchingRule extends ConfigEntityBase {

  /**
   * Primary identifier.
   *
   * Matches the id of a contact type.
   *
   * @var string
   */
  public $type;

  /**
   * Defines the score at which a contact is considered a match.
   *
   * @var int
   */
  public $threshold = 0;

  /**
   * The order in which to return contact matches.
   *
   * @var string
   */
  public $return_order = 'created';

  /**
   * Strict matching status.
   *
   * @var bool
   */
  public $strict = FALSE;

  /**
   * @var array
   */
  public $rules = array();

  /**
   * The entity label.
   *
   * @var string
   */
  protected $label;

  /**
   * Overrides Entity::id().
   */
  public function id() {
    return $this->type;
  }

  /**
   * {@inheritdoc}
   */
  public function label() {
    if (empty($this->label)) {
      $type = ContactType::load($this->type);
      $this->label = t('@type Matching Rule', array(
        '@type' => $type->label(),
      ));
    }
    return parent::label();
  }

}

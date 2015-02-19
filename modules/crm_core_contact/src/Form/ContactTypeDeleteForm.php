<?php

/**
 * @file
 * Contains \Drupal\crm_core_contact\Form\ContactTypeDeleteForm.
 */

namespace Drupal\crm_core_contact\Form;

use Drupal\Core\Database\Connection;
use Drupal\Core\Entity\EntityConfirmFormBase;
use Drupal\Core\Form\FormStateInterface;
use Drupal\Core\Url;
use Symfony\Component\DependencyInjection\ContainerInterface;

class ContactTypeDeleteForm extends EntityConfirmFormBase {

  /**
   * The database connection.
   *
   * @var \Drupal\Core\Database\Connection
   */
  protected $database;

  /**
   * Constructs a new NodeTypeDeleteConfirm object.
   *
   * @param \Drupal\Core\Database\Connection $database
   *   The database connection.
   */
  public function __construct(Connection $database) {
    $this->database = $database;
  }

  /**
   * {@inheritdoc}
   */
  public static function create(ContainerInterface $container) {
    return new static(
      $container->get('database')
    );
  }

  /**
   * {@inheritdoc}
   */
  public function getQuestion() {
    return $this->t('Are you sure you want to delete the contact type %type?', array('%type' => $this->entity->label()));
  }

  /**
   * {@inheritdoc}
   */
  public function getCancelUrl() {
    return new Url('entity.crm_core_contact_type.collection');
  }

  /**
   * {@inheritdoc}
   */
  public function getConfirmText() {
    return $this->t('Delete');
  }

  /**
   * {@inheritdoc}
   */
  public function buildForm(array $form, FormStateInterface $form_state) {
    $num_nodes = $this->database->query("SELECT COUNT(*) FROM {crm_core_contact} WHERE type = :type", array(':type' => $this->entity->id()))->fetchField();
    if ($num_nodes) {
      $caption = \Drupal::translation()->formatPlural(
          $num_nodes,
          '%type is used by one contact on your site. You can not remove this contact type until you have removed all of the %type contacts.',
          '%type is used by @count contacts on your site. You may not remove %type until you have removed all of the %type contacts.',
          array('%type' => $this->entity->label()));
      $form['#title'] = $this->getQuestion();
      $form['description'] = array('#markup' => '<p>' . $caption . '</p>');
      return $form;
    }

    return parent::buildForm($form, $form_state);
  }

  /**
   * {@inheritdoc}
   */
  public function submitForm(array &$form, FormStateInterface $form_state) {
    $this->entity->delete();
    $t_args = array('%name' => $this->entity->label());
    drupal_set_message($this->t('The contact type %name has been deleted.', $t_args));
    \Drupal::logger('node')->notice('Deleted contact type %name.', $t_args);

    $form_state->setRedirect('entity.crm_core_contact_type.collection');
  }

}

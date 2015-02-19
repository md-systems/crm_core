<?php

/**
 * @file
 * Contains \Drupal\crm_core_contact\Form\ContactDeleteForm.
 */

namespace Drupal\crm_core_contact\Form;

use Drupal\Core\Database\Connection;
use Drupal\Core\Entity\EntityConfirmFormBase;
use Drupal\Core\Form\FormStateInterface;
use Drupal\Core\Url;
use Symfony\Component\DependencyInjection\ContainerInterface;

class ContactDeleteForm extends EntityConfirmFormBase {

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
    return new Url('crm_core_contact.type_list');
  }

  /**
   * {@inheritdoc}
   */
  public function getConfirmText() {
    return $this->t('Yes');
  }

  /**
   * {@inheritdoc}
   */
  public function submit(array $form, FormStateInterface $form_state) {
    $this->entity->delete();
    $t_args = array(
      '%id' => $this->entity->id(),
      '%name' => $this->entity->label(),
    );
    drupal_set_message($this->t('The contact %name (%id) has been deleted.', $t_args));
    \Drupal::logger('node')->notice('Deleted contact %name (%id).', $t_args);

    $form_state->setRedirect('crm_core_contact.list');
  }

}

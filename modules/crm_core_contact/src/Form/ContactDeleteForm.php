<?php

/**
 * @file
 * Contains \Drupal\crm_core_contact\Form\ContactDeleteForm.
 */

namespace Drupal\crm_core_contact\Form;

use Drupal\Core\Database\Connection;
use Drupal\Core\Entity\EntityConfirmFormBase;
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
    return t('Are you sure you want to delete the contact type %type?', array('%type' => $this->entity->label()));
  }

  /**
   * {@inheritdoc}
   */
  public function getCancelRoute() {
    return array(
      'route_name' => 'crm_core_contact.type_list',
    );
  }

  /**
   * {@inheritdoc}
   */
  public function getConfirmText() {
    return t('Yes');
  }

  /**
   * {@inheritdoc}
   */
  public function submit(array $form, array &$form_state) {
    $this->entity->delete();
    $t_args = array(
      '%id' => $this->entity->id(),
      '%name' => $this->entity->label(),
    );
    drupal_set_message(t('The contact %name (%id) has been deleted.', $t_args));
    watchdog('node', 'Deleted contact %name (%id).', $t_args, WATCHDOG_NOTICE);

    $form_state['redirect_route']['route_name'] = 'crm_core_contact.list';
  }

}

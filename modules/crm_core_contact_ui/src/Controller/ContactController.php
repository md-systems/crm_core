<?php

/**
 * @file
 * Contains \Drupal\crm_core_contact_ui\Controller\ContactController.
 */

namespace Drupal\crm_core_contact_ui\Controller;

use Drupal\Core\Controller\ControllerBase;
use Drupal\crm_core_contact\Entity\ContactType;

class ContactController extends ControllerBase {

  /**
   * Displays add contact links for available contact types.
   *
   * Redirects to node/add/[type] if only one content type is available.
   *
   * @return array
   *   A render array for a list of the node types that can be added; however,
   *   if there is only one node type defined for the site, the function
   *   redirects to the node add page for that one node type and does not return
   *   at all.
   *
   * @see crm_core_contact_menu()
   */
  public function addPage() {
    $content = array();

    // Only use node types the user has access to.
    foreach ($this->entityManager()->getStorage('crm_core_contact_type')->loadMultiple() as $type) {
      if ($this->entityManager()->getAccessController('crm_core_contact')->createAccess($type->type)) {
        $content[$type->type] = $type;
      }
    }

    // Bypass the node/add listing if only one content type is available.
    if (count($content) == 1) {
      $type = array_shift($content);
      return $this->redirect('crm_core_contact.add', array('crm_core_content_type' => $type->type));
    }

    return array(
      '#theme' => 'crm_core_contact_ui_add_list',
      '#content' => $content,
    );
  }

  /**
   * Provides the node submission form.
   *
   * @param \Drupal\crm_core_contact\Entity\ContactType $crm_core_contact_type
   *   The node type entity for the node.
   *
   * @return array
   *   A node submission form.
   */
  public function add(ContactType $crm_core_contact_type) {
    $account = $this->currentUser();

    $contact = $this->entityManager()->getStorage('crm_core_contact')->create(array(
      'uid' => $account->id(),
      'type' => $crm_core_contact_type->type,
    ));

    $form = $this->entityFormBuilder()->getForm($contact);

    return $form;
  }

  /**
   * The _title_callback for the node.add route.
   *
   * @param \Drupal\crm_core_contact\Entity\ContactType $crm_core_contact_type
   *   The current node.
   *
   * @return string
   *   The page title.
   */
  public function addPageTitle(ContactType $crm_core_contact_type) {
    return $this->t('Create @name', array('@name' => $crm_core_contact_type->name));
  }
}

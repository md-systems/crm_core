<?php

/**
 * @file
 * Contains \Drupal\crm_core_contact\Form\ContactTypeForm.
 */

namespace Drupal\crm_core_contact\Form;

use Drupal\Core\Entity\EntityForm;
use Drupal\Core\Entity\EntityTypeInterface;
use Drupal\field\Entity\FieldInstanceConfig;

/**
 * Class ContactTypeForm
 *
 * Form for edit contact types.
 *
 * @package Drupal\crm_core_contact\Form
 */
class ContactTypeForm extends EntityForm {

  /**
   * {@inheritdoc}
   */
  public function form(array $form, array &$form_state) {
    $form = parent::form($form, $form_state);

    /* @var \Drupal\crm_core_contact\Entity\ContactType $type */
    $type = $this->entity;

    $form['name'] = array(
      '#title' => t('Name'),
      '#type' => 'textfield',
      '#default_value' => $type->name,
      '#description' => t('The human-readable name of this contact type. It is recommended that this name begin with a capital letter and contain only letters, numbers, and spaces. This name must be unique.'),
      '#required' => TRUE,
      '#size' => 32,
    );

    $form['type'] = array(
      '#type' => 'machine_name',
      '#default_value' => $type->id(),
      '#maxlength' => EntityTypeInterface::BUNDLE_MAX_LENGTH,
      '#disabled' => $type->isLocked(),
      '#machine_name' => array(
        'exists' => 'Drupal\crm_core_contact\Entity\ContactType::load',
        'source' => array('name'),
      ),
      '#description' => t('A unique machine-readable name for this contact type. It must only contain lowercase letters, numbers, and underscores.'),
    );

    $form['description'] = array(
      '#title' => t('Description'),
      '#type' => 'textarea',
      '#default_value' => $type->description,
      '#description' => t('Describe this contact type.'),
    );

    // Primary fields section.
    $form['primary_fields_container'] = array(
      '#type' => 'fieldset',
      '#title' => t('Primary Fields'),
      '#description' => t('Primary fields are used to tell other modules what fields to use for common communications tasks such as sending an email, addressing an envelope, etc. Use the fields below to indicate the primary fields for this contact type.'),
    );

    // @todo Move primary fields array to some hook. This Would allow extend this
    // list to other modules. This hook should return arra('key'=>t('Name')).
    $default_primary_fields = array('email', 'address', 'phone');
//    $primary_fields = variable_get('crm_core_contact_default_primary_fields', $default_primary_fields);
    $primary_fields = $default_primary_fields;
    $options = array();
    if (isset($type->type)) {
      /* @var FieldInstanceConfig[] $instances */
      $instances = \Drupal::entityManager()->getFieldDefinitions('crm_core_contact', $type->type);
      $instances = isset($instances[$type->type]) ? $instances[$type->type] : array();
      foreach ($instances as $instance) {
        $options[$instance->getName()] = $instance->getLabel();
      }
    }
    foreach ($primary_fields as $primary_field) {
      $form['primary_fields_container'][$primary_field] = array(
        '#type' => 'select',
        '#title' => t('Primary @field field', array('@field' => $primary_field)),
        '#default_value' => empty($type->primary_fields[$primary_field]) ? '' : $type->primary_fields[$primary_field],
        '#empty_value' => '',
        '#empty_option' => t('--Please Select--'),
        '#options' => $options,
      );
    }

    return $form;
  }

  /**
   * {@inheritdoc}
   */
  protected function actions(array $form, array &$form_state) {
    $actions = parent::actions($form, $form_state);
    $actions['submit']['#value'] = t('Save contact type');
    $actions['delete']['#title'] = t('Delete contact type');
    return $actions;
  }

  /**
   * {@inheritdoc}
   */
  public function validate(array $form, array &$form_state) {
    parent::validate($form, $form_state);

    $id = trim($form_state['values']['type']);
    // '0' is invalid, since elsewhere we check it using empty().
    if ($id == '0') {
      $this->setFormError('type', $form_state, $this->t("Invalid machine-readable name. Enter a name other than %invalid.", array('%invalid' => $id)));
    }
  }

  /**
   * {@inheritdoc}
   */
  public function save(array $form, array &$form_state) {
    $type = $this->entity;

    $status = $type->save();

    $t_args = array('%name' => $type->label());

    if ($status == SAVED_UPDATED) {
      drupal_set_message(t('The contact type %name has been updated.', $t_args));
    }
    elseif ($status == SAVED_NEW) {
      drupal_set_message(t('The contact type %name has been added.', $t_args));
      watchdog('crm_core_contact', 'Added contact type %name.', $t_args, WATCHDOG_NOTICE, l(t('View'), 'admin/structure/crm-core/contact-types'));
    }

    $form_state['redirect_route']['route_name'] = 'crm_core_contact.type_list';
  }
}

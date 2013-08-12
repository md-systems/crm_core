<?php

/**
 * @file
 *  Pages for administrative settings in CRM Core Contact.
 */

/**
 * Form callback: create or edit a contact type.
 *
 * @param object $contact_type
 *   (Optional) The contact type object to edit.
 *   Can be empty when used for a create form.
 */
function crm_core_contact_type_form($form, &$form_state, $contact_type) {

  // Store the initial product type in the form state.
  $form_state['contact_type'] = $contact_type;

  $form['contact_type'] = array(
    '#tree' => TRUE,
  );

  $form['contact_type']['name'] = array(
    '#type' => 'textfield',
    '#title' => t('Name'),
    '#default_value' => $contact_type->name,
    '#description' => t('The human-readable name of this contact type. It is recommended that this name begin with a capital letter and contain only letters, numbers, and spaces. This name must be unique.'),
    '#required' => TRUE,
    '#size' => 32,
    '#field_suffix' => ' <small id="edit-product-type-name-suffix">' . $contact_type->type . '</small>',
  );

  if (empty($contact_type->type)) {
    $js_settings = array(
      'type' => 'setting',
      'data' => array(
        'machineReadableValue' => array(
          'contact-type-name' => array(
            'text' => t('Machine name'),
            'target' => 'product-type-type',
            'searchPattern' => '[^a-z0-9]+',
            'replaceToken' => '_',
          ),
        ),
      ),
    );
    $form['contact_type']['type'] = array(
      '#type' => 'textfield',
      '#title' => t('Machine name'),
      '#default_value' => $contact_type->type,
      '#maxlength' => 32,
      '#required' => TRUE,
      '#description' => t('The machine-readable name of this contact type. This name must contain only lowercase letters, numbers, and underscores, it must be unique.'),
      '#attached' => array(
        'js' => array(
          drupal_get_path('module', 'system') . '/system.js',
          $js_settings,
        ),
      ),
    );
  }

  $form['contact_type']['description'] = array(
    '#type' => 'textarea',
    '#title' => t('Description'),
    '#description' => t('Describe this contact type. The text will be displayed on the <em>Add new content</em> page.'),
    '#default_value' => $contact_type->description,
    '#rows' => 3,
  );

  // Primary fields section.
  $form['contact_type']['primary_fields_container'] = array(
    '#type' => 'fieldset',
    '#title' => t('Primary Fields'),
  );

  $form['contact_type']['primary_fields_container']['description_wrapper'] = array(
    '#type' => 'container',
  );
  $primary_fields_description = 'Primary fields are used to tell other modules'
    . ' what fields to use for common communications tasks such as sending an'
    . ' email, addressing an envelope, etc. Use the fields below to indicate'
    . ' the primary fields for this contact type.';
  $form['contact_type']['primary_fields_container']['description_wrapper'] = array(
    '#markup' => t($primary_fields_description),
  );

  // @todo Move primary fields array to some hook. This Would allow extend this
  // list to other modules. This hook should return arra('key'=>t('Name')).
  $default_primary_fields = array('email', 'address', 'phone');
  $primary_fields = variable_get('crm_core_contact_default_primary_fields', $default_primary_fields);
  $options = array();
  if (isset($contact_type->type)) {
    $instances = field_info_instances('crm_core_contact', $contact_type->type);
    foreach ($instances as $instance) {
      $options[$instance['field_name']] = $instance['label'];
    }
  }
  foreach ($primary_fields as $primary_field) {
    $form['contact_type']['primary_fields_container'][$primary_field] = array(
      '#type' => 'select',
      '#title' => t('Primary @field field', array('@field' => $primary_field)),
      '#default_value' => empty($contact_type->primary_fields[$primary_field]) ? '' : $contact_type->primary_fields[$primary_field],
      '#empty_value' => '',
      '#empty_option' => t('--Please Select--'),
      '#options' => $options,
    );
  }

  $form['actions'] = array(
    '#type' => 'container',
    '#attributes' => array('class' => array('form-actions')),
    '#weight' => 40,
  );

  // We add the form's #submit array to this button along with the actual submit
  // handler to preserve any submit handlers added by a form callback_wrapper.
  $submit = array();

  if (!empty($form['#submit'])) {
    $submit += $form['#submit'];
  }

  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save contact type'),
    '#submit' => $submit + array('crm_core_contact_type_form_submit'),
  );

  $form['#validate'][] = 'crm_core_contact_type_form_validate';

  return $form;
}

/**
 * Validation callback.
 */
function crm_core_contact_type_form_validate($form, &$form_state) {
  $contact_type = $form_state['contact_type'];

  // If saving a new contact type, ensure it has a unique machine name.
  if (empty($contact_type->type)) {
    if (!crm_core_contact_type_validate_unique($form_state['values']['contact_type']['type'])) {
      form_set_error('contact_type][type', 'The machine name specified is already in use.');
    }
  }
}

/**
 * Form submit handler: save a contact type.
 */
function crm_core_contact_type_form_submit($form, &$form_state) {
  $contact_type = $form_state['contact_type'];
  $updated = !empty($contact_type->type);

  // Saving primary fields selection.
  $contact_type->primary_fields = $form_state['values']['contact_type']['primary_fields_container'];
  unset($form_state['values']['contact_type']['primary_fields_container']);

  foreach ($form_state['values']['contact_type'] as $key => $value) {
    $contact_type->$key = $value;
  }

  // Write the contact type to the database.
  $contact_type->is_new = !$updated;
  crm_core_contact_type_save($contact_type);

  // Redirect based on the button clicked.
  $form_state['redirect'] = 'admin/structure/crm-core/contact-types/';
  drupal_set_message(t('Contact type saved.'));
}

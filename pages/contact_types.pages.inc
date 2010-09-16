<?php
/**
 * @file
 * Interface elements for adding, editing, and otherwise working with contact types.
 */


function crm_contact_types_overview() {
  $header = array(
    t('Name'),
    t('Operations'),
  );
  $rows = array();

  foreach (crm_contact_types() as $type => $contact_type) {
    $links = menu_contextual_links('crm', 'crm/admin/contact-types/manage', array($type));

    $rows[] = array(
      check_plain($contact_type->name),
      theme('links', array('links' => $links, 'attributes' => array('class' => 'links operations'))),
    );
  }

  $build['contact_type_table'] = array(
    '#theme' => 'table',
    '#header' => $header,
    '#rows' => $rows,
    '#empty' => t('The are no contact types. <a href="@link">Add contact type</a>.', array('@link' => url('crm/admin/contact-types/manage'))),
  );

  return $build;
}

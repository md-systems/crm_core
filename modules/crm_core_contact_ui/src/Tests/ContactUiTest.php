<?php
/**
 * @file
 * Contains \Drupal\crm_core_contact_ui\Tests\ContactUiTest;
 */

namespace Drupal\crm_core_contact_ui\Tests;

use Drupal\crm_core_contact\Entity\Contact;
use Drupal\simpletest\WebTestBase;

/**
 * Tests the UI for Contact CRUD operations
 *
 * @group crm_core
 */
class ContactUiTest extends WebTestBase {

  /**
   * Modules to enable.
   *
   * @var array
   */
  public static $modules = array(
    'text',
    'crm_core_contact',
    'crm_core_contact_ui',
  );

  /**
   * Tests the contact operations.
   *
   * User with permissions 'administer crm_core_contact entities'
   * should be able to create/edit/delete contacts of any contact type.
   *
   * @todo Test with name field once that is available again.
   *   Code that is name field specific was left in as comment so it can be
   *   easily but back in place.
   */
  public function testContactOperations() {
    // Create user and login.
    $user = $this->drupalCreateUser(array('administer crm_core_contact entities', 'view any crm_core_contact entity'));
    $this->drupalLogin($user);

    // There should be no contacts available after fresh installation and
    // there is link to create new contacts.
    $this->drupalGet('crm-core/contact');
    $this->assertText(t('There are no contacts available. Add one now.'), 'No contacts available after fresh installation.');
    $this->assertLink(t('Add a contact'));

    // Open page crm-core/contact/add and assert standard contact types available.
    $this->drupalGet('crm-core/contact/add');
    $this->assertLink(t('Add Household'));
    $this->assertLink(t('Add Individual'));
    $this->assertLink(t('Add Organization'));

    // Create Household contact.
    $household_node = array(
      'name[0][value]' => 'Fam. Smith',
    );
    $this->drupalPostForm('crm-core/contact/add/household', $household_node, 'Save Household');

    // Assert we were redirected back to the list of contacts.
    $this->assertUrl('crm-core/contact');

    $this->assertLink('Fam. Smith', 0, 'Newly created contact title listed.');
    $this->assertText(t('Household'), 'Newly created contact type listed.');

    // Create individual contact.
    $individual_node = array(
      'name[0][value]' => 'Smith',
//      'name[und][0][title]' => 'Mr.',
//      'name[und][0][given]' => 'John',
//      'name[und][0][middle]' => 'Emanuel',
//      'name[und][0][family]' => 'Smith,
//      'name[und][0][generational]' => 'IV',
//      'name[und][0][credentials]' => '',
    );
    $this->drupalPostForm('crm-core/contact/add/individual', $individual_node, 'Save Individual');

    // Assert we were redirected back to the list of contacts.
    $this->assertUrl('crm-core/contact');

    $this->assertLink('Smith', 0, 'Newly created contact title listed.');
    $this->assertText(t('Individual'), 'Newly created contact type listed.');

    // Create Organization contact.
    $organization_node = array(
      'name[0][value]' => 'Example ltd',
    );
    $this->drupalPostForm('crm-core/contact/add/organization', $organization_node, 'Save Organization');

    // Assert we were redirected back to the list of contacts.
    $this->assertUrl('crm-core/contact');

    $this->assertLink('Example ltd', 0, 'Newly created contact title listed.');
    $this->assertText(t('Organization'), 'Newly created contact type listed.');

    // Edit operations.
    // We know that created nodes household is id 1, individual is no 2,
    // organization is no 3. But we should have better API to find contact by
    // name.
    $household_node = array(
      'name[0][value]' => 'Fam. Johnson',
    );
    $this->drupalPostForm('crm-core/contact/1/edit', $household_node, 'Save Household');

    // Assert we were redirected back to the list of contacts.
    $this->assertUrl('crm-core/contact/1');
    $this->assertText('Fam. Johnson', 0, 'Contact updated.');

    // Check listing page.
    $this->drupalGet('crm-core/contact');
    $this->assertLink('Fam. Johnson', 0, 'Updated contact title listed.');

    // Delete household contact.
    $this->drupalPostForm('crm-core/contact/1/delete', array(), 'Yes');
    $this->assertUrl('crm-core/contact');
    $this->assertNoLink('Fam. Johnson', 0, 'Deleted contact title no more listed.');

    // Edit individual contact.
    $individual_node = array(
      'name[0][value]' => 'Johnson',
//      'name[und][0][title]' => 'Mr.',
//      'name[und][0][given]' => 'John',
//      'name[und][0][middle]' => 'Emanuel',
//      'name[und][0][family]' => 'Smith,
//      'name[und][0][generational]' => 'IV',
//      'name[und][0][credentials]' => '',
    );
    $this->drupalPostForm('crm-core/contact/2/edit', $individual_node, 'Save Individual');

    // Assert we were redirected back to the list of contacts.
    $this->assertUrl('crm-core/contact/2');

    // Check listing page.
    $this->drupalGet('crm-core/contact');
    $this->assertLink('Johnson', 0, 'Updated individual contact title listed.');

    // Delete individual contact.
    $this->drupalPostForm('crm-core/contact/2/delete', array(), 'Yes');
    $this->assertUrl('crm-core/contact');
    $this->assertNoLink('Johnson', 0, 'Deleted individual contact title no more listed.');

    // Edit organization contact.
    $organization_node = array(
      'name[0][value]' => 'Another Example ltd',
    );
    $this->drupalPostForm('crm-core/contact/3/edit', $organization_node, 'Save Organization');

    // Assert we were redirected back to the list of contacts.
    $this->assertUrl('crm-core/contact/3');
    $this->assertText('Another Example ltd', 0, 'Contact updated.');

    // Check listing page.
    $this->drupalGet('crm-core/contact');
    $this->assertLink('Another Example ltd', 0, 'Updated contact title listed.');

    // Delete organization contact.
    $this->drupalPostForm('crm-core/contact/3/delete', array(), 'Yes');
    $this->assertUrl('crm-core/contact');
    $this->assertNoLink('Another Example ltd', 0, 'Deleted contact title no more listed.');

    // Assert that there are no contacts left.
    $this->assertText(t('There are no contacts available. Add one now.'), 'No contacts available after fresh installation.');
  }

  /**
   * Tests the contact type operations.
   *
   * User with permissions 'administer contact types' should be able to
   * create/edit/delete contact types.
   */
  public function testContactTypeOperations() {
    // Given I am logged in as a user with permission 'administer contact types'
    $user = $this->drupalCreateUser(array('administer contact types'));
    $this->drupalLogin($user);

    // When I visit the contact type admin page.
    $this->drupalGet('admin/structure/crm-core/contact-types');

    // Then I should see edit, enable, delete but no enable links for existing
    // contacts.
    $this->assertContactTypeLink('household', 'Edit link for household.');
    $this->assertContactTypeLink('household/disable', 'Disable link for household.');
    $this->assertNoContactTypeLink('household/enable', 'No enable link for household.');
    $this->assertContactTypeLink('household/delete', 'Delete link for household.');

    $this->assertcontacttypelink('individual', 'Edit link for individual.');
    $this->assertcontacttypelink('individual/disable', 'Disable link for individual.');
    $this->assertNoContacttypelink('individual/enable', 'No enable link for individual.');
    $this->assertcontacttypelink('individual/delete', 'Delete link for individual.');

    $this->assertcontacttypelink('organization', 'Edit link for organization.');
    $this->assertcontacttypelink('organization/disable', 'Disable link for organization.');
    $this->assertNoContacttypelink('organization/enable', 'No enable link for organization.');
    $this->assertcontacttypelink('organization/delete', 'Delete link for organization.');

    // Given the 'household' contact type is disabled.
    $this->drupalPostForm('admin/structure/crm-core/contact-types/household/disable', array(), 'Disable');

    // When I visit the contact type admin page.
    $this->drupalGet('admin/structure/crm-core/contact-types');

    // Then I should see an enable link.
    $this->assertContactTypeLink('household/enable', 'Enable link for household.');
    // And I should not see a disable link.
    $this->assertNoContactTypeLink('household/disable', 'No disable link for household.');

    // When I enable 'household'
    $this->drupalPostForm('admin/structure/crm-core/contact-types/household/enable', array(), 'Enable');

    // Then I should see a disable link.
    $this->assertContactTypeLink('household/disable', 'Disable link for household.');

    // Given there is a contact of type 'individual.'.
    Contact::create(array('type' => 'individual'))->save();

    // When I visit the contact type admin page.
    $this->drupalGet('admin/structure/crm-core/contact-types');

    // Then I should not see a delete link.
    $this->assertNoContactTypeLink('individual/delete', 'No delete link for individual.');

    // When I edit the organization type.
    $this->drupalGet('admin/structure/crm-core/contact-types/organization');

    // Then I should see a delete link.
    $this->assertContactTypeLink('organization/delete', 'Delete link on organization type form.');

    // When I edit the individual type.
    $this->drupalGet('admin/structure/crm-core/contact-types/individual');

    // @todo Assert for a positive fact to ensure being on the correct page.
    // Then I should not see a delete link.
    $this->assertNoContactTypeLink('individual/delete', 'No delete link on individual type form.');
  }

  /**
   * Asserts a contact type link.
   *
   * The path 'admin/structure/crm-core/contact-types/' gets prepended to the
   * path provided.
   *
   * @see WebTestBase::assertLinkByHref()
   */
  public function assertContactTypeLink($href, $message = '') {
    $this->assertLinkByHref('admin/structure/crm-core/contact-types/' . $href, 0, $message);
  }

  /**
   * Asserts no contact type link.
   *
   * The path 'admin/structure/crm-core/contact-types/' gets prepended to the
   * path provided.
   *
   * @see WebTestBase::assertNoLinkByHref()
   */
  public function assertNoContactTypeLink($href, $message = '') {
    $this->assertNoLinkByHref('admin/structure/crm-core/contact-types/' . $href, $message);
  }
}

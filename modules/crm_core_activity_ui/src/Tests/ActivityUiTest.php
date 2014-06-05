<?php

/**
 * @file
 * Contains \Drupal\crm_core_activity_ui\Tests\ActivityUiTest.
 */

namespace Drupal\crm_core_activity_ui\Tests;

use Drupal\crm_core_contact\Entity\Contact;
use Drupal\simpletest\WebTestBase;

/**
 * Activity UI test case.
 */
class ActivityUiTest extends WebTestBase {

  /**
   * Modules to enable.
   *
   * @var array
   */
  public static $modules = array(
    'crm_core_activity_ui',
  );

  public static function getInfo() {
    return array(
      'name' => t('Activity UI'),
      'description' => t('Test create/edit/delete activities.'),
      'group' => t('CRM Core'),
    );
  }

  /**
   * Test basic UI operations with Activities.
   *
   * Create a contact.
   * Add activity of every type to contact.
   * Assert activities listed on Activities tab listing page.
   * Edit every activity. Assert activities changed from listing page.
   * Delete every activity. Assert they disappeared from listing page.
   */
  public function testActivityOperations() {
    // Create and login user. User should be able to create contacts and
    // activities.
    $user = $this->drupalCreateUser(array(
      'administer crm_core_contact entities',
      'view any crm_core_contact entity',
      'administer crm_core_activity entities',
    ));
    $this->drupalLogin($user);

    // Create Household contact.
    $household = Contact::create(array(
      'contact_name[0][name]' => $this->randomName(),
      'type' => 'household',
    ));
    $household->save();

    $this->drupalGet('crm-core/activity');
    $this->assertText(t('There are no activities available.'), t('No activities available.'));
    $this->assertLink(t('Add an activity'));

    $this->drupalGet('crm-core/activity/add');
    $this->assertLink(t('Add Meeting'));
    $this->assertLink(t('Add Phone call'));

    // Create Meeting activity. Ensure it it listed.
    $meeting_activity = array(
      'title[0][value]' => 'Pellentesque',
      'activity_date[0][value][date]' => $this->randomDate(),
      'activity_date[0][value][time]' => $this->randomTime(),
      'activity_notes[0][value]' => $this->randomString(),
      'activity_participants[0][target_id]' => $household->label() . ' (' . $household->id() . ')',
    );
    $this->drupalPostForm('crm-core/activity/add/meeting', $meeting_activity, t('Save Activity'));
    $this->assertText('Activity Pellentesque created.', t('No errors after adding new activity.'));

    // Create Meeting activity. Ensure it it listed.
    $phonecall_activity = array(
      'title[0][value]' => 'Mollis',
      'activity_date[0][value][date]' => $this->randomDate(),
      'activity_date[0][value][time]' => $this->randomTime(),
      'activity_notes[0][value]' => $this->randomString(),
      'activity_participants[0][target_id]' => $household->label() . ' (' . $household->id() . ')',
    );
    $this->drupalPostForm('crm-core/activity/add/phone_call', $phonecall_activity, t('Save Activity'));
    $this->assertText('Activity Mollis created.', t('No errors after adding new activity.'));

    // Update activity and assert its title changed on the list.
    $meeting_activity = array(
      'title[0][value]' => 'Vestibulum',
    );
    $this->drupalPostForm('crm-core/activity/1/edit', $meeting_activity, t('Save Activity'));
    $this->assertText('Vestibulum', t('Activity updated.'));
    $this->drupalGet('crm-core/activity');
    $this->assertLink('Vestibulum', 0, t('Updated activity listed properly.'));

    // Update phone call activity and assert its title changed on the list.
    $phonecall_activity = array(
      'title[0][value]' => 'Commodo',
    );
    $this->drupalPostForm('crm-core/activity/2/edit', $phonecall_activity, t('Save Activity'));
    $this->assertText('Commodo', t('Activity updated.'));
    $this->drupalGet('crm-core/activity');
    $this->assertLink('Commodo', 0, t('Updated activity listed properly.'));

    // Delete Meeting activity.
    $this->drupalPostForm('crm-core/activity/1/delete', array(), t('Delete'));
    $this->assertText('Meeting Vestibulum has been deleted.', t('No errors after deleting activity.'));
    $this->drupalGet('crm-core/activity');
    $this->assertNoLink('Vestibulum', t('Deleted activity is no more listed.'));

    // Delete Phone call activity.
    $this->drupalPostForm('crm-core/activity/2/delete', array(), t('Delete'));
    $this->assertText('Phone call Commodo has been deleted.', t('No errors after deleting activity.'));
    $this->drupalGet('crm-core/activity');
    $this->assertNoLink('Commodo', t('Deleted activity is no more listed.'));

    // Assert there is no activities left.
    $this->drupalGet('crm-core/activity');
    $this->assertText(t('There are no activities available.'), t('No activities listed.'));
  }

  /**
   * Generate random Date for form element input.
   */
  function randomDate() {
    return format_date(REQUEST_TIME + rand(0, 100000), 'custom', 'Y-m-d');
  }

  /**
   * Generate random Time for form element input.
   */
  function randomTime() {
    return format_date(REQUEST_TIME + rand(0, 100000), 'custom', 'H:m:s');
  }
}

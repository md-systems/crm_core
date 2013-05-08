-- SUMMARY --

CRM Core is a set of modules that provide contact management functionality within Drupal 
web sites. The module supports contacts, relationships and activities, and provides various UI
elements can can (optionally) be used for maintaining these entities. It also provides support 
for identifying duplicate contacts, linking contact records to user accounts, and generating 
reports.

-- REQUIREMENTS --

A number of modules ship with CRM Core. They require the following modules to operate
properly, in addition to some interdependencies between the modules themselves:

* Entity API
* Field UI
* Field
* Field SQL storage
* Views
* Chaos tools
* Fieldgroup
* Name Field
* Entity Reference
* Date API
* Date

-- INSTALLATION --

CRM Core can be installed like any other Drupal module.

1) Download CRM Core to the modules directory for your site.

2) Go to the admin/modules page and enable CRM Core.

From here, select the options most appropriate for your site:

- if you want to use activity management in CRM Core, enable CRM Core Activity. You will probably also
  want to enable CRM Core Activity UI, but this is not required.

- if you want to use relationship management, enable CRM Core Relationship. You will probably also want to 
  enable CRM Core Relationship UI, but this is not required.
  
- if you want to provide a UI for viewing reports defined by CRM Core features, enable CRM Core Report.
  Please note, CRM Core does not ship with reports of it's own. This page will be blank until you define
  reports.
  
- if you want to link contact records to user accounts, enable CRM Core User Sync.

- if you want to use matching engines to identify duplicate contacts, enable CRM Core Match.

- if you want to use the default matching engine that ships with CRM Core, enable CRM Core Default Matching
  Engine.
  
-- UPGRADING --

Any time you upgrade the CRM Core modules, run update.php on your Drupal website.

-- RELATED MODULES --

* CRM Core Profile

  A form builder for CRM Core. Allows administrators to dynamically create forms for interacting
  with contact information.

  http://drupal.org/project/crm_core_profile

-- CONFIGURATION --

There are a number of options for how to configure CRM Core. Please see the project documentation at 
http://drupal.org/node/1856906 for more details on the configuration and setup process.

-- CONTACT --

Current maintainers:

* techsoldaten - http://drupal.org/user/16339

If you are interested in participating in CRM Core development or seek assistance with CRM Core,
please contact me directly through this form.

Development sponsored by Trellon.


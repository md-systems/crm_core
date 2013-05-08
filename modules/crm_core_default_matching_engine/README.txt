-- SUMMARY --

CRM Core Default Matching Engine is a matching engine for CRM Core Match. It is capable of identifying
duplicate contacts in CRM Core based on wieghted scores and logical operators.

-- REQUIREMENTS --

The following modules are required by CRM Core Default Matching Engine:

* CRM Core Match
* CRM Core
* CRM Core Contact
* Entity API
* Field UI
* Field
* Field SQL storage
* Views
* Chaos tools
* Fieldgroup
* Name Field

-- INSTALLATION --

CRM Core can be installed like any other Drupal module.

1) Download CRM Core to the modules directory for your site.

2) Go to the admin/modules page and enable CRM Core Default Matching Engine.
  
-- CONFIGURATION --

1) Go to admin/config/crm-core/match and enable CRM Core Default Matching engine.

2) Click on the link to configure matching rules for contact types.

3) For each contact type, you will be configuring a weight for matches on each field.

4) There is a threshhold value listed at the top of the page. This is the total the sum of the 
   field matches must equal or exceed in order to indicate a positive match. 
   
5) Select the fields for matching by checking the appropriate checkbox.

6) Select a logical operator for each field.

7) Enter a weight for each selected field. Matches will be ordered according to the total 
   of the field values.

-- ABOUT MATCHING ENGINES --

CRM Core Default Matching Engine is a simple implementation of contact matching logic that can be 
used on most Drupal websites. It was designed to interoperate with CRM Core Match, and can act as 
one of several matching engines.

If you are looking for a way to build more advanced matching systems, or to inject information
into contact records when they are being identified as potential matches, you are encouraged to 
develop your own matching engines.

See the README.txt for CRM Core Match for more details.

-- CONTACT --

Current maintainers:

* techsoldaten - http://drupal.org/user/16339

If you are interested in participating in CRM Core development or seek assistance with CRM Core,
please contact me directly through this form.

Development sponsored by Trellon.


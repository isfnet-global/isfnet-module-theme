For latest documentation visit: http://drupal.org/node/1432894
Introduction
------------
This module allows creating the front-end forms using Drupal's Field systems.

For more information on adding fields see the Field UI documentation here: http://drupal.org/documentation/modules/field-ui




Installation
-------------
Once you activate the module it sets up an entity administration interface under
Administration > Content > Entityform Types


Usage
---------------
1. Enable the module
2. Goto admin/structure/entityform_types
3. Click "Add an Entityform Type"
4. Fill out basic form information. Under Access Settings make sure at least 1 role can submit the form
5. Click "Save Entityform Type"
6. Click manage fields and add fields the same way you would for a node content type.
7. Once you have added fields you can view the form by clicking the Submit Link on admin/structure/entityform_types


Access Rules
------------------
Rules Components can be used to determine if the form fields will be shown to the user.  
These Rules are run after the user has accessed the form page.
Two types of Rules components can be used to determine if the fields should be shown
1. Conidtional Component(either "and" or "or")
  If the conditional component evaluates to TRUE then the fields will be shown
  The conditional component must accept 2 parameters
    1. Entityform
    2. Entityform type
2. Rules Components
  The Rule component must have 3 variable
    1. Provided variable - Truth Value. 
      If this is set to FALSE or Zero then the fields will not show
      If this is NOT SET the field values will show
    2. Parameter - Entityform
    3. Parameter - Entityform Type


Module Intergration
---------------------
The aim of this module is create a form creation method that leverages that power of entities and fields.  For this reason instead of writting custom code

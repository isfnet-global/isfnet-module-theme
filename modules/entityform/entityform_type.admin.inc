<?php
define('ENTITYFORM_TYPE_DEFAULT_PROP_TEXT', 'Leave this field blank to use default setting.');
/**
 * @file
 * Entityform type editing UI.
 */

/**
 * UI controller.
 */
class EntityformTypeUIController extends EntityDefaultUIController {

  /**
   * Overrides hook_menu() defaults.
   */
  public function hook_menu() {
    $items = parent::hook_menu();
    $items[$this->path]['description'] = 'Manage entityform entity types, including adding and removing fields and the display of fields.';
    $items[$this->path]['type'] = MENU_NORMAL_ITEM;
    $items[$this->path . '/manage/%entity_object/edit']['context'] = MENU_CONTEXT_PAGE | MENU_CONTEXT_INLINE;

    return $items;
  }

  protected function overviewTableRow($conditions, $id, $entity, $additional_cols = array()) {
    $row = parent::overviewTableRow($conditions, $id, $entity, $additional_cols);
    $row[] = l(t('Submissions'), $this->path . '/manage/' . $id . '/submissions');
    $row[] = l(t('Submit Link'), entity_ui_controller('entityform')->submit_path($id));
    return $row;
  }

  public function overviewTable($conditions = array()) {
    $render = parent::overviewTable($conditions);
    $render['#header'][] = t('Submissions');
    $render['#header'][] = t('Submit Link');
    return $render;
  }
}


/**
 * Generates the entityform type editing form.
 */
function entityform_type_form($form, &$form_state, $entityform_type, $op = 'edit') {

  if ($op == 'clone') {
    $entityform_type->label .= ' (cloned)';
    $entityform_type->type = '';
  }
  $form = _entityform_type_setttings_elements($entityform_type, $op);
  $form['#entityform_type'] = $entityform_type;
  $form['label'] = array(
    '#title' => t('Label'),
    '#type' => 'textfield',
    '#default_value' => $entityform_type->label,
    '#description' => t('The human-readable name of this entityform type.'),
    '#required' => TRUE,
    '#size' => 30,
    '#weight' => -50,
  );

  // Machine-readable type name.
  $form['type'] = array(
    '#type' => 'machine_name',
    '#default_value' => isset($entityform_type->type) ? $entityform_type->type : '',
    '#maxlength' => 32,
    //    '#disabled' => $entityform_type->isLocked() && $op != 'clone',
    '#machine_name' => array(
      'exists' => 'entityform_get_types',
      'source' => array('label'),
    ),
    '#description' => t('A unique machine-readable name for this entityform type. It must only contain lowercase letters, numbers, and underscores.'),
  );
  $form['data']['#tree'] = TRUE;

  $form['data']['redirect_path'] = array(
    '#type' => 'textfield',
    '#title' => t('Redirect Path'),
    '#default_value' => empty($entityform_type->data['redirect_path']) ? '' : $entityform_type->data['redirect_path'],
    '#description' => t('What path should the user be redirected to on correct submission?  Leave blank for default action'),
  );
  $form['data']['instruction_pre'] = array(
    '#type' => 'text_format',
    '#title' => t('Intro Form instructions'),
    '#default_value' => empty($entityform_type->data['instruction_pre']['value']) ? '' : $entityform_type->data['instruction_pre']['value'],
    '#format' => empty($entityform_type->data['instruction_pre']['format']) ? NULL : $entityform_type->data['instruction_pre']['format'],
  );
  _entityform_remove_fieldsets_from_tree($form['data'], array('data'));

  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save entityform type'),
    '#weight' => 40,
  );
  if (module_exists('path')) {
    $form['paths'] = array(
      '#type' => 'fieldset',
      '#title' => t('URL path settings'),
      '#collapsible' => TRUE,
      '#collapsed' => empty($path['alias']),
      '#group' => 'additional_settings',
      '#attributes' => array(
        'class' => array('path-form'),
      ),
      '#access' => user_access('create url aliases') || user_access('administer url aliases'),
      '#weight' => 30,
      '#tree' => TRUE,
      '#element_validate' => array('_entityform_paths_form_element_validate'),
    );
    $existing_paths = array(
      'submit' => array(
        'default_path' => _entityform_type_get_submit_url( $entityform_type->type),
        'title' => 'Submit URL alias',
      ),
      'confirm' => array(
        'default_path' => _entityform_type_get_confirm_url( $entityform_type->type),
        'title' => 'Confirm URL alias',
      ),
    );
    foreach ($existing_paths as $key => $existing_path) {
      if ($op == 'edit') {
        //check for existing alias
        $conditions = array('source' => $existing_path['default_path']);
        $path = path_load($conditions);
        if ($path === FALSE) {
          $path = array();
        }
      }
      else {
        $path = array();
      }
      $path += array(
        'pid' => NULL,
        'source' => isset($entityform_type) ? $existing_path['default_path'] : NULL,
        'alias' => '',
        'language' => LANGUAGE_NONE,
      );
      $form['paths'][$key]['alias'] = array(
        '#type' => 'textfield',
        '#title' => check_plain($existing_path['title']),
        '#default_value' => $path['alias'],
        '#maxlength' => 255,
        '#collapsible' => TRUE,
        '#collapsed' => TRUE,
        '#description' => t('Optionally specify an alternative URL by which this content can be accessed. For example, type "about" when writing an about page. Use a relative path and don\'t add a trailing slash or the URL alias won\'t work.'),
      );
      $form['paths'][$key]['pid'] = array(
        '#type' => 'value',
        '#value' => $path['pid'],
      );
      $form['paths'][$key]['source'] = array(
        '#type' => 'value',
        '#value' => $path['source'],
      );
      $form['paths'][$key]['language'] = array(
        '#type' => 'value',
        '#value' => $path['language'],
      );
    }
    $form['additional_settings'] = array(
      '#type' => 'vertical_tabs',
      '#weight' => 99,
    );
  }
  if (module_exists('token')) {
    $form['tokens'] = array(
      '#title' => t('Available Tokens'),
      '#type' => 'fieldset',
      '#collapsible' => TRUE,
      '#weight' => '39',
    );
    $form['tokens']['token_tree'] = array(
      '#theme' => 'token_tree',
      '#token_types' => array('global_types', 'entityform_type', 'entityform'),
    );
  }
  else {
    $form['tokens'] = array(
      '#markup' => '<p>' . t('Enable the <a href="@drupal-token">Token module</a> to view the available token browser.', array('@drupal-token' => 'http://drupal.org/project/token')) . '</p>',
    );
  }

  return $form;
}
/**
 * Entityform global setting form
 * @param array $form
 * @param array $form_state
 * @return array
 */
function entityform_settings($form, &$form_state) {
  //choose the default View for viewing for submissions
  $defaults = entity_get_controller('entityform_type')->create();
  $form = _entityform_type_setttings_elements($defaults, 'defaults');
  $form['#entityform_type'] = $defaults;
  $form['entityform_type_defaults']['data'] = $form['data'];
  $form['entityform_type_defaults']['#tree'] = TRUE;
  $form['entityform_type_defaults']['#type'] = 'value';
  unset($form['data']);
  _entityform_remove_fieldsets_from_tree($form['entityform_type_defaults']['data'], array('entityform_type_defaults', 'data'));
  //collaspe all fieldsets
  foreach (element_children($form['entityform_type_defaults']['data']) as $key) {
    if (isset($form['entityform_type_defaults']['data'][$key]['#type']) && $form['entityform_type_defaults']['data'][$key]['#type'] == 'fieldset') {
      $form['entityform_type_defaults']['data'][$key]['#collapsed'] = TRUE;
    }
  }
  return system_settings_form($form);
}

/**
 * Create Form elements that are shared between EntityformType add/edit form and EntityformType default settings form
 * This allows default settings to set for most form settings.
 * @param $entityform_type
 * @param $op
 * @return array
 *   Form elements
 */
function _entityform_type_setttings_elements($entityform_type, $op) {
  //****************DRAFT FIELDSET SETTINGS *********************//
  $form['data']['draft_set'] = array(
      '#type' => 'fieldset',
      '#title' => t('Draft settings'),
      '#collapsible' => TRUE,
      '#group' => 'additional_settings',
      '#weight' => 20,
  );
  $form['data']['draft_set']['draftable'] = array(
      '#type' => 'checkbox',
      '#title' => t('Draftable'),
      '#default_value' => !empty($entityform_type->data['draftable']),
      '#description' => t('Should users be able to save a draft of this form?'),
  );
  $form['data']['draft_set']['draft_button_text'] = array(
      '#type' => 'textfield',
      '#title' => t('Override Draft Button Text'),
      '#default_value' => empty($entityform_type->data['draft_button_text'])?'':$entityform_type->data['draft_button_text'],
      '#description' => t('Text to use for draft save button.  If blank it will default to "Save Draft".'),
  );
  $form['data']['draft_set']['draft_save_text'] = array(
      '#type' => 'text_format',
      '#title' => t('Draft save text'),
      '#default_value' => empty($entityform_type->data['draft_save_text']['value']) ? '' : $entityform_type->data['draft_save_text']['value'],
      '#format' => empty($entityform_type->data['draft_save_text']['format']) ? NULL : $entityform_type->data['draft_save_text']['format'],
      '#description' => t('Text to dispaly to the user when the form is saved as a draft'),
  );
  //****************FORM OVERRIDE FIELDSET SETTINGS *********************//
  $form['data']['formoverride_set'] = array(
      '#type' => 'fieldset',
      '#title' => t('Form Overrides'),
      '#collapsible' => TRUE,
      '#group' => 'additional_settings',
      '#weight' => 25,
  );
  $form['data']['formoverride_set']['submit_button_text'] = array(
      '#type' => 'textfield',
      '#title' => t('Override Save Button Text'),
      '#default_value' => empty($entityform_type->data['submit_button_text'])?'':$entityform_type->data['submit_button_text'],
      '#description' => t('Text to use for submit button.  If blank it will default to "Submit Form".'),
  );

  //****************SUBMISSION PAGE FIELDSET SETTINGS *********************//
  $form['data']['submission_page_set'] = array(
      '#type' => 'fieldset',
      '#title' => t('Submission Page settings'),
      '#collapsible' => TRUE,
      '#group' => 'additional_settings',
      '#weight' => 40,
  );
  $form['data']['submission_page_set']['submission_page_title'] = array(
      '#type' => 'textfield',
      '#title' => t('Submission Page Title'),
      '#default_value' => empty($entityform_type->data['submission_page_title']) ? '' : $entityform_type->data['submission_page_title'],
      '#description' => t('Page title for correct submission.') . ($op != 'defaults' ? ' ' . ENTITYFORM_TYPE_DEFAULT_PROP_TEXT:''),
  );

  $form['data']['submission_page_set']['submission_text'] = array(
      '#type' => 'text_format',
      '#title' => t('Submission Reply'),
      '#default_value' => empty($entityform_type->data['submission_text']['value']) ? '' : $entityform_type->data['submission_text']['value'],
      '#format' => empty($entityform_type->data['submission_text']['format']) ? NULL : $entityform_type->data['submission_text']['format'],
      '#description' => t('Text for correct submission'),
  );
  $form['data']['submission_page_set']['submission_show_submitted'] = array(
      '#type' => 'checkbox',
      '#title' => t('Show Submission information'),
      '#default_value' => !empty($entityform_type->data['submission_show_submitted']),
      '#description' => t('Show submitted data on submission page?'),
  );
  //****************SUBMISSION VIEWS FIELDSET SETTINGS *********************//
  // Let Each entityform use a different View for viewing Submission
  $form['data']['submissions_set'] = array(
      '#type' => 'fieldset',
      '#title' => t('Submissions View'),
      '#collapsible' => TRUE,
      '#group' => 'additional_settings',
      '#weight' => 30,
  );
  $view_options = _entityform_get_entityform_views_options();
  if ($op != 'defaults') {
    $view_options['default'] = t('Use Default');
  }
  $submission_view = $entityform_type->data['submissions_view'];
  $parts = explode(':', $submission_view);
  $submission_view = $parts[0];
  $form['data']['submissions_set']['submissions_view'] = array(
      '#type' => 'select',
      '#title' => t('View for Submissions'),
      '#default_value' => $submission_view,
      '#options' => $view_options,
  );
  $form['entityform_default_submission_view']['#options'] = _entityform_get_entityform_views_options();
  //****************ACCESS FIELDSET SETTINGS *********************//
  $form['data']['access_set'] = array(
      '#type' => 'fieldset',
      '#title' => t('Access settings'),
      '#collapsible' => TRUE,
      '#group' => 'additional_settings',
      '#weight' => 50,
  );
  $form['data']['access_set']['form_status'] = array(
      '#type' => 'select',
      '#title' => t('Form Status'),
      '#options' => array(
          ENTITYFORM_STATUS_OPEN => t('Open for new submissions'),
          ENTITYFORM_STATUS_CLOSED => t('Closed form new form submissions'),
      ),
      '#default_value' => empty($entityform_type->data['form_status']) ? 'open' : $entityform_type->data['form_status'],
      '#description' => t('Can users submit this form?'),
  );
  $form['data']['access_set']['roles'] = array(
      '#type' => 'checkboxes',
      '#title' => t('Roles'),
      '#options' => user_roles(),
      '#default_value' => empty($entityform_type->data['roles']) ? array() : $entityform_type->data['roles'],
      '#description' => t('Please select the Role(s) that can submit this form.'),
      '#required' => TRUE,
      '#multiple' => TRUE,
  );
  $form['data']['access_set']['resubmit_action'] = array(
      '#type' => 'select',
      '#title' => t('Resubmit Action'),
      '#options' => array(
          'new' => t('Allow new submission'),
          'old' => t('Edit old submission'),
          'disallow' => t("Don't allow"),
          'confirm' => t('Goto Confirm page'),
      ),
      '#default_value' => empty($entityform_type->data['resubmit_action']) ? 'new' : $entityform_type->data['resubmit_action'],
      '#description' => t('Action to take if logged in user has already submitted form.'),
  );
  $form['data']['access_set']['access_rules'] = array(
    '#type' => 'select',
    '#title' => t('Access Rules'),
    '#description' => t('Select the Rules to invoke to determine if the user will be shown the form fields. This does not control whether the user can see the form page.'),
    '#default_value' => empty($entityform_type->data['access_rules']) ? array() : $entityform_type->data['access_rules'],
    '#multiple' => TRUE,
    '#options' => entityform_get_rule_options(array('and', 'or', 'rule', 'rule set'), 'entityform access'),
  );

  return $form;
}
/**
 * Form element validation handler for URL alias form element.
 */
function _entityform_paths_form_element_validate($element, &$form_state, $complete_form) {
  $previous_pathes = array();
  foreach ($form_state['values']['paths'] as $path_type => $path) {
    // Trim the submitted value.
    $alias = trim($path['alias']);
    if (!empty($alias)) {
      form_set_value($element[$path_type]['alias'], $alias, $form_state);

      $path = $form_state['values']['paths'][$path_type];
      if (in_array($path['alias'], $previous_pathes)) {
        form_error($element[$path_type]['alias'], "Aliases must be unique.");
        return;
      }
      $previous_pathes[] = $path['alias'];

      // Ensure that the submitted alias does not exist yet.
      $query = db_select('url_alias')
      ->condition('alias', $path['alias'])
      ->condition('language', $path['language']);
      if (!empty($path['source'])) {
        $query->condition('source', $path['source'], '<>');
      }
      $query->addExpression('1');
      $query->range(0, 1);
      if ($query->execute()->fetchField()) {
        form_error($element[$path_type]['alias'], t('The alias is already in use.'));
        return;
      }
    }
  }

}
function _entityform_form_email_list_validate($element, &$form_state, $form) {
  $emails = explode(', ', $element['#value']);
  foreach ($emails as $email) {
    $email = trim($email);
    if (!empty($email) && !valid_email_address($email)) {
      form_error($element, t('Please enter valid email addresses.'));
      return;
    }
  }
}


/**
 * Form API submit callback for the type form.
 */
function entityform_type_form_submit(&$form, &$form_state) {
  $entityform_type = entity_ui_form_submit_build_entity($form, $form_state);
  if (empty($entityform_type->is_new)) {
    $form_state['redirect'] = 'admin/structure/entityform_types';
  }
  else {
    drupal_set_message(t('The Entityform @label has been created. Add fields to this form below', array('@label' => $entityform_type->label)));
    $form_state['redirect'] = "admin/structure/entityform_types/manage/{$entityform_type->type}/fields";
  }
  $entityform_type->save();
}


/**
 * Form API submit callback for the delete button.
 */
function entityform_type_form_submit_delete(&$form, &$form_state) {
  $form_state['redirect'] = 'admin/structure/entityform_types/manage/' . $form_state['entityform_type']->type . '/delete';
}
function _entityform_type_get_submit_url($type) {
  return 'eform/submit/' . str_replace('_', '-', $type);
}
function _entityform_type_get_confirm_url($type) {
  return 'eform/' . str_replace('_', '-', $type) . '/confirm';
}
function entityform_get_rule_options($plugin_types, $tag) {
  $conditions = array(
      'plugin' => $plugin_types,
      'active' => TRUE,
      );
  $entities = entity_load('rules_config', FALSE, $conditions);
  ksort($entities);
  $rules = array();
  foreach ($entities as $entity) {
    if (in_array($tag, $entity->tags)) {
      $rules[$entity->name] = $entity->label;
    }
  }
  return $rules;
}

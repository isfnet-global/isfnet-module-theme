<?php

/**
 * @file
 * Admin interface for the pcp module
 */

/**
 * Form builder; Configure PCP fields for core profile.
 *
 * @ingroup forms
 */
function pcp_admin_settings($form, &$form_state) {
  $form['general_settings'] = array(
    '#type' => 'fieldset',
    '#title' => 'General Settings',
  );
  $form['general_settings']['hide_block_on_complete'] = array(
    '#type' => 'checkbox',
    '#title' => 'Hide Block When Complete',
    '#default_value' => variable_get('pcp_hide_block_on_complete', 0),
    '#description' => t('When a user reaches 100% complete of their profile, do you want the profile complete percent block to go away? If so, check this box on.'),
  );
  if (variable_get('user_pictures', 0)) {
    $form['general_settings']['pcp_enable_user_picture'] = array(
      '#type' => 'checkbox',
      '#title' => 'Enable User Picture Support',
      '#default_value' => variable_get('pcp_enable_user_picture', 0),
      '#description' => t('Picture support has been turned on, do you want to make it required as part of Profile Complete Percent? Checking this box will make it required.'),
    );
  }

  $form['profile_field_settings'] = array(
    '#type' => 'fieldset',
    '#title' => 'Core Profile Field Settings',
    '#description' => t('Checking a profile field below will add that field to the logic of the complete percentage. Please note that these are simply core profile fields, not fields attached to Profile2 profiles.'),
  );

  $options = array();
  $settings = pcp_admin_settings_form_data('user');
  foreach ($settings['profile_fields_options'] as $fiid => $field_name) {
    $field_info = field_info_instance('user', 'field_zucht', 'user');
    $options[$fiid] = $field_info['label'];
  }

  $form['profile_field_settings']['profile_fields'] = array(
    '#title' => t('Profile Fields'),
    '#description' => empty($options) ? t('There are no profile fields enabled. Please <a href="@link">add some profile fields</a> first.', array('@link' => url('admin/config/people/accounts/fields'))) : t('Checking a profile field below will add that field to the logic of the complete percentage.'),
    '#type' => 'checkboxes',
    '#options' => $options,
    '#default_value' => array_flip($settings['default_values']),
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
  );

  return $form;
}

/**
 * Implements hook_form_FORM_ID_alter().
 * Form builder; Configure PCP fields for profile2 profile types.
 */
function pcp_form_profile2_type_form_alter(&$form, &$form_state, $form_id) {

  $bundle = $form['type']['#default_value'];

  $form['profile_complete'] = array(
    '#type' => 'fieldset',
    '#title' => 'Profile Complete Settings',
  );

  $form['profile_complete']['hide_block_on_complete'] = array(
    '#type' => 'checkbox',
    '#title' => 'Hide Block When Complete',
    '#default_value' => variable_get('pcp_profile2_' . $bundle . '_hide_block_on_complete', 0),
    '#description' => t('When a user reaches 100% complete of their profile, do you want the profile complete percent block to go away? If so, check this box on.'),
  );

  $options = array();
  $settings = pcp_admin_settings_form_data('profile2', $bundle);
  foreach ($settings['profile_fields_options'] as $fiid => $field_name) {
    $field_info = field_info_instance('user', 'field_zucht', 'user');
    $options[$fiid] = $field_info['label'];
  }
  $form['profile_complete']['profile_fields'] = array(
    '#title' => t('Profile Fields'),
    '#description' => empty($options) ? t('There are no fields enabled for this profile type. Please <a href="@link">add some profile fields</a> first.', array('@link' => url('admin/structure/profiles/manage/' . $bundle . '/fields'))) : t('Checking a profile field below will add that field to the logic of the complete percentage.'),
    '#type' => 'checkboxes',
    '#options' => $options,
    '#default_value' => array_flip($settings['default_values']),
  );

  $form['#submit'][] = 'pcp_admin_settings_submit';

  return $form;
}

/**
 * Implements hook_form_FORM_ID_alter().
 */
function pcp_form_user_admin_settings_alter(&$form, &$form_state) {
  if (user_access('administer pcp')) {
    $form['personalization']['pictures']['pcp_enable_user_picture'] = array(
      '#title' => 'Make required for PCP module',
      '#type' => 'checkbox',
      '#default_value' => variable_get('pcp_enable_user_picture', 0),
      '#description' => t('Checking this box will tag this field as a required field for completion of the users profile.'),
      '#weight' => -5,
    );
    $form['#submit'][] = 'pcp_admin_settings_submit';
  }
}

/**
 * Implements hook_form_FORM_ID_alter().
 */
function pcp_form_field_ui_field_edit_form_alter(&$form, &$form_state) {
  // @todo do we need user access check here?

  if ($form['#instance']['entity_type'] == 'profile2') {

    $tag = FALSE;
    $result = pcp_get_tagged_profile_fields($form['#instance']['id']);
    if (!empty($result)) {
      $tag = TRUE;
    }

    $form['instance']['pcp_tag'] = array(
      '#type' => 'checkbox',
      '#title' => t('Make required for PCP module'),
      '#description' => t('Checking this box will tag this field as a required field for completion of the users profile.'),
      '#default_value' => $tag,
      '#weight' => -5,
    );
    $form['#submit'][] = 'pcp_admin_settings_submit';

  }
}

/**
 * Function that sets up parameters to be used
 * when the pcp_admin_settings_form() function
 * is executed.
 *
 * @param $entity_type
 *   this parameter indicates the source of the profile fields. accepted
 *   values include user and profile2.
 * @param $bundle
 *   (optional) NULL The bundle to return fields for.
 *
 * @return - assoc array
 *  ['profile_fields_options']
 *    - An associative array of all fields created from the profile module.
 *  ['default_values']
 *    - An indexed array of all (if any) default values for the form.
 */
function pcp_admin_settings_form_data($entity_type, $bundle = NULL) {
  $options = array();

  $field_conditions = array(
    'entity_type' => $entity_type,
  );

  // If we're dealing with a profile2 entity, specify the field bundle.
  if ($entity_type == 'profile2') {
    $field_conditions = array_merge($field_conditions, array('bundle' => $bundle));
  }

  $options['profile_fields_options'] = pcp_get_profile_fields($field_conditions);
  $options['default_values'] = pcp_get_tagged_profile_fields(NULL, $entity_type, $bundle);

  return $options;
}

/**
 * Submit callback.
 */
function pcp_admin_settings_submit($form, &$form_state) {
  switch ($form['#form_id']) {

    // admin/config/people/accounts
    case 'user_admin_settings':
      variable_set('pcp_enable_user_picture', $form_state['values']['pcp_enable_user_picture']);
      break;

    // admin/config/people/accounts/fields/[field_name]
    case 'field_ui_field_edit_form':
      if ($form['#instance']['entity_type'] == 'profile2') {
        $fiid = $form['#instance']['id'];
        _pcp_disable_field($fiid);
        if ($form_state['values']['instance']['pcp_tag']) {
          _pcp_enable_field($fiid);
        }
      }
      break;

    // admin/structure/profiles/manage/[profile_type]
    case 'profile2_type_form':
      // Provides the profile2 pcp data
      $profile_type = $form_state['values']['type'];
      variable_set('pcp_profile2_' . $profile_type . '_hide_block_on_complete', $form_state['values']['hide_block_on_complete']);
      // Process the profile fields that have been modified
      if ($form_state['values']['profile_fields']) {
        _pcp_save_profile_fields($form_state['values']['profile_fields']);
      }

      break;

    // admin/config/people/pcp
    default:
      // Provides the General Settings data
      variable_set('pcp_hide_block_on_complete', $form_state['values']['hide_block_on_complete']);
      if (variable_get('user_pictures', 0)) {
        variable_set('pcp_enable_user_picture', $form_state['values']['pcp_enable_user_picture']);
      }

      // Process the profile fields that have been modified
      if (isset($form_state['values']['profile_fields'])) {
        _pcp_save_profile_fields($form_state['values']['profile_fields']);
      }
  }
}


/**
 *  Utility function used to save fields.
 */
function _pcp_save_profile_fields($profile_fields) {
  foreach ($profile_fields as $fiid => $value) {
    if ($value == 0) {
      _pcp_disable_field($fiid);
    }
    else {
      _pcp_enable_field($fiid);
    }
  }
  drupal_set_message(t("Your settings have been saved."));
}

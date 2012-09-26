<?php

/**
 * Creates the teaser listing for the jobs page
 * that users can see.
 */
function jobs_page()
{
	global $language, $user;

	$jids = jobs_get_published_jids($language->language);
	$jobs = job_load_multiple($jids);
	$output = array();
	foreach($jobs as $job)
	{
		$form = FALSE;
		$already_applied = FALSE;
		if(in_array('Applicant', $user->roles))
		{
			$already_applied = db_query_range('SELECT timestamp FROM {job_applications} WHERE jid = :jid AND uid = :uid', 0, 1, array(':jid' => $job->jid, ':uid' => $user->uid))->fetchField();
			if(!$already_applied)
			{
				$form = drupal_render(drupal_get_form('jobs_job_application_form', $job));
			}
			else
			{
				$already_applied = format_date($already_applied);
			}
		}
		$output[] = theme('job_teaser', array('job' => $job, 'application_form' => $form, 'already_applied' => $already_applied));
	}
	
	return theme('jobs_page', array('teasers' => $output));
}

/**
 * Outputs a list of links for any any job types the user has permission
 * to access
 */
function jobs_job_add_page()
{
	$types = jobs_get_job_types();
	$items = array();
	$default = FALSE;
	foreach($types as $type)
	{
		if(user_access('create Jobs of type: ' . $type->type))
		{
			if(!$default)
			{
				$default = str_replace('_', '-', $type->type);
			}
			$items[] = l($type->label, 'job/add/' . str_replace('_', '-', $type->type));
		}
	}
	if(count($items) === 1)
	{
		drupal_goto('job/add/' . $default);
	}
	return theme('item_list', array('items' => $items));
}

/**
 * Form definition for the Job Type form, used for creating new job types
 */
function job_type_form($form, &$form_state, $job, $op = 'edit')
{
	if($op == 'clone')
	{
		$job->label .= ' ' . t('(cloned)');
		$job->name .= '_' . t('clone');
	}
	
	$form['label'] = array
	(
		'#title' => t('Label'),
		'#type' => 'textfield',
		'#default_value' => $job->label,
	);
	$form['type'] = array
	(
		'#type' => 'machine_name',
		'#default_value' => (isset($job->name)) ? $job->name : '',
		'#disabled' => ($op === 'edit') ? TRUE : FALSE,
		'#description' => t('A unique machine-readable name for this job. It can only contain lowercase letters, numbers, and underscores.'),
		'#machine_name' => array
		(
			'exists' => 'jobs_load_by_name',
			'source' => array('label'),
		),
	);
	$form['actions'] = array
	(
		'#type' => 'actions',
	);
	$form['actions']['submit'] = array
	(
		'#type' => 'submit',
		'#value' => t('Save job'),
		'#weight' => 50,
	);
	return $form;
}

/**
 * Submit function for job_type_form()
 */
function job_type_form_submit(&$form, &$form_state)
{
	$job = entity_ui_form_submit_build_entity($form, $form_state);
	$job->save();
	$form_state['redirect'] = 'admin/structure/jobs';
}

/**
 * Page callback for job/add/[JOB TYPE]
 */
function job_add($type)
{
	global $user;

	$types = jobs_get_job_types();
	if($types[$type->type])
	{
		$job = new Job(array('user' => $user->uid, 'type' => $type->type));
		drupal_set_title(t('Create @name', array('@name' => $types[$type->type]->type)), PASS_THROUGH);
		$output = drupal_get_form($type->type . '_job_form', $job);
	}
	else
	{
		return t('Error in retrieving job type');
	}
	return $output;
}

/**
 * Page callback for job/[JID]/edit
 */
function job_edit($job)
{
	$types = jobs_get_job_types();
	drupal_set_title(t('Edit @name', array('@name' => $types[$job->type]->type)), PASS_THROUGH);
	return drupal_get_form($job->type . '_job_form', $job);
}

/**
 * Form defnition allowing for creation and editing of jobs
 */
function job_form($form, &$form_state, $job)
{
	global $language;
	
	$default_language = language_default()->language;
	$site_languages = language_list();

	if($job->jid && user_access('translate jobs'))
	{
		if(count($site_languages > 1))
		{
			$form['#display_language_changed'] = (isset($form_state['values']['show_language']) && $form_state['values']['show_language']);
			if($form['#display_language_changed'])
			{
				$form['#display_language'] = $form_state['values']['show_language'];
			}

			$options[''] = array('---');
			foreach($site_languages as $code => $l)
			{
				if($code !== $language->language)
				{
					$options[$code] = $l->native;
				}
			}
			$form['show_language'] = array
			(
				'#type' => 'select',
				'#title' => t('Show values for the following language'),
				'#options' => $options,
				'#ajax' => array
				(
					'callback' => 'jobs_ajax_callback',
					'wrapper' => 'job_ajax_wrapper',
				),
			);
			$form['job'] = array
			(
				'#prefix' => '<div id="job_ajax_wrapper">',
				'#suffix' => '</div>',
			);
		}
	}

	$form['job']['label'] = array
	(
		'#title' => t('Job title'),
		'#type' => 'textfield',
		'#required' => TRUE,
		'#default_value' => (isset($job->label)) ? $job->label : '',
	);
	$form['job']['job'] = array
	(
		'#type' => 'value',
		'#value' => $job,
	);
    $form['job']['jobs_' . $job->type]['#tree'] = TRUE;
    $form['job']['jobs_' . $job->type]['#parents'] = array('jobs_' . $job->type);

	field_attach_form('job', $job, $form['job']['jobs_' . $job->type], $form_state, $language->language);
	if(count($site_languages) > 1)
	{
		if(user_access('translate jobs'))
		{
			$form['translated'] = array
			(
				'#type' => 'fieldset',
				'#title' => t('Translated languages'),
				'#description' => '<p>' . t('Check the boxes for the languages that the job has been fully translated into. The job will then be moved into the moderation queue for that language.') . '</p><p>' . t('Jobs that have been checked as translated and then published cannot be marked as untranslated until they have first been marked as unpublished.'),
				'#tree' => TRUE,
			);
			foreach($site_languages as $key => $l)
			{
				$form['translated'][$key] = array
				(
					'#type' => 'checkbox',
					'#title' => $l->name,
					'#default_value' => (isset($job->translated_languages[$key])),
					'#disabled' => (isset($job->published_languages[$key])),
				);
			}
		}
		if(user_access('moderate jobs'))
		{
			$form['published'] = array
			(
				'#type' => 'fieldset',
				'#title' => t('Published languages'),
				'#description' => '<p>' . t('Check the boxes for the languages in which this jobs should be published. The job will then show up in public-facing lists for that language.') . '</p><p>' . t('Jobs cannot be marked as published for a language until they have first been marked as translated for that language.') . '</p>',
				'#tree' => TRUE,
			);
			foreach($site_languages as $key => $l)
			{
				$form['published'][$key] = array
				(
					'#type' => 'checkbox',
					'#title' => $l->name,
					'#default_value' => (isset($job->published_languages[$key])),
					'#disabled' => (!isset($job->translated_languages[$key]))
				);
			}
		}
	}
	$form['actions'] = array('#type' => 'actions');
	$form['actions']['submit'] = array
	(
		'#type' => 'submit',
		'#value' => t('Save job'),
	);
	if (!empty($job->jid) && job_access('delete', NULL, $job))
	{
		$form['actions']['delete'] = array
		(
			'#type' => 'submit',
			'#value' => t('Delete'),
			'#weight' => 15,
			'#submit' => array('job_form_delete_submit'),
			'#limit_validation_errors' => array(),
		);
 	}
	return $form;
}

/**
 * Validation function for job_form()
 */
function job_form_validate($form, &$form_state)
{
	$job = $form_state['values']['job'];
	field_attach_form_validate('job', $job, $form['job']['jobs_' . $job->type], $form_state);
	if(isset($form_state['values']['published']))
	{
		foreach($form_state['values']['published'] as $key => $l)
		{
			if(strlen($l) && $l && (!strlen($form_state['values']['translated'][$key]) || !$form_state['values']['translated'][$key]))
			{
				form_set_error('published][' . $key, t('You cannot set the job to be published in a language it has not been translated to.'));
			}
		}
	}
}

/**
 * Submit function for job_form()
 */
function job_form_submit($form, &$form_state)
{
	$job = $form_state['values']['job'];
	$translated_languages = '';
	if(isset($form_state['values']['translated']))
	{
		foreach($form_state['values']['translated'] as $key => $l)
		{
			if(strlen($l) && $l)
			{
				if(strlen($translated_languages))
				{
					$translated_languages .= '|';
				}
				$translated_languages .= $key;
			}
		}
	}

	$published_languages = '';
	if(isset($form_state['values']['published']))
	{
		foreach($form_state['values']['published'] as $key => $l)
		{
			if(strlen($l) && $l)
			{
				if(strlen($published_languages))
				{
					$published_languages .= '|';
				}
				$published_languages .= $key;
			}
		}
	}

	$job->translated = (strlen($translated_languages)) ? $translated_languages : NULL;
	$job->published = (strlen($published_languages)) ? $published_languages : NULL;
	$job->label = $form_state['values']['label'];

	$job->save();

	$form_state['redirect'] = 'job/' . $job->jid;

	field_attach_submit('job', $job, $form['job']['jobs_' . $job->type], $form_state);
}

/*
 * Submit function for the delete button on the job edit page.
 * Redirects users to the confirm page that confirms the user wants to
 * delete the job.
 */
function job_form_delete_submit($form, &$form_state)
{
	$destination = array();
	$job = $form['job']['job']['#value'];
	$destination = (isset($_GET['destination']) && $_GET['destination']) ? $_GET['destination'] : 'jobs';
	$form_state['redirect'] = array('job/' . $job->jid . '/delete', array('query' => array('destination' => $destination)));
	
	// Note: the following is required to ensure the redirect properly occurs even when destination is set in the URL
	unset($_GET['destination']);
	drupal_static_reset('drupal_get_destination');
	drupal_get_destination();
}

/**
 * Confirm function to confirm that a user really does want to delete a specified job
 */
function job_delete_confirm($form, &$form_state, $job)
{
	$form['#job'] = $job;
	// Always provide entity id in the same form key as in the entity edit form.
	$form['jid'] = array
	(
		'#type' => 'value',
		'#value' => $job->jid
	);
	return confirm_form(
		$form,
    	t('Are you sure you want to delete the job: %title?', array('%title' => $job->label)),
    	'job/' . $job->jid,
    	t('This action cannot be undone.'),
    	t('Delete'),
    	t('Cancel')
	);
}

/**
 * Submit function for job_delete_confirm()
 *
 * Deletes the specified Job from the system
 */
function job_delete_confirm_submit($form, &$form_state)
{
	job_delete($form_state['values']['jid']);
	drupal_goto('jobs');
}

function jobs_admin_list($type)
{
	$table = render(drupal_get_form('jobs_admin_list_table_form', $type));

	return theme('jobs_page_' . $type, array($type . '_jobs_table' => $table));
}

function jobs_admin_list_table_form($form, &$form_state, $type)
{
	global $language;

	$languages = language_list();

	if(count($languages) > 1)
	{
		$options = array();
		foreach($languages as $code => $l)
		{
			$options[$code] = $l->name;
		}
		$current_language = isset($form_state['values']['language']) && strlen($form_state['values']['language']) ? $form_state['values']['language'] : $language->language;
		$form['language'] = array
		(
			'#type' => 'select',
			'#title' => t('Show !type items from the following language', array('!type' => $type)),
			'#options' => $options,
			'#default_value' => $current_language,
			'#ajax' => array
			(
				'callback' => 'jobs_ajax_callback',
				'wrapper' => 'language_ajax_wrapper',
				'effect' => 'fade',
			),
		);
		$form['display'] = array
		(
			'#prefix' => '<div id="language_ajax_wrapper">',
			'#suffix' => '</div>',
			'#markup' => jobs_build_admin_list_table($type, $current_language),
		);
	}
	else
	{
		$form['display'] = array
		(
			'#markup' => jobs_build_admin_list_table($type, $language->language),
		);
	}
	return $form;	
}

function jobs_build_admin_list_table($type, $language)
{
	$languages = language_list();

	if($type === 'untranslated')
	{
		$jobs = jobs_get_jobs('untranslated', $language);
	}
	elseif($type === 'queue')
	{
		$jobs = jobs_get_jobs('moderation_queue', $language);
	}
	elseif($type == 'published')
	{
		$jobs = jobs_get_jobs('published', $language);
	}

	$header = array
	(
		array('data' => t('Job')),
		array('data' => t('Edit')),
		array('data' => t('Type')),
		array('data' => t('Translated languages')),
		array('data' => t('Published languages')),
		array('data' => t('Created')),
		array('data' => t('Updated')),
	);
	$rows = array();
	foreach($jobs as $job)
	{
		$row = array();
		$row[] = l($job->label, 'job/' . $job->jid);
		$row[] = l(t('Edit'), 'job/' . $job->jid . '/edit', array('query' => array('destination' => 'admin/content/jobs/' . $type)));
		$row[] = t($job->type);
		$translated = '';
		foreach($job->translated_languages as $tl)
		{
			if(isset($languages[$tl]))
			{
				if(strlen($translated))
				{
					$translated .= '<br />';
				}
				$translated .= $languages[$tl]->name;
			}
		}
		if(!strlen($translated))
		{
			$translated = t('None');
		}
		$row[] = $translated;
		$published = '';
		foreach($job->published_languages as $pl)
		{
			if(isset($languages[$pl]))
			{
				if(strlen($published))
				{
					$published .= '<br />';
				}
				$published .= $languages[$pl]->name;
			}
		}
		if(!strlen($published))
		{
			$published = t('None');
		}
		$row[] = $published;
		$row[] = format_date($job->created);
		$row[] = format_date($job->changed);
		$rows[] = $row;
	}
	if(!count($rows))
	{
		$row = array();
		$row[] = array('data' => t('No jobs were found for the current language (!language)', array('!language' => $languages[$language]->name)), 'colspan' => 7);
		$rows[] = $row;
	}

	return theme('table', array('header' => $header, 'rows' => $rows));
}

function jobs_get_default_jobs_mail_notify_applicant_application_text()
{
	$text = 'Hello [applicant]

This mail is a record for your application to the job : [job_name]. You can view the details of this job at [job_link].

Regards,

[site_name]';
	return $text;
}

function jobs_get_default_jobs_mail_notify_job_owner_application_text()
{
	$text = 'Hello [owner]

A new application has been made for the job: [job_name] ([job_link]). Please log in to view the details of the application.

Regards,

[site_name]';
	return $text;
}

function jobs_get_default_jobs_mail_notify_admin_application_text()
{
	$text = 'Hello [site_name] admin

A new application has been made for the job: [job_name] ([job_link]). Please log in to view the details of the application.

Regards,

[site_name]';
	return $text;
}

<?php

function jobs_page()
{
	$jobs = job_load_multiple(FALSE);
	$output = '';
	foreach($jobs as $job)
	{
		$output .= theme('job_teaser', array('job' => $job));
	}

	return $output;
}

function jobs_job_add_page()
{
	$types = jobs_get_job_types();
	$items = array();
	$default = FALSE;
	foreach($types as $type)
	{
		if(user_access('create Job of type: ' . $type->type))
		{
			if(!$default)
			{
				$default = $type->type;
			}
			$items[] = l($type->label, 'job/add/' . $type->type);
		}
	}
	if(count($items === 1))
	{
		//drupal_goto('job/add/' . $default);
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
	$form['label'] = array
	(
		'#title' => t('Job title'),
		'#type' => 'textfield',
		'#required' => TRUE,
		'#default_value' => (isset($job->label)) ? $job->label : '',
	);
	$form['job'] = array
	(
		'#type' => 'value',
		'#value' => $job,
	);
    $form['jobs_' . $job->type]['#tree'] = TRUE;
    $form['jobs_' . $job->type]['#parents'] = array('jobs_' . $job->type);
    field_attach_form('job', $job, $form['jobs_' . $job->type], $form_state);
	$form['actions'] = array('#type' => 'actions');
	$form['actions']['submit'] = array
	(
		'#type' => 'submit',
		'#value' => t('Save job'),
	);
	if (!empty($job->jid) && job_access('delete', $job->type))
	{
		$form['actions']['delete'] = array
		(
			'#type' => 'submit',
			'#value' => t('Delete'),
			'#weight' => 15,
			'#submit' => array('job_form_delete_submit'),
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
	field_attach_form_validate('job', $job, $form['jobs_' . $job->type], $form_state);
}

/**
 * Submit function for job_form()
 */
function job_form_submit($form, &$form_state)
{
	$job = $form_state['values']['job'];
	$job->label = $form_state['values']['label'];
	$job->save();
	$form_state['redirect'] = 'job/' . $job->jid;
	field_attach_submit('job', $job, $form['jobs_' . $job->type], $form_state);
}

/*
 * Submit function for the delete button on the job edit page.
 * Redirects users to the confirm page that confirms the user wants to
 * delete the job.
 */
function job_form_delete_submit($form, &$form_state)
{
	$destination = array();
	$job = $form_state['values']['job'];
	$form_state['redirect'] = array('job/' . $job->jid . '/delete', array('query' => array('destination' => 'jobs')));
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
}

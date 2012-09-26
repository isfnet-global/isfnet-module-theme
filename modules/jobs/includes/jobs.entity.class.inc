<?php

/**
 * The class used for profile entities.
 */
class Job extends Entity {

	/**
	 * The job id.
	 *
	 * @var integer
	 */
	public $jid;

	/**
	 * The name of the job type.
	 *
	 * @var string
	 */
	public $type;

	/**
	 * The job label.
	 *
	 * @var string
	 */
	public $label;

	/**
	 * The user id of the job owner.
	 *
	 * @var integer
	 */
	public $uid;

	/**
	 * The Unix timestamp when the profile was created.
	 *
	 * @var integer
	 */
	public $created;

	/**
	 * The Unix timestamp when the profile was most recently saved.
	 *
	 * @var integer
	*/
	public $changed;
	
	/**
	 * A string of language codes into which the job has been translated,
	 * separated by a vertical pipe
	 *
	 * @var string
	*/
	public $translated;
	
	/**
	 * An array of languages into which the job has been translated
	 *
	 * @var string
	*/
	public $translated_languages;
	
	/**
	 * A string of language codes in which the job has been published,
	 * separated by a vertical pipe
	 *
	 * @var string
	*/
	public $published;
	
	/**
	 * An array of language codes in which the job has been published,
	 *
	 * @var array
	*/
	public $published_languages;
	
	/**
	 * A Boolean indicating the status of the job
	*/
	public $status;


	public function __construct($values = array())
	{
		if (isset($values['user']))
		{
			$this->setUser($values['user']);
			unset($values['user']);
		}
		if (isset($values['type']) && is_object($values['type']))
		{
			$values['type'] = $values['type']->type;
		}
		$this->translated_languages = array();
		$this->published_languages = array();

		parent::__construct($values, 'job');
	}

	/**
	* Returns the user owning this profile.
	*/
	public function user()
	{
		return user_load($this->uid);
	}

	/**
	* Sets a new user owning this profile.
	*
	* @param $account
	*   The user account object or the user account id (uid).
	*/
	public function setUser($account)
	{
		$this->uid = is_object($account) ? $account->uid : $account;
	}

	/**
	 * Gets the associated profile type object.
	 *
	 * @return JobType
	 */
	public function type()
	{
		return jobs_get_job_types($this->type);
	}

	/**
	 * Returns the full url() for the job.
	 */
	public function url()
	{
		$uri = $this->uri();
		return url($uri['path'], $uri);
	}

	/**
	 * Returns the drupal path to this job type.
	 */
	public function path()
	{
	    $uri = $this->uri();
		return $uri['path'];
	}

	public function defaultUri()
	{
		return array
		(
			'path' => 'job/' . $this->jid,
			'options' => array('fragment' => 'job-' . $this->type),
		);
	}
	
	public function buildContent($view_mode = 'full', $langcode = NULL)
	{
		$content = array();
		// Assume newly create objects are still empty.
		if (!empty($this->is_new))
		{
			$content['empty']['#markup'] = '<em class="job-no-data">' . t('There is no job data yet.') . '</em>';
		}
		return entity_get_controller($this->entityType)->buildContent($this, $view_mode, $langcode, $content);
	}
	
	public function setUp()
	{
		global $language;
		
		if(strlen($this->translated))
		{
			$languages = explode('|', $this->translated);
			foreach($languages as $code)
			{
				$this->translated_languages[$code] = $code;
			}
		}

		if(strlen($this->published))
		{
			$languages = explode('|', $this->published);
			foreach($languages as $code)
			{
				$this->published_languages[$code] = $code;
			}
		}

		$result = db_query('SELECT language, label FROM {jobs_titles} WHERE jid = :jid', array(':jid' => $this->jid));
		while($r = $result->fetch())
		{
			$this->translated_labels[$r->language] = $r->label;
		}
		if(isset($this->translated_labels[$language->language]))
		{
			$this->label = $this->translated_labels[$language->language];
		}
		elseif($this->jid)
		{
			$this->label = t('No Title');
		}
	}

	public function save()
	{
		global $language;

		// Care about setting created and changed values. But do not automatically
		// set a created values for already existing profiles.
		if(!empty($this->is_new) || !$this->jid)
		{
			if(empty($this->created))
			{
				$this->created = REQUEST_TIME;
			}
			if(empty($this->translated))
			{
				$this->translated = '';
			}
			if(empty($this->published))
			{
				$this->published = '';
			}
			if(empty($this->status))
			{
				$this->status = 0;
			}
		}

		$this->changed = REQUEST_TIME;
		
		parent::save();

		db_query
		(
			'INSERT INTO {jobs_titles} (jid, language, label) VALUES (:jid, :language, :label) ON DUPLICATE KEY UPDATE label = :label',
			array(':jid' => $this->jid, ':language' => $language->language, ':label' => $this->label)
		);

		// Update the static cache from jobs_load_by_user().
		$cache = &drupal_static('jobs_load_by_user', array());
		if (isset($cache[$this->uid]))
		{
			$cache[$this->uid][$this->type] = $this->jid;
		}
		menu_cache_clear_all();
	}
}

/**
 * Use a separate class for profile types so we can specify some defaults
 * modules may alter.
 */
class JobType extends Entity {

	public $type;
	public $label;
	public $weight = 0;
	public $status;

	public function __construct($values)
	{
		parent::__construct($values, 'job_type');
	}

	/**
	 * Returns whether the job type is locked, thus may not be deleted or renamed.
	 *
	 * Job types provided in code are automatically treated as locked, as well
	 * as any fixed job type.
	 */
	public function isLocked()
	{
		return isset($this->status) && empty($this->is_new) && (($this->status & ENTITY_IN_CODE) || ($this->status & ENTITY_FIXED));
	}
}

/**
 * UI controller.
 */
class JobTypeUIController extends EntityDefaultUIController
{
	/**
	 * Overrides hook_menu() defaults.
	 */
	public function hook_menu()
	{
		$items = parent::hook_menu();
		$items[$this->path]['description'] = 'Manage job types and fields.';
		return $items;
	}
}


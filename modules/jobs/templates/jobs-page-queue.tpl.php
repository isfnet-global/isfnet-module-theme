<?php global $language; ?>

<h1><?php print t('Moderation queue (!language)', array('!language' => $language->name)); ?></h1>
<p><?php print t('Below is a list of all jobs that have not yet been published in !language. To publish a job, click the edit link, and fill in the !language values. Check the <strong>!language box</strong> under <strong>Published</strong> and click <strong>save</strong>.', array('!language' => $language->name)); ?></p>
<p><?php print t('For a list of jobs unpublished in other languages, please visit this page in that language.'); ?></p>

<?php print $queue_jobs_table; ?>
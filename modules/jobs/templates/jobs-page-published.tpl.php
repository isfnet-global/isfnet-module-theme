<?php global $language; ?>

<h1><?php print t('Published Jobs (!language)', array('!language' => $language->name)); ?></h1>
<p><?php print t('Below is a list of all jobs that have been published in !language.', array('!language' => $language->name)); ?></p>
<p><?php print t('For a list of published jobs in other languages, please visit this page in that language.'); ?></p>

<?php print $published_jobs_table; ?>
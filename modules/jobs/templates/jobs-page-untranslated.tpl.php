<?php global $language; ?>

<h1><?php print t('Untranslated jobs (!language)', array('!language' => $language->name)); ?></h1>
<p><?php print t('Below is a list of all jobs that have not yet been translated into !language. To translate a job, click the edit link, and fill in the !language values. Check the <strong>!language box</strong> under <strong>Languages</strong> and click <strong>save</strong>.', array('!language' => $language->name)); ?></p>
<p><?php print t('For a list of jobs untranslated in other languages, please visit this page in that language.'); ?></p>

<?php print $untranslated_jobs_table; ?>
<?php global $language; ?>
<a name="#job-<?php print $job->jid; ?>"></a>
<div id="job_<?php print $job->jid; ?>" class="popup popup-data">
	<div class="exit"></div>
	<h3 class="popup_title" ><?php print l($job->label, 'job/' . $job->jid); ?></h3>
	<div class="job_popup_ct">
	<?php if($application_form): ?>
		<?php print $application_form; ?>
	<?php elseif($already_applied): ?>
		<p><?php print t('You applied for this job on !date', array('!date' => $already_applied)); ?></p>
	<?php endif; ?>
	<?php foreach(field_attach_view('job', $job, 'Full', $language->language) as $field): ?>
		<?php print drupal_render($field); ?>
	<?php endforeach; ?>
	</div> <!-- END. job_popup_ct  -->
</div>

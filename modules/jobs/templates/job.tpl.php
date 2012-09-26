<script type="text/javascript">
jQuery(window).load(function() {
	jQuery('.field-name-field-job-salary-start, .field-name-field-job-salary-end, .field-name-field-job-city, .field-name-field-job-location').removeClass('clearfix');
});
</script>
<div id="job_<?php print $job->jid; ?>" class="job"></div>
	<?php if($application_form): ?>
		<?php print $application_form; ?>
	<?php elseif($already_applied): ?>
		<p><?php print t('You applied for this job on !date', array('!date' => $already_applied)); ?></p>
	<?php endif; ?>
	<?php
//		echo '<pre>';
//		print_r(field_attach_view('job', $job, 'Full'));
//		echo '</pre>';
		$fields_arr = field_attach_view('job', $job, 'Full');
		$new_arr    = array();	
		foreach($fields_arr as $i => $field): ?>
			<?php		
			if ($i == 'field_job_description') {
				$new_arr[0] = drupal_render($field);				
			} 
			if ($i == 'field_job_responsibilities') {
				$new_arr[1] = drupal_render($field);				
			} 		
			if ($i == 'field_job_type') {
				$new_arr[2] = drupal_render($field);				
			} 			 
			if ($i == 'field_job_type') {
				$new_arr[2] = drupal_render($field);				
			} 	
			if ($i == 'field_job_status') {
				$new_arr[3] = drupal_render($field);				
			} 
			if ($i == 'field_job_status') {
				$new_arr[3] = drupal_render($field);				
			}   
			if ($i == 'field_job_duration') {
				$new_arr[4] = drupal_render($field);				
			} 
			if ($i == 'field_job_category') {
				$new_arr[5] = drupal_render($field);				
			} 
			if ($i == 'field_job_location') {
				$new_arr[6] = drupal_render($field);				
			} 
			if ($i == 'field_job_city') {
				$new_arr[7] = drupal_render($field);				
			} 
			if ($i == 'field_job_salary_start') {				
				$new_arr[8] = drupal_render($field);				
			} 
			if ($i == 'field_job_salary_end') {
				$new_arr[9] = drupal_render($field);							
			} 
			if ($i == 'field_job_start_date') {
				$new_arr[10] = drupal_render($field);				
			} 
			if ($i == 'field_job_working_hours') {
				$new_arr[11] = drupal_render($field);				
			} 
			if ($i == 'field_other_requirements') {
				$new_arr[12] = drupal_render($field);				
			} 
			if ($i == 'field_new_language_skills') {
				$new_arr[13] = drupal_render($field);				
			} 
			?>
		<?php endforeach; ?>
		<?php 			
			ksort($new_arr);
			foreach ($new_arr as $i => $item) {
				if ($i == 8) { echo "<div class='cfix'></div>"; }
				if ($i == 10) { echo "<div class='cfix'></div>"; }
				echo $item;
				
			}
		?>		
	<?php if($applicant_table): ?>
		<h2><?php print t('Applicants'); ?></h2>
		<?php print $applicant_table; ?>
	<?php endif; ?>
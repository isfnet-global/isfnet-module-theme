(function ($) { 
	
	/* ONLOAD PAGE */
	$(document).ready(function(){
		$('body').append('<div class="popup opacity-provider" id="background"></div>');

		
		// -----------------------------
		
	});
	
	$("#show").live("click", function(){ 
		$("div.popup").fadeIn();
	    return false;
	});
	//End of gif related example. Put your code between these comments;
	
	// CLICK JOB title
	$(".Job_selector").live("click", function() {
		//$("div.popup").fadeOut();
		var clicked_id = $(this).attr('id');
		var job = "#job_";
		var job_id = job.concat(clicked_id);
		// ------------------------
		$("#background").fadeIn();
		$(job_id).fadeIn();		
		$.scrollTo('104px', 800);	
		//---------------------------------
		$("#background").click( function(e) {
			$(".exit").click();
		});
		return false;
	});
	
	
	/// CLICK EXIT
	$(".exit").live("click", function() {
		$("div.popup").fadeOut();
	});

})(jQuery);

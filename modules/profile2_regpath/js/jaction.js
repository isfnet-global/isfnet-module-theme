/**
 * ACTION JAVAscript. 
 * Control applicant/register page
 */
(function ($) {

//-------------------------
/* ONLOAD PAGE */
$(document).ready(function(){
	var f_name = $("#edit-profile-applicant-field-user-first-name-en-0-value").val();
	if (f_name != "") {
		$("#profile2_applicant_form_group_personal_info").css("display", "none");
		$("#profile2_applicant_form_group_complete").css("display", "block");
		var ele = $(".step_2");
		ele.innerHTML = ele.innerHTML + "<em> Ok </em>";
 	}

	hidePreButton();
	hideCaptCha();
	getEventButton('.multipage-link-next');
	getEventButton('.multipage-link-previous');
	// Progess bar ----------------------------------
	addProgessBar();	
	// Validate -------------------------------------

	$('.description').css('visibility','hidden');
	addClassValidate();
	$('#edit-profile-applicant-field-disclaimer .description').css('visibility','visible');
	// $('#edit-mollom-captcha').next('.description').css('visibility','visible');
	$('#profile2_applicant_form_group_skills').next('.description').css('visibility','visible');
	// LAST STEP 
	clickSubM(); 
	// --------- 
	showStatusStep(); 
 });

$("#edit-profile-applicant-field-user-country-en").live("change", function() {
  var country = $("#edit-profile-applicant-field-user-country-en").attr("selectedIndex");
  var nationality = $("#edit-profile-applicant-field-country-of-residence-en").attr("selectedIndex");
  if ( parseInt(country) == parseInt(nationality)) {
  	$("#edit-profile-applicant-field-user-visa").css("display", "none");
  } else {
	$("#edit-profile-applicant-field-user-visa").css("display", "block");
  }
});

$("#edit-profile-applicant-field-country-of-residence-en").live("change", function() {
  var country = $("#edit-profile-applicant-field-user-country-en").attr("selectedIndex");
  var nationality = $("#edit-profile-applicant-field-country-of-residence-en").attr("selectedIndex");
  if ( parseInt(country) == parseInt(nationality)) {
        $("#edit-profile-applicant-field-user-visa").css("display", "none");
  } else {
        $("#edit-profile-applicant-field-user-visa").css("display", "block");
  }
});



/* ------------------------------------------------------------ */
/* 
 * After submit, but have a error in any step.  
 */
function showStatusStep() {
	 var $panes = $('.multipage-pane');
	 var cur_step;
	 $panes.each(function () {		
		 cur_step = parseInt($(this).attr('step'));
		 if ($(this).css('display') != 'block') {		 
			 $('.step.step_'+(cur_step)).append('<em>OK</em>');
			 $(".step.step_"+ (cur_step+1) +", .step.step_"+ (cur_step) +" .line_to_step").addClass('active');
		 } else {
			 if (cur_step > 1) { 
				 $(".step.step_"+ (cur_step+1) +", .step.step_"+ (cur_step) +" .line_to_step").addClass('active');
			 }
			 return false;
		 }		 
	 });
} 


// PREBUTTON --------------------------
/*
 * FUNCTION: hidePreButton 
 * get event and do something.
 * Params ----------------------------
 *  @  nameClass : class name of object 
*/
function getEventButton(nameClass) { 
	var objNext     = $(nameClass);
	objNext.click(function() {
		hidePreButton();
		hideCaptCha();
	});
}
/*
 * FUNCTION: hidePreButton
 * Action show or hide Previous Button.
 */
function hidePreButton() {
	var hideorshow = jQuery('#profile2_applicant_form_group_personal_info').css('display');
	var objPre     = $('.multipage-link-previous');
	if (hideorshow == 'block') {
		objPre.hide();	
	} else {
		objPre.show();	
	}
} 

//CAPTCHA ------------------------------------------------------------

/*
 * FUNCTION: hideCaptCha
 * Action show or hide CaptCha.
 */
function hideCaptCha() {
	var hideorshow_captcha = jQuery('#profile2_applicant_form_group_complete').css('display');
	var objCaptCha         = $('.form-type-textfield.form-item-mollom-captcha');
	if (hideorshow_captcha == 'none') {
		objCaptCha.hide();	
	} else {
		objCaptCha.show();	
	}
} 

// PROGESS BAR ------------------------------------------------------------

/*
 * FUNCTION: addProgessBar
 * Action add indicator - progress bar.
 */
function addProgessBar() {
	var div_wrap_ProgessBar = jQuery('.group_div_above_multipage');
	var HTML_progessBar     = '<div class="step_wrap progess_bar">';
		HTML_progessBar     +=  	'<div class="step step_1 active">';
		HTML_progessBar     +=  		'<span class="step_no">1</span>';
		HTML_progessBar     +=			'<p class="step_desc">Personal Information</p>';					
		HTML_progessBar     +=			'<div class="line_to_step"></div>';				
		HTML_progessBar     +=		'</div>';
		HTML_progessBar     +=  	'<div class="step step_2">';
		HTML_progessBar     +=  		'<span class="step_no">2</span>';
		HTML_progessBar     +=			'<p class="step_desc">Education Details</p>';					
		HTML_progessBar     +=			'<div class="line_to_step"></div>';				
		HTML_progessBar     +=		'</div>';
		HTML_progessBar     +=  	'<div class="step step_3">';
		HTML_progessBar     +=  		'<span class="step_no">3</span>';
		HTML_progessBar     +=			'<p class="step_desc">Work Details</p>';					
		HTML_progessBar     +=			'<div class="line_to_step"></div>';				
		HTML_progessBar     +=		'</div>';
		HTML_progessBar     +=  	'<div class="step step_4">';
		HTML_progessBar     +=  		'<span class="step_no">4</span>';
		HTML_progessBar     +=			'<p class="step_desc">Skill & Interest</p>';					
		HTML_progessBar     +=			'<div class="line_to_step"></div>';				
		HTML_progessBar     +=		'</div>';					
		HTML_progessBar     +=  	'<div class="step step_5">';
		HTML_progessBar     +=  		'<span class="step_no">5</span>';
		HTML_progessBar     +=			'<p class="step_desc">Finish</p>';					
			
		HTML_progessBar     +=		'</div>';		
		
		HTML_progessBar     +=		'<div class="clearfix"></div>';		
		HTML_progessBar     +=	'</div><!-- END: .step_wrap -->';	
						
	div_wrap_ProgessBar.append(HTML_progessBar);	
} 

// Validate Form -----------------------------------------------------------------------

/*
 * FUNCTION: addClassValidate
 * Add more class of validation when onload Page
 */
function addClassValidate() {

	var myRadioWarp = new Array($('#edit-profile-applicant-field-gender-en')
								,$('#edit-profile-applicant-field-contact-en-0-field-contact-type-en'));
	
		$('#edit-name, #edit-mail').next('.description').css({'visibility':'visible'});
		$('input, textarea, select').blur(function() {
			 $(this).css({'border' : '1px solid #ccc'});
			 if ($(this).next('.description').length > 0) { // OK exist description message
				 if ($(this).attr('id') == 'edit-name' || $(this).attr('id') == 'edit-mail' ) {
					 	$(this).next('.description').css('color','#272525');
					} else {
						$(this).next('.description').css({'visibility':'hidden'});
					}				 
			 } 
			 $('#edit-profile-applicant-field-user-dob-en-0-value').next('.description').css({'visibility':'hidden'});
			 $.each(myRadioWarp, function(index, objWarp) { 
				 resetRadioBox(objWarp); 
			 });			
		});	
		$('#edit-profile-applicant-field-disclaimer-en:checkbox').blur(function() {
			$('#edit-profile-applicant-field-disclaimer-en:checkbox').next('label').css('color','#272525');
		});
		$('#edit-profile-applicant-field-user-resume-und-0-upload').blur(function() {
		 $('#edit-profile-applicant-field-user-resume-und-0-upload').parent('div').prev('label').css('color','#272525');
		});
} // END Function

function resetRadioBox(objR) {
	// Check radio	
	objR.resetError =  function() {
		if (this.next('.description').length > 0) { // OK exist description message
			this.next('.description').css({'visibility':'hidden'});
		} 
		if (this.prev('label').length > 0) { // OK exist label
			this.prev('label').css('color','#272525');
		}
	}
	if (objR.find('input:radio').length > 0) {
		objR.resetError();	
	}
	
} 

function clickSubM() {
	
		$('#user-register-form').submit(function() {
			// checking here
			if (!$('#edit-profile-applicant-field-disclaimer-en:checkbox:checked').val() == 1 
					|| $('#edit-profile-applicant-field-user-resume-und-0-upload').val() == '' 
					|| $('#edit-mollom-captcha').val() == '') {
				if (!$('#edit-profile-applicant-field-disclaimer-en:checkbox:checked').val() == 1){
					$('#edit-profile-applicant-field-disclaimer-en:checkbox').next('label').css('color','red');
				}
				if ($('#edit-profile-applicant-field-user-resume-und-0-upload').val() == ''){
					$('#edit-profile-applicant-field-user-resume-und-0-upload').parent('div').prev('label').css('color','red');
				}
				if ($('#edit-mollom-captcha').val() == '') {
					 $('#edit-mollom-captcha').css('border','1px solid red');
					 $('#edit-mollom-captcha').next('.description').css('color','red');
				}
				return false;
			}
			return true;
		});		
	
}

// ------------------------------------------------------------------------
})(jQuery); 

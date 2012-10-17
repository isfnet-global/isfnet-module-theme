(function ($) {

/**
 * This script transforms a set of wrappers into a stack of multipage pages. 
 * Another pane can be entered by clicking next/previous.
 *
 */
Drupal.behaviors.MultiPage = {
  attach: function (context) {
    $('.multipage-panes', context).once('multipage', function () {

      var focusID = $(':hidden.multipage-active-control', this).val();
      var paneWithFocus;

      // Check if there are some wrappers that can be converted to multipages.
      var $panes = $('> div.field-group-multipage', this);
      var $form = $panes.parents('form');
      if ($panes.length == 0) {
        return;
      }

      // Create the next/previous controls.
      var $controls;
      var counter = 1;
      // Transform each div.multipage-pane into a multipage with controls.
      $panes.each(function () {
    	 
    	// Override  here - MINH 09 05 2012  
    	$(this).attr('step',counter);
    	counter  ++;
    	// END ---------------------------
        $controls = $('<div class="multipage-controls-list"></div>');
        $(this).append('<div class="multipage-controls clearfix"></div>').append($controls);
        
        // Check if the submit button needs to move to the latest pane.
        if (Drupal.settings.field_group.multipage_move_submit && $('.form-actions').length) {
          $('.form-actions', $form).remove().appendTo($($controls, $panes.last()));
        }
       
        var multipageControl = new Drupal.multipageControl({
          title: $('> .multipage-pane-title', this).text(),
          wrapper: $(this),
          has_next: $(this).next().length,
          has_previous: $(this).prev().length
        });
        
        $controls.append(multipageControl.item);
        $(this)
          .addClass('multipage-pane')
          .data('multipageControl', multipageControl);

        if (this.id == focusID) {
          paneWithFocus = $(this);
        }
        
      });
      
      if (!paneWithFocus) {
        // If the current URL has a fragment and one of the tabs contains an
        // element that matches the URL fragment, activate that tab.
        if (window.location.hash && $(window.location.hash, this).length) {
          paneWithFocus = $(window.location.hash, this).closest('.multipage-pane');
        }
        else {
          paneWithFocus = $('multipage-open', this).length ? $('multipage-open', this) : $('> .multipage-pane:first', this);
        }
      }
      if (paneWithFocus.length) {
        paneWithFocus.data('multipageControl').focus();
      }
      
    });
    
  }
};

/**
 * The multipagePane object represents a single div as a page.
 *
 * @param settings
 *   An object with the following keys:
 *   - title: The name of the tab.
 *   - wrapper: The jQuery object of the <div> that is the tab pane.
 */
Drupal.multipageControl = function (settings) {
  var self = this;
  $.extend(this, settings, Drupal.theme('multipage', settings));
  var cur_step       = checkCurrentStep();
  var id_of_stepwrap = '';
 
  this.nextLink.click(function () {	  	
	    var resultStep;
	    // ------- Click NEXT -----------
	    cur_step = parseInt(checkCurrentStep());
	   
	    // id of current step
	    id_of_stepwrap = $(this).closest('div.field-group-multipage').attr('id');
	    resultStep = checkOuterForm(cur_step, id_of_stepwrap);
	    if (resultStep) {
	    	nextStep_formone(cur_step);
	    	self.nextPage(); // show next step
	    } 	
	    // -- GET value and next page or not -------------------	       	
	    return false;
	  });
  this.previousLink.click(function () {
	    self.previousPage();
	    // ------------------
	    cur_step = parseInt(checkCurrentStep());
	    preStep_formone(cur_step);	   
	    return false;
	});
  
/*
  // Keyboard events added:
  // Pressing the Enter key will open the tab pane.
  this.nextLink.keydown(function(event) {
    if (event.keyCode == 13) {
      self.focus();
      // Set focus on the first input field of the visible wrapper/tab pane.
      $("div.multipage-pane :input:visible:enabled:first").focus();
      return false;
    }
  });

  // Pressing the Enter key lets you leave the tab again.
  this.wrapper.keydown(function(event) {
    // Enter key should not trigger inside <textarea> to allow for multi-line entries.
    if (event.keyCode == 13 && event.target.nodeName != "TEXTAREA") {
      // Set focus on the selected tab button again.
      $(".multipage-tab-button.selected a").focus();
      return false;
    }
  });
*/
};

Drupal.multipageControl.prototype = {
    
  /**
   * Displays the tab's content pane.
   */
  focus: function () {
    this.wrapper
      .show()
      .siblings('div.multipage-pane')
        .each(function () {
          var tab = $(this).data('multipageControl');
          tab.wrapper.hide();
        })
        .end()
      .siblings(':hidden.multipage-active-control')
        .val(this.wrapper.attr('id'));
    // Mark the active control for screen readers.
    $('#active-multipage-control').remove();
    this.nextLink.append('<span id="active-multipage-control" class="element-invisible">' + Drupal.t('(active page)') + '</span>');
  },
  
  /**
   * Continues to the next page or step in the form.
   */
  nextPage: function () {
    this.wrapper.next().data('multipageControl').focus();    
  },
  
  /**
   * Returns to the previous page or step in the form.
   */
  previousPage: function () {
    this.wrapper.prev().data('multipageControl').focus();
  },

  /**
   * Shows a horizontal tab pane.
   */
  tabShow: function () {
    // Display the tab.
    this.item.show();
    // Update .first marker for items. We need recurse from parent to retain the
    // actual DOM element order as jQuery implements sortOrder, but not as public
    // method.
    this.item.parent().children('.multipage-control').removeClass('first')
      .filter(':visible:first').addClass('first');
    // Display the wrapper.
    this.wrapper.removeClass('multipage-control-hidden').show();
    // Focus this tab.
    this.focus();
    return this;
  },

  /**
   * Hides a horizontal tab pane.
   */
  tabHide: function () {
    // Hide this tab.
    this.item.hide();
    // Update .first marker for items. We need recurse from parent to retain the
    // actual DOM element order as jQuery implements sortOrder, but not as public
    // method.
    this.item.parent().children('.multipage-control').removeClass('first')
      .filter(':visible:first').addClass('first');
    // Hide the wrapper.
    this.wrapper.addClass('horizontal-tab-hidden').hide();
    // Focus the first visible tab (if there is one).
    var $firstTab = this.wrapper.siblings('.multipage-pane:not(.multipage-control-hidden):first');
    if ($firstTab.length) {
      $firstTab.data('multipageControl').focus();
    }
    return this;
  }
};

/**
 * Theme function for a multipage control.
 *
 * @param settings
 *   An object with the following keys:
 *   - title: The name of the tab.
 * @return
 *   This function has to return an object with at least these keys:
 *   - item: The root tab jQuery element
 *   - nextLink: The anchor tag that acts as the clickable area of the control
 *   - nextTitle: The jQuery element that contains the group title
 *   - previousLink: The anchor tag that acts as the clickable area of the control
 *   - previousTitle: The jQuery element that contains the group title
 */
Drupal.theme.prototype.multipage = function (settings) {
  var controls = {};
  controls.item = $('<span class="multipage-button"></span>');
  controls.item.append(controls.nextLink = $('<input type="button" class="form-submit multipage-link-next" value="" />').val(controls.nextTitle = Drupal.t('Next page')));
  controls.item.append(controls.previousLink = $('<a class="multipage-link-previous" href="#"></a>'));
  if (!settings.has_next) {
    controls.nextLink.hide();
  }
  if (settings.has_previous) {
    controls.previousLink.append(controls.previousTitle = $('<strong></strong>').text(Drupal.t('Previous')));
  }
  
  return controls;
};


Drupal.FieldGroup = Drupal.FieldGroup || {};
Drupal.FieldGroup.Effects = Drupal.FieldGroup.Effects || {};

/**
 * Implements Drupal.FieldGroup.processHook().
 */
Drupal.FieldGroup.Effects.processMultipage = {
  execute: function (context, settings, type) {
    if (type == 'form') {
      // Add required fields mark to any element containing required fields
      $('div.multipage-pane').each(function(i){
        if ($('.error', $(this)).length) {
          Drupal.FieldGroup.setGroupWithfocus($(this));
          $(this).data('multipageControl').focus();
        }
      });
    }
  }
}

/* ---------------- Extra LIB by MINHNGUYEN ------------------- */

//DATE VALIDATION ------------------------------------------------------
/* FUNCTION: Check that the applicant's age is over 18 years old
 *
 */


function checkMinimumAge() {
        var year = $('#edit-profile-applicant-field-user-dob-en-0-value-year').val();
        var month = parseBelowTen($('#edit-profile-applicant-field-user-dob-en-0-value-month').val());
        var day = parseBelowTen($('#edit-profile-applicant-field-user-dob-en-0-value-day').val());
        var date_string = String(String(year).concat("-").concat(String(month)).concat("-").concat(String(day)));
        var selected_date = new Date(date_string);
        var current_date = new Date();
        var diff = new Date(current_date - selected_date);

        var days_diff = diff/1000/60/60/24;
        var age = days_diff/365;

          if (age < 18 || isNaN(age)) {
                return false;
        } else {
                return true;
        }
}

// HELPER FUNCTION TO PARSE STRING TO INT ------------------------------
function parseBelowTen(num) {
  var monthInt = parseInt(String(num));
  if (monthInt < 10) {
    return "0".concat(num);
  } else {
    return num;
  }
}


/**
* function checkOuterForm()
* check another type the validate lib cannt support. 
* Note: Can put more Params here 
* @ id_of_stepwrap (int) : id of the current step form
* @return True/False, for next step.
*/
function checkOuterForm(cur_step, id_of_stepwrap) {	
	var stuff_error = [], resultStep = true, target_top;
	// in Step 1
	if (cur_step == 1) {	
		stuff_error = [];
		
		if ($(location).attr('href').indexOf("user") < 0) {
			if (!checkTxtBox($('#edit-name'),'username')){stuff_error.push(false);}
			if (!checkTxtBox($('#edit-mail'),'email')){stuff_error.push(false);}
			
		}
		if (!checkTxtBox($('#edit-profile-applicant-field-user-first-name-en-0-value'),'textbox')){stuff_error.push(false);}
		if (!checkTxtBox($('#edit-profile-applicant-field-user-last-name-en-0-value'),'textbox')){stuff_error.push(false);}
		// Date of Birth
		if (!checkTxtBox($('#edit-profile-applicant-field-user-dob-en-0-value-month'),'selectbox')
				|| !checkTxtBox($('#edit-profile-applicant-field-user-dob-en-0-value-day'),'selectbox')
				|| !checkTxtBox($('#edit-profile-applicant-field-user-dob-en-0-value-year'),'selectbox') 
				|| !checkMinimumAge()) {
			stuff_error.push(false);
			$('#edit-profile-applicant-field-user-dob-en-0-value').next('.description').css({'color':'red','visibility':'visible'});
		}
		// POST code
		if (!checkTxtBox($('#edit-profile-applicant-field-user-city-en-0-value'),'textbox')){stuff_error.push(false);}
		
		// Address
		if (!checkTxtBox($('#edit-profile-applicant-field-user-address-en-0-value'),'textbox')){stuff_error.push(false);}
		// Contact Number 
		if (!checkTxtBox($('#edit-profile-applicant-field-contact-en-0-field-contact-number-en-0-value'),'textbox')){stuff_error.push(false);}
		// Gender
		if (!checkRadioBox($('#edit-profile-applicant-field-gender-en'))){stuff_error.push(false);}
		// Contact Type
		if (!checkRadioBox($('#edit-profile-applicant-field-contact-en-0-field-contact-type-en'))){stuff_error.push(false);}
		// END Check radio edit-account
		if (stuff_error.length > 0) { resultStep = false; }	
	} else if (cur_step == 2) {
		stuff_error = [];
		if (!checkTxtBox($('#edit-profile-applicant-field-education-school-und-0-field-education-name-und-0-value'),'textbox')){stuff_error.push(false);}
		if (!checkTxtBox($('#edit-profile-applicant-field-education-school-und-0-field-education-qualification-und'),'selectbox')){stuff_error.push(false);}
		if (!checkTxtBox($('#edit-profile-applicant-field-education-school-und-0-field-program-name-und-0-value'),'textbox')){stuff_error.push(false);}
		
		if (stuff_error.length > 0) { resultStep = false; }	
	} else if (cur_step == 3) {
		stuff_error = [];
		if (!checkTxtBox($('#edit-profile-applicant-field-employment-collection-und-0-field-work-employer-und-0-value'),'textbox')){stuff_error.push(false);}
		if (!checkTxtBox($('#edit-profile-applicant-field-employment-collection-und-0-field-work-location-und'),'selectbox')){stuff_error.push(false);}
		if (!checkTxtBox($('#edit-profile-applicant-field-employment-collection-und-0-field-work-summary-und-0-value'),'textbox')){stuff_error.push(false);}
		if (stuff_error.length > 0) { resultStep = false; }	
	} 
	if ($(location).attr('href').indexOf("user") < 0) {
		target_top = $('#edit-name').offset().top;		
	} else {
		target_top = $('#profile2_applicant_form_group_personal_info').offset().top;
	}	
	$('html, body').animate({scrollTop:target_top}, 500);
	return resultStep;
}


/**
* function checkRadioBox()
* check Radios. 
* Params:
* @ objR (json) : obj warp radios group
* @ typeValid     : username, email, textbox,selectbox
* @ cusValid      : json data write a function   
* @return True/False, for next step.
*/
function checkRadioBox(objR) {
	// Check radio
	var resultCheckInput = true;
	checkRadio = objR.find('input:radio:checked').val();
	objR.showError =  function() {
		if (this.next('.description').length > 0) { // OK exist description message
			this.next('.description').css({'color':'red','visibility':'visible'});
		} 
		if (this.prev('label').length > 0) { // OK exist label
			this.prev('label').css('color','red');
		}
	}
	// validate ..................
	if (checkRadio == undefined) {		
		objR.showError();		
		resultCheckInput = false;	
	} 
	return resultCheckInput;
} 
/**
* function checkTxtBox()
* check validate, using for textbox, select box. 
* Params:
* @ objTXT (json) : obj is valided 
* @ typeValid     : username, email, textbox,selectbox
* @ cusValid      : json data write a function   
* @return True/False, for next step.
*/
function checkTxtBox(objTXT, typeValid, cusValid ) {
	var resultCheckInput = true;
	if (objTXT) {
		var valObj = jQuery.trim(objTXT.val());
		var rule;
		objTXT.showError =  function() {
			this.css('border','1px solid red');
			if (this.next('.description').length > 0) { // OK exist description message
				if (this.attr('id') == 'edit-name' || this.attr('id') == 'edit-mail' ) {
					this.next('.description').css({'color':'red'});
				} else {
					this.next('.description').css({'color':'red','visibility':'visible'});
				}
			} 
		}
		// emailFilter.test(value)
		if (typeValid) {
			switch(typeValid) {
				case 'username':
				  rule = /^[a-z0-9_-]{3,16}$/;
				  resultCheckInput = rule.test(valObj);		
				  break;
				case 'email':
				  rule = /^[a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\.[a-zA-Z]{2,4}$/;
				  resultCheckInput = rule.test(valObj);				 
				  break;
				case 'textbox':
				  if (valObj.length < 3) {
					  resultCheckInput = false;					  
				  }
				  break;
				case 'selectbox':
				  if (valObj.length == 0 || valObj == 0 || valObj == '_none') {
					  resultCheckInput = false;					  
				  }
				  break;  
			}
		} else { // use custom Valid
			  if ( cusValid instanceof Object && cusValid.valid instanceof Function ) {
				  // run function here
				  resultCheckInput = cusValid.valid(valObj);
			  }
		} // END: typeValid
		if (!resultCheckInput) {  objTXT.showError();  }
		return resultCheckInput;
		 		
	} else {
		alert('The Object is validated, it does not exist! Pls check again.');	
		return;
	}	
}



/**
* function insertMesBox()
* insert Message Box 
* Note: Can put more Params here 
* @ mes (string)           : message you want to say
* @ objShow (json, object) : obj you want to show mesage
* @ topEx  (interger - px) : top position you want to add for message
* @ leftEx (interger - px) : left position you want to add for message
* @return HTML, for obj you want to show message.
*/
function insertMesBox(mes, objShow, topEx, leftEx, wMes ) {

	//alert ( + " - " +  $('.group_div_above_multipage').offset().left);
	if (wMes) wMes = 300;
	var htmlShow  = '<div style="top: '+ (objShow.offset().top + topEx - $('.group_div_above_multipage').offset().top) +'px; '
							  +' left: '+ (objShow.offset().top + leftEx - $('.group_div_above_multipage').offset().left) +'px; '
							  +' opacity: 1; '
							  +'width: '+ wMes +'px;  " class="messbox">';
	    htmlShow += 	'<div style="height:12px;" class="icon iconblank">';
	    htmlShow += 		'<div class="iconbg"></div>';
	    htmlShow += 	'</div>';
	    htmlShow += 	'<div style="height:12px; line-height:12px;" class="content contentblank">';	
	    htmlShow += 		'<span class="messageblank">'+ mes +'</span>';		
	    htmlShow += 	'</div>';			
	    htmlShow += '</div>';			
	    // NOW append HTML
	    objShow.append(htmlShow);	 
}

/**
* function checkCurrentStep()
* Detect Current Step 
* Note: Can put more Params here 
*
* @return the number of the current step
*/
function checkCurrentStep() {
	var $panes   = $('.multipage-panes .multipage-pane');
	var re_value = 1;
	$panes.each(function (index, domEle) {
		hideorshow_thisstep = $(domEle).css('display');
		
		if (hideorshow_thisstep == 'block') {			
			re_value = $(domEle).attr('step');	
			//return re_value;
		}	
	}); // END EACH	
	return re_value;
}

/**
* function nextStep_formone()
* IF FORM 1 returns true, Do this ACTION   
* SUBMIT DATA to THE SERVER - save and show next form  
* Note: Can put more Params here 
*
* @return true if the form validates, false if it fails
*/
function nextStep_formone(cur_step) {
	// IF SENDING DATA is OK, ADD OK INTO  step step_1
	$('.step.step_'+(cur_step)).append('<em>OK</em>');
	$(".step.step_"+ (cur_step+1) +", .step.step_"+ (cur_step) +" .line_to_step").addClass('active');
				
}

/**
* function preStep_formone()
* IF FORM 1 returns true, Do this ACTION   
* SUBMIT DATA to THE SERVER - save and show previous form  
* Note: Can put more Params here 
*
* @return true if the form validates, false if it fails
*/
function preStep_formone(cur_step) {
		// IF SENDING DATA is OK, ADD OK INTO  step step_1
		$('.step.step_'+(cur_step)+' em ').remove();
		$(".step.step_"+ (cur_step+1) +", .step.step_"+ (cur_step) +" .line_to_step").removeClass('active');
		
}
})(jQuery);



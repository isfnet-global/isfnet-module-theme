(function ($) {
$(document).ready(function(){



$('ul.menu li:even').addClass('even');
$('ul.menu li:odd').addClass('odd');

/* Set Up Sliders */

$('aside h2.block-title').addClass('slider');

/* Section Slider */
$('.slider').click(function(){

	$(this).toggleClass('slider-active');
	$(this).parent().find('div.content').slideToggle('slow');
		return false; //Prevent the browser jump to the link anchor
	});


/* Correct the Slideshow pager with */

if ($('body').hasClass('front')) {

  var count = $('#widget_pager_bottom_slideshow-block').children().size();
  var width = $('#widget_pager_bottom_slideshow-block div.views-content-field-image-thumbnail').outerWidth();
  
  var size = (width * count) + 100;
  $('#widget_pager_bottom_slideshow-block').width(size);

	
	}
  
/*********************
	scrollfunction
	*********************/
	$(window).scroll(function() {
		if($(this).scrollTop() !== 0) {
			$('.scrolltop').fadeIn();	
		} else {
			$('.scrolltop').fadeOut();
		}
	});
	 
	$('div.scrolltop').click(function() {
		$('body,html').animate({scrollTop:0},1500);
	});	

});
})(jQuery);
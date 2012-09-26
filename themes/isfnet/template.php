<?php
/**
 * Implements hook_html_head_alter().
 * We are overwriting the default meta character type tag with HTML5 version.
 */
function isfnet_html_head_alter(&$head_elements) {
  $head_elements['system_meta_content_type']['#attributes'] = array(
    'charset' => 'utf-8'
  );

}

/**
 * Return a themed breadcrumb trail.
 *
 * @param $breadcrumb
 *   An array containing the breadcrumb links.
 * @return a string containing the breadcrumb output.
 */
function isfnet_breadcrumb($variables) {
  $breadcrumb = $variables['breadcrumb'];

  if (!empty($breadcrumb)) {
    // Provide a navigational heading to give context for breadcrumb links to
    // screen-reader users. Make the heading invisible with .element-invisible.
    $heading = '<h2 class="element-invisible">' . t('You are here') . '</h2>';
    // Uncomment to add current page to breadcrumb
	// $breadcrumb[] = drupal_get_title();
    return '<nav class="breadcrumb">' . $heading . implode(' » ', $breadcrumb) . '</nav>';
  }
}

/**
 * Duplicate of theme_menu_local_tasks() but adds clearfix to tabs.
 */
function isfnet_menu_local_tasks(&$variables) {
  $output = '';

  if (!empty($variables['primary'])) {
    $variables['primary']['#prefix'] = '<h2 class="element-invisible">' . t('Primary tabs') . '</h2>';
    $variables['primary']['#prefix'] .= '<ul class="tabs primary clearfix">';
    $variables['primary']['#suffix'] = '</ul>';
    $output .= drupal_render($variables['primary']);
  }
  if (!empty($variables['secondary'])) {
    $variables['secondary']['#prefix'] = '<h2 class="element-invisible">' . t('Secondary tabs') . '</h2>';
    $variables['secondary']['#prefix'] .= '<ul class="tabs secondary clearfix">';
    $variables['secondary']['#suffix'] = '</ul>';
    $output .= drupal_render($variables['secondary']);
  }
  return $output;
}

/**
 * Override or insert variables into the node template.
 */
function isfnet_preprocess_node(&$variables) {
  $variables['submitted'] = t('Published by !username on !datetime', array('!username' => $variables['name'], '!datetime' => $variables['date']));
  if ($variables['view_mode'] == 'full' && node_is_page($variables['node'])) {
    $variables['classes_array'][] = 'node-full';
  }
}

/**
 * Preprocess variables for region.tpl.php
 *
 * @param $variables
 *   An array of variables to pass to the theme template.
 * @param $hook
 *   The name of the template being rendered ("region" in this case.)
 */
function isfnet_preprocess_region(&$variables, $hook) {
  // Use a bare template for the content region.
  if ($variables['region'] == 'content') {
    $variables['theme_hook_suggestions'][] = 'region__bare';
  }
}

/**
 * init render HTML page.
 */
function isfnet_preprocess_html(&$variables) {
	$options = array(
		'group' => JS_THEME,
	);
	drupal_add_js(drupal_get_path('theme', 'isfnet'). '/js/main.js', $options);
	
	
	
	// MINNGUYEN
     $path = isset($_GET['q']) ? $_GET['q'] : '<front>';
  	 $link = url($path, array('absolute' => TRUE));
  	 if (strstr($link, "members/")) {
  		$file = "sites/all/themes/isfnet/css/profile.css";
  		drupal_add_css($file, array('group' => CSS_DEFAULT));
  		
  	 }  // END (strstr($link, ...
  	 if (strstr($link, "user/")) {
  	 	drupal_add_css(drupal_get_path('module', 'profile2_regpath') . '/css/style.css', 'theme');
  	 	drupal_add_js(drupal_get_path('module', 'profile2_regpath') . '/js/jaction.js', 'file');
  	 }
  	 if (strstr($link, "/jobs")) {
  	 	
  	 	 drupal_add_css(drupal_get_path('theme', 'isfnet'). '/css/jobs.css', $options);  	 	 
  	 	//drupal_add_js(drupal_get_path('module', 'profile2_regpath') . '/js/jaction.js', 'file');
  	 }
	// TONY CHIU: added if condition for google_maps autocomplete js
	if (strstr($link, "applicant/register")) {
		drupal_add_js(drupal_get_path('module', 'profile2_regpath') . '/js/google_maps_autocomplete.js', 'file');
		drupal_add_js(drupal_get_path('module', 'profile2_regpath') . '/js/googlemaps.js', 'file');
	}
	 //----------------------------	
	
	// Classes for body element. Allows advanced theming based on context
  // (home page, node of certain type, etc.)
  if (!$variables['is_front']) {
    // Add unique class for each page.
    $path = drupal_get_path_alias($_GET['q']);
    // Add unique class for each website section.
    list($section, ) = explode('/', $path, 2);
    if (arg(0) == 'node') {
      if (arg(1) == 'add') {
        $section = 'node-add';
      }
      elseif (is_numeric(arg(1)) && (arg(2) == 'edit' || arg(2) == 'delete')) {
        $section = 'node-' . arg(2);
      }
    }
    $variables['classes_array'][] = drupal_html_class('section-' . $section);
  }
 
  // Store the menu item since it has some useful information.
  $variables['menu_item'] = menu_get_item();
  switch ($variables['menu_item']['page_callback']) {
    case 'views_page':
      // Is this a Views page?
      $variables['classes_array'][] = 'page-views';
      break;
    case 'page_manager_page_execute':
    case 'page_manager_node_view':
    case 'page_manager_contact_site':
      // Is this a Panels page?
      $variables['classes_array'][] = 'page-panels';
      break;
  }
	
	
}

function isfnet_preprocess_page(&$vars) {
	
}

/**
 * Override or insert variables into the block templates.
 *
 * @param $variables
 *   An array of variables to pass to the theme template.
 * @param $hook
 *   The name of the template being rendered ("block" in this case.)
 */
function isfnet_preprocess_block(&$variables, $hook) {
  // Use a bare template for the page's main content.
  if ($variables['block_html_id'] == 'block-system-main') {
    $variables['theme_hook_suggestions'][] = 'block__bare';
  }
  $variables['title_attributes_array']['class'][] = 'block-title';
}

/**
 * Override or insert variables into the block templates.
 *
 * @param $variables
 *   An array of variables to pass to the theme template.
 * @param $hook
 *   The name of the template being rendered ("block" in this case.)
 */
function isfnet_process_block(&$variables, $hook) {
  // Drupal 7 should use a $title variable instead of $block->subject.
  $variables['title'] = $variables['block']->subject;
}

/**
 * Changes the search form to use the "search" input element of HTML5.
 */
function isfnet_preprocess_search_block_form(&$vars) {
  $vars['search_form'] = str_replace('type="text"', 'type="search"', $vars['search_form']);
}

/*------------- MINH NGUYEN override css ------------------------------ */
function isfnet_js_alter(&$javascript) {
  if (isset($javascript['sites/all/modules/field_group/multipage/multipage.js'])) {
    $file = "sites/all/themes/isfnet/js/multipage.js";
    $javascript['sites/all/modules/field_group/multipage/multipage.js'] = drupal_js_defaults($file);   
  } //  END: (isset($ja...
   	 $path = isset($_GET['q']) ? $_GET['q'] : '<front>';
  	 $link = url($path, array('absolute' => TRUE));
  	 if (strstr($link, "members/")) {
  		$file = "sites/all/themes/isfnet/js/jprofile.js";
  		$javascript[] = drupal_js_defaults($file);
  	 }
}


// END OF FILE

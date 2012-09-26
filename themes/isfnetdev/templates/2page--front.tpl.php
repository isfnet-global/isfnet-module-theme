<?php
/**
 * @file
 * Zen theme's implementation to display a single Drupal page.
 *
 * Available variables:
 *
 * General utility variables:
 * - $base_path: The base URL path of the Drupal installation. At the very
 *   least, this will always default to /.
 * - $directory: The directory the template is located in, e.g. modules/system
 *   or themes/garland.
 * - $is_front: TRUE if the current page is the front page.
 * - $logged_in: TRUE if the user is registered and signed in.
 * - $is_admin: TRUE if the user has permission to access administration pages.
 *
 * Site identity:
 * - $front_page: The URL of the front page. Use this instead of $base_path,
 *   when linking to the front page. This includes the language domain or
 *   prefix.
 * - $logo: The path to the logo image, as defined in theme configuration.
 * - $site_name: The name of the site, empty when display has been disabled
 *   in theme settings.
 * - $site_slogan: The slogan of the site, empty when display has been disabled
 *   in theme settings.
 *
 * Navigation:
 * - $main_menu (array): An array containing the Main menu links for the
 *   site, if they have been configured.
 * - $secondary_menu (array): An array containing the Secondary menu links for
 *   the site, if they have been configured.
 * - $secondary_menu_heading: The title of the menu used by the secondary links.
 * - $breadcrumb: The breadcrumb trail for the current page.
 *
 * Page content (in order of occurrence in the default page.tpl.php):
 * - $title_prefix (array): An array containing additional output populated by
 *   modules, intended to be displayed in front of the main title tag that
 *   appears in the template.
 * - $title: The page title, for use in the actual HTML content.
 * - $title_suffix (array): An array containing additional output populated by
 *   modules, intended to be displayed after the main title tag that appears in
 *   the template.
 * - $messages: HTML for status and error messages. Should be displayed
 *   prominently.
 * - $tabs (array): Tabs linking to any sub-pages beneath the current page
 *   (e.g., the view and edit tabs when displaying a node).
 * - $action_links (array): Actions local to the page, such as 'Add menu' on the
 *   menu administration interface.
 * - $feed_icons: A string of all feed icons for the current page.
 * - $node: The node object, if there is an automatically-loaded node
 *   associated with the page, and the node ID is the second argument
 *   in the page's path (e.g. node/12345 and node/12345/revisions, but not
 *   comment/reply/12345).
 *
 * Regions:
 * - $page['help']: Dynamic help text, mostly for admin pages.
 * - $page['highlighted']: Items for the highlighted content region.
 * - $page['content']: The main content of the current page.
 * - $page['sidebar_first']: Items for the first sidebar.
 * - $page['sidebar_second']: Items for the second sidebar.
 * - $page['header']: Items for the header region.
 * - $page['footer']: Items for the footer region.
 * - $page['bottom']: Items to appear at the bottom of the page below the footer.
 *
 * @see template_preprocess()
 * @see template_preprocess_page()
 * @see zen_preprocess_page()
 * @see template_process()
 */
?>

<div id="page-wrapper"><div id="page">

  <div id="header"><div class="section clearfix">

    <?php if ($logo): ?>
       <a href="<?php print $front_page; ?>" title="<?php print t('Home'); ?>" rel="home" id="logo"><img src="<?php print $logo; ?>" alt="<?php print t('Home'); ?>" /></a>
     <?php endif; ?>

    <?php if ($site_name || $site_slogan): ?>
      <div id="name-and-slogan">
        <?php if ($site_name): ?>
          <?php if ($title): ?>
            <div id="site-name"><strong>
              <a href="<?php print $front_page; ?>" title="<?php print t('Home'); ?>" rel="home"><span><?php print $site_name; ?></span></a>
            </strong></div>
          <?php else: /* Use h1 when the content title is empty */ ?>
            <h1 id="site-name">
              <a href="<?php print $front_page; ?>" title="<?php print t('Home'); ?>" rel="home"><span><?php print $site_name; ?></span></a>
            </h1>
          <?php endif; ?>
        <?php endif; ?>

        <?php if ($site_slogan): ?>
          <div id="site-slogan"><?php print $site_slogan; ?></div>
        <?php endif; ?>
      </div><!-- /#name-and-slogan -->
    <?php endif; ?>
<?
/*    <?php print theme('links__system_secondary_menu', array(
 *     'links' => $secondary_menu,
 *     'attributes' => array(
 *      'id' => 'secondary-menu',
 *      'class' => array('links', 'inline', 'clearfix'),
 *    ),
 *    'heading' => array(
 *      'text' => $secondary_menu_heading,
 *      'level' => 'h2',
 *      'class' => array('element-invisible'),
 *    ),
 *  )); ?>
*/
?>
    <?php print render($page['header']); ?>

  </div></div><!-- /.section, /#header -->

  <div id="main-wrapper"><div id="main" class="clearfix<?php if ($main_menu || $page['navigation']) { print ' with-navigation'; } ?>">

    <div id="content" class="column"><div class="section">
      <?php print render($page['highlighted']); ?>
      <?php print $breadcrumb; ?>
      <a id="main-content"></a>
      <?php print render($title_prefix); ?>
      <?php if ($title): ?>
        <h1 class="title" id="page-title"><?php print $title; ?></h1>
      <?php endif; ?>
      <?php print render($title_suffix); ?>
      <?php print $messages; ?>
      <?php if ($tabs = render($tabs)): ?>
        <div class="tabs"><?php print $tabs; ?></div>
      <?php endif; ?>
      <?php print render($page['help']); ?>
      <?php if ($action_links): ?>
        <ul class="action-links"><?php print render($action_links); ?></ul>
      <?php endif; ?>
      <?php print render($page['content']); ?>
      <?php print $feed_icons; ?>
    </div></div><!-- /.section, /#content -->

    <?php if ($page['navigation'] || $main_menu): ?>
      <div id="navigation" ><div class="section clearfix">

        <?php print theme('links__system_main_menu', array(
          'links' => $main_menu,
          'attributes' => array(
            'id' => 'main-menu',
            'class' => array('links', 'inline', 'clearfix'),
          ),
          'heading' => array(
            'text' => t('Main menu'),
            'level' => 'h2',
            'class' => array('element-invisible'),
          ),
        )); ?>

        <?php print render($page['navigation']); ?>

      </div></div><!-- /.section, /#navigation -->
    <?php endif; ?>

    <?php print render($page['sidebar_first']); ?>

    <?php print render($page['sidebar_second']); ?>
    
    

  </div><div id="latest_jobs_block" style="font-size:11px; padding-left:20px; width:180px;"><?php print render($page['latest_jobs']); ?><br />
<br />
</div></div><!-- /#main, /#main-wrapper -->

  <?php print render($page['footer']); ?>

</div>
<link href="http://localhost:8888/isfnet/sites/all/themes/isfnet2/css/css.css" media="screen" rel="stylesheet" type="text/css" />
<div id="logo">
  <div id="logo_inner"><img alt="isfnet.com" height="47" src="http://localhost:8888/isfnet/themes/zen/logo.png" width="146" /></div>
</div>
<!-- end logo -->
<div id="nav">
  <div id="nav_inner">
    <ul>
      <li><a href="#home"><img height="20" src="http://localhost:8888/isfnet/themes/isfnet2/images/home_icon.png" width="21" /></a></li>
      <li><a href="http://localhost:8888/isfnet/?q=node/5">About Us</a></li>
      <li><a href="http://localhost:8888/isfnet/?q=node/2">Services</a></li>
      <li><a href="http://localhost:8888/isfnet/?q=node/3">Partnerships</a></li>
      <li><a href="http://localhost:8888/isfnet/?q=node/4">Careers</a></li>
      <li><a href="http://localhost:8888/isfnet/?q=node/5">Contact Us</a></li>
    </ul>
  </div>
  <!-- end nav_inner --></div>
<!-- end nav -->
<div id="banner"><img alt="CEO Message" class="full" height="268" src="http://localhost:8888/isfnet/themes/isfnet2/images/img01.jpg" width="940" /></div>
<!-- end banner -->
<div id="tagline"><script language="JavaScript">
<!--
var r_text = new Array ();
r_text[0] = "Our aim is to provide the highest standards globally...";
r_text[1] = "Powered by the Desire to Improve Lives...";
r_text[2] = "Working with you for a greener future...";
r_text[3] = "We care about our employees, Our Communities, and Our Environment...";
r_text[4] = "Hospitality and Quality IT Customer Care...";
r_text[5] = "ISFnet is commited to giving back to local through our strong global prescence..";
r_text[6] = "Our job placement and training services redefine the global IT ecosystem...";
var i = Math.floor(7*Math.random())
document.write(r_text[i]);
//-->
</script></div>
<!-- end tagline --><br />
<div id="container" style="margin:0px auto; width:940px; height:350px; ">
  <div id="itsolutions_container" style="width: 940px; font-size:12px;">
    <div style="float: left;  width: 215px; border:1px solid #e4f0fd;margin-right:7px;padding: 5px; background-image:url(http://localhost:8888/isfnet/themes/isfnet2/images/bg_small_column.jpg); background-repeat:repeat-x; background-color:white; height:320px;"><img height="119" src="http://localhost:8888/isfnet/themes/isfnet2/images/itsolutions.jpg" width="199" /> eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat.Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat...<br />
      <br />
      <a href="#">Read More...</a></div>
    <!-- end it_solutions_container -->
    <div style="float: left;  width: 215px;border:1px solid #e4f0fd;margin-right:7px;padding: 5px; background-image:url(http://localhost:8888/isfnet/themes/isfnet2/images/bg_small_column.jpg); background-repeat:repeat-x; background-color:white;height:320px;padding-left:5px;"><img height="115" src="http://localhost:8888/isfnet/themes/isfnet2/images/engineer.jpg" width="199" /> eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat.Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat...<br />
      <br />
      <a href="#">Read More... </a></div>
    <div style="float: left;  width: 215px;border:1px solid #e4f0fd;margin-right:7px;padding: 5px; background-image:url(http://localhost:8888/isfnet/themes/isfnet2/images/bg_small_column.jpg); background-repeat:repeat-x; background-color:white; height:320px;"><img height="119" src="http://localhost:8888/isfnet/themes/isfnet2/images/hrservices.jpg" width="199" /> eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat.Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat...<br />
      <br />
      <a href="#">Read More...</a></div>
    <div style="float: right;  width: 220px;border:1px solid #e4f0fd;margin-right:0px;padding: 5px; background-image:url(http://localhost:8888/isfnet/themes/isfnet2/images/bg_small_column.jpg); background-repeat:repeat-x; background-color:white;"><span class="header_blue">Job Postings</span><br />
      <br />
      <div id="latest_jobs_block" style="font-size:11px; padding-left:20px; width:180px;"><br />
        <?php print render($page['latest_jobs']); ?></div>
    </div>
    <div style="clear: both;">&nbsp;</div>
  </div>
</div>
<div id="bottom">
  <div style="width: 940px;">
    <div style="float: left;  width: 225px;margin-right:15px; margin-left:15px;"><span class="header_blue">Member Login</span><br />
      <br />
      <div id="form"><?php
$block = module_invoke('user', 'block_view', '2');
print render($block);
?><br />
        <br />
        <img height="115" src="http://localhost:8888/isfnet/themes/isfnet2/images/Partnership.jpg" width="199" /></div>
    </div>
    <div style="float: left;  width: 300px; margin-right:30px; font-size:12px;"><!-- begin news --><span class="header_blue">News</span><br />
      <br />
      <a href="#news">ISFnet Thailand opens</a><br />
      <span class="news_item">Lorem Ipsum is simply dummy text of the printing and typesetting industry. Lorem Ipsum has been the industry&#39;s standard dummy text ever since the 1500s.<br />
      <a href="#">Read More &gt;&gt;</a> </span><br />
      <br />
      <a href="#news">ISFnet Thailand opens</a><br />
      <span class="news_item">Lorem Ipsum is simply dummy text of the printing and typesetting industry. Lorem Ipsum has been the industry&#39;s standard dummy text ever since the 1500s.<br />
      <a href="#">Read More &gt;&gt;</a> </span><br />
      <br />
      <a href="#news">ISFnet Thailand opens</a><br />
      <span class="news_item">Lorem Ipsum is simply dummy text of the printing and typesetting industry. Lorem Ipsum has been the industry&#39;s standard dummy text ever since the 1500s.<br />
      <a href="#">Read More &gt;&gt;</a> </span></div>
    <!-- end end news -->
    <div style="float: left;  width: 300px; font-size:12px;"><!-- begin subsidiaries --><span class="header_blue">Subsidiaries</span><br />
      <br />
      <span class="job_postings"><a href="#">ISFnet Inc.</a><br />
      <br />
      <a href="#">ISFnet Harmony Ltd.</a><br />
      <br />
      <a href="#">ISFnet Care Ltd.</a><br />
      <br />
      <a href="#">ISFnet Life Ltd</a><br />
      <br />
      <a href="#">Joy Consulting Ltd</a><br />
      <br />
      <a href="#">ISFnet Global - China, India, Korea, Malaysia, Singapore, Vietnam</a> </span></div>
    <!-- end subsidiaries -->
    <div style="clear: both;">&nbsp;</div>
    <hr />
  </div>
</div>
<div id="footer">
  <div id="footer_top"><a href="#">Home</a> &bull; <a href="#">About Us</a> &bull; <a href="#">Our Values</a> &bull; <a href="#">Services</a> &bull; <a href="#">Partnership</a> &bull; <a href="#">News &amp; Events</a> &bull; <a href="#">Careers</a> &bull; <a href="#">Contact Us</a></div>
  <div id="footer_bottom">
    <div>
      <div style="float: left; width: 500px;"><a href="#">Privacy Policy</a> &bull; <a href="#">Terms of Use</a> &bull; <a href="#">Sitemap</a></div>
      <div style="float: right;  width: 300px;">&copy; 2011 ISFnet Group, Inc. All rights reserved.</div>
      <div style="clear: both;">&nbsp;</div>
    </div>
  </div>
</div>

</div><!-- /#page, /#page-wrapper -->

<?php print render($page['bottom']); ?>

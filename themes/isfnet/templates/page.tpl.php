<div id="skip-link">
            <a href="#main-content-link" class="element-invisible element-focusable"><?php print t('Skip to main content'); ?></a> <?php if ($main_menu): ?> <a href="#navigation" class="element-invisible element-focusable"><?php print t('Skip to navigation'); ?></a> <?php endif; ?>
        </div>

        <header id="header" role="banner" class="clearfix">
            <div id="header-content" class="clearfix center-page">
                <?php if ($page['header_top']): ?><!-- #content-top -->

                <div id="header-top">
                <div id="header-top-content">
                    <?php print render($page['header_top']); ?>
                    </div><!-- end #header-top-content -->
                </div><!-- end #header-top -->
                <?php endif; ?><?php if ($logo): ?> <a href="<?php print $front_page; ?>" title="<?php print t('Home'); ?>" id="logo"><img src="<?php print $logo; ?>" alt="<?php print t('Home'); ?>"></a> <?php endif; ?> <?php if ($site_name || $site_slogan): ?>

                <hgroup id="site-name-slogan">
                    <?php if ($site_name): ?>

                    <h1 id="site-name"><a href="<?php print $front_page; ?>" title="<?php print t('Home'); ?>"><span><?php print $site_name; ?></span></a></h1><?php endif; ?><?php if ($site_slogan): ?>

                    <h2 id="site-slogan"><?php print $site_slogan; ?></h2><?php endif; ?>
                </hgroup><?php endif; ?><?php print render($page['header']); ?>
            </div><!-- end header-content -->
        </header><!-- end #header -->
        <?php if ($main_menu || !empty($page['navigation'])): ?>

        <nav id="navigation" role="navigation" class="clearfix">
            <div id="navigation-content" class="center-page">
                <?php if (!empty($page['navigation'])): ?><!--if block in navigation region, override $main_menu and $secondary_menu-->
                <?php print render($page['navigation']); ?><?php endif; ?><?php if (empty($page['navigation'])): ?><?php print theme('links__system_main_menu', array(
                            'links' => $main_menu,
                            'attributes' => array(
                              'id' => 'main-menu',
                              'class' => array('links', 'clearfix'),
                            ),
                            'heading' => array(
                              'text' => t('Main menu'),
                              'level' => 'h2',
                              'class' => array('element-invisible'),
                            ),
                          )); ?><?php endif; ?>
            </div><!-- end navigation-content -->
        </nav><!-- end #navigation -->
        <?php endif; ?>

        <section id="main-wrapper" role="main" class="clearfix">
           
           <?php /* Disable Breadcrumbs
              if (!$is_front):  ?>
            <section id="breadcrumbs" class="center-page">
            <?php print $breadcrumb; ?>
            </section>
            <?php endif; */ ?>            
            
            <?php if ($page['main_top']): ?>
 			<section id="main-top">
                <div id="main-top-content" class="center-page">
                    <?php print render($page['main_top']); ?>
                </div>
            </section><?php endif; ?>

            <section id="main-page-wrapper" class="clearfix">
                <div id="main-page" role="main-page" class="center-page">
                <?php print $messages; ?>
                
                    <?php if ($page['content_top']):  ?><!-- #content-top -->

                    <div id="content-top">
                        <?php print render($page['content_top']); ?>
                    </div><!-- end #content-top -->
                    <?php endif;  ?><?php if ($page['sidebar_first']): ?><!-- #sidebar-first -->

                    <aside id="sidebar-first" role="complementary" class="sidebar clearfix"><?php print render($page['sidebar_first']); ?></aside><!-- end #sidebar-first -->
                    <?php endif; ?><?php if ($page['sidebar_second']): ?><!-- #sidebar-second -->

                    <aside id="sidebar-second" role="complementary" class="sidebar clearfix"><?php print render($page['sidebar_second']); ?></aside><!-- end #sidebar-second -->
                    <?php endif; ?><!-- #main-content -->

                    <div id="main-content" class="clearfix">
                        <a id="main-content-anchor"></a> <?php if ($page['highlighted']): ?>

                        <div id="highlighted">
                            <?php print render($page['highlighted']); ?>
                        </div><?php endif; ?>
                        <?php print render($title_prefix); ?><?php if ($title && !$is_front): ?>

                        <h1 class="title" id="page-title"><?php print $title; ?></h1><?php endif; ?><?php print render($title_suffix); ?><?php if (!empty($tabs['#primary'])): ?>

                        <div class="tabs-wrapper clearfix">
                            <?php print render($tabs); ?>
                        </div><?php endif; ?>
                        <?php print render($page['help']); ?><?php if ($action_links): ?>

                        <ul class="action-links">
                            <?php print render($action_links); ?>
                        </ul><?php endif; ?><?php print render($page['content']); ?>
                    </div><!-- end #main-content -->
                    <?php if ($page['content_bottom']): ?><!-- #content-bottom -->

                    <div id="content-bottom">
                        <?php print render($page['content_bottom']); ?>
                    </div><!-- end #content-bottom -->
                    <?php endif; ?>
                </div><!-- end #main-page -->
            </section><!-- end #main-page-wrapper -->
        </section><!-- end #main-wrapper -->
        <!-- #footer -->

        <footer id="footer" role="contentinfo" class="clearfix">
           
            <?php if ($page['footer_top']): ?><!-- #footer-top -->

            <section id="footer-top" class="center-page clearfix">
                <div id="footer-top-content">
                    <?php print render($page['footer_top']); ?>
                </div>
            </section><!-- end #footer-top -->
            <?php endif; ?>
            
            <?php if ($page['footer_left'] || $page['footer_middle'] || $page['footer_right']):  ?>
            <!-- #footer-content -->

            <section id="footer-content" class="center-page clearfix">
                <div id="top-link" class="scrolltop">
                    <?php print t('^ Top'); ?>
                </div>

                <div id="footer-left" class="footer-block">
                 <div id="footer-left-content" class="footer-block-content">
                <?php if (!empty($page['footer_left'])): print render($page['footer_left']); endif; ?>
                </div>
                </div>

                <div id="footer-middle" class="footer-block">
                    <div id="footer-middle-content" class="footer-block-content">
                        <?php if (!empty($page['footer_middle'])): print render($page['footer_middle']);  endif; ?>
                    </div>
                </div>

                    <div id="footer-right" class="footer-block">
                        <div id="footer-right-content" class="footer-block-content">
                            <?php if (!empty($page['footer_right'])): print render($page['footer_right']); endif; ?>
                        </div>
                    </div>
                </div>
            </section><!-- end #footer-content -->
            <?php endif; ?>
            
            <?php if ($page['footer_bottom']): ?><!-- #footer-bottom -->

            <section id="footer-bottom" class="center-page clearfix">
                <div id="footer-bottom-content">
                    <?php print render($page['footer_bottom']); ?>
                </div>
            </section><!-- end #footer-bottom -->
            <?php endif; ?>
        </footer><!-- end #footer -->
        <!-- #main-bottom -->

        <section id="main-bottom">
            <div id="main-bottom-content" class="center-page">
                <?php if ($page['main_bottom']): ?><?php print render($page['main_bottom']); ?><?php endif; ?>

                <h5 id="copyright"><?php print $footer_message; ?></h5>
            </div>
        </section><!-- end #main-bottom -->

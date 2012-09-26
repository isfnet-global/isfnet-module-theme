/**
 * This file is a part of mod_tpniceslideshow package
 * Author: http://www.templateplazza.com
 * Creator: Jerry Wijaya ( me@jerrywijaya.com )
*/

/*
    This file is part of JonDesign's SmoothGallery v2.0.

    JonDesign's SmoothGallery is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 3 of the License, or
    (at your option) any later version.

    JonDesign's SmoothGallery is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with JonDesign's SmoothGallery; if not, write to the Free Software
    Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA

    Main Developer: Jonathan Schemoul (JonDesign: http://www.jondesign.net/)
    Contributed code by:
    - Christian Ehret (bugfix)
	- Nitrix (bugfix)
	- Valerio from Mad4Milk for his great help with the carousel scrolling and many other things.
	- Archie Cowan for helping me find a bugfix on carousel inner width problem.
	- Tomocchino from #mootools for the preloader class
	Many thanks to:
	- The mootools team for the great mootools lib, and it's help and support throughout the project.
*/

/*
edited by Jerry Wijaya ( me@jerrywijaya.com )
as required by TPNiceSlideShow Module for http://www.templateplazza.com
edit date: 22/11/2008
*/

// declaring the class
var TPSlideShowClass = {
	initialize: function(element, options) {
		this.setOptions({
			showMenu: true,
			showInfopane: true,
			embedLinks: true,
			fadeDuration: 500,
			readMore: true,
			readMoreText: 'Read More',
			showTitle: true,
			titleLink: true,
			showDescription: true,
			timed: false,
			delay: 9000,
			preloader: true,
			preloaderImage: true,
			preloaderErrorImage: true,
			/* Data retrieval */
			manualData: [],
			populateData: true,
			destroyAfterPopulate: true,
			elementSelector: "div.imageElement",
			titleSelector: "h3",
			subtitleSelector: "p",
			linkSelector: "a.open",
			imageSelector: "img.full",
			defaultTransition: "fade",
			/* InfoPane options */
			slideInfoZoneOpacity: 0.7,
			slideInfoZoneSlide: true,
			/* CSS Classes */
			baseClass: 'TPNiceSlideShowGallery',
			menuSlide: 'TPNiceSlideShowMenu',
			menuSlideActive: 'active',
			scrollBar: false,
			itemCount: 1,
			scrollBarContent:	false,
			scrollBarArea: false,
			scrollBarHandle: false
		}, options);
		this.fireEvent('onInit');
		this.currentIter = 0;
		this.lastIter = 0;
		this.maxIter = 0;
		this.galleryElement = element;
		this.galleryData = this.options.manualData;
		this.galleryInit = 1;
		this.galleryElements = Array();
		this.galleryElement.addClass(this.options.baseClass);
		
		if(this.options.scrollBar) {
			//this.makeScrollbar();
			this.steps = this.options.scrollBarContent.getSize().scrollSize.y - this.options.scrollBarContent.getSize().size.y;
			var myScrollFx = new Fx.Scroll(this.options.scrollBarContent, {
				wait: false
			});
			
			var slider = new Slider(this.options.scrollBarArea, this.options.scrollBarHandle, {
				mode: 'vertical',
				steps: this.steps,
				onChange: function(step){
					// Scrolls the content element in x or y direction.
					var x = 0;
					var y = step;
					myScrollFx.scrollTo(x,y);
				}
			}).set(0);
			
			this.slider = slider;
			this.sliderHeight = this.options.scrollBarContent.getSize().scrollSize.y / this.options.itemCount;
			
			$$(this.options.scrollBarContent, this.options.scrollBarArea).addEvent('mousewheel', function(e){	
				e = new Event(e).stop();
				var step = slider.step - e.wheel * 30;	
				slider.set(step);
			});
	
			$(document.body).addEvent('mouseleave',function(){slider.drag.stop()});
		}
		
		this.options.default_timed = this.options.timed;

		if(this.options.showMenu) {
			this.tpNav = $$('#'+this.options.menuSlide+' div');
			this.tpNav.each(function(el,i){
				if(i==0) {
					el.addClass(this.options.menuSlideActive);
				}
				el.addEvent('click',function(){
					this.goTo(i);
				}.bind(this,el,i));
			},this);
		}

		this.populateFrom = element;
		if (this.options.populateData)
			this.populateData();
		element.style.display="block";
		
		if (this.options.embedLinks)
		{
			this.galleryElement = element;
		}
		
		this.constructElements();
		this.loadingElement = new Element('div').addClass('loadingElement').injectInside(element);
		if (this.options.showInfopane) this.initInfoSlideshow();
		this.doSlideShow(1);
	},
	populateData: function() {
		currentArrayPlace = this.galleryData.length;
		options = this.options;
		var data = $A(this.galleryData);
		data.extend(this.populateGallery(this.populateFrom, currentArrayPlace));
		this.galleryData = data;
		this.fireEvent('onPopulated');
	},
	populateGallery: function(element, startNumber) {
		var data = [];
		options = this.options;
		currentArrayPlace = startNumber;
		element.getElements(options.elementSelector).each(function(el) {
			elementDict = {
				image: el.getElement(options.imageSelector).getProperty('src'),
				number: currentArrayPlace,
				transition: this.options.defaultTransition
			};
			elementDict.extend = $extend;
			if (options.showInfopane)
				elementDict.extend({
					title: el.getElement(options.titleSelector).innerHTML,
					description: el.getElement(options.subtitleSelector).innerHTML
				});
				elementDict.extend({
					link: el.getElement(options.linkSelector).href||false,
					linkTitle: el.getElement(options.linkSelector).title||false,
					linkTarget: el.getElement(options.linkSelector).getProperty('target')||false
				});
			
			data.extend([elementDict]);
			currentArrayPlace++;
			if (this.options.destroyAfterPopulate)
				el.remove();
		});
		return data;
	},
	constructElements: function() {
		el = this.galleryElement;
		this.maxIter = this.galleryData.length;
		var currentImg;
		for(i=0;i<this.galleryData.length;i++)
		{
			if(this.options.embedLinks) {
				var createElm = 'a';
			} else {
				var createElm = 'div';
			}
			var currentImg = new Fx.Styles(
				new Element(createElm).addClass('slideElement').setStyles({
					'position':'absolute',
					'left':'0px',
					'right':'0px',
					'margin':'0px',
					'padding':'0px',
					'backgroundPosition':"center center",
					'opacity':'0'
				}).injectInside(el),
				'opacity',
				{duration: this.options.fadeDuration}
			);
			
			if(this.options.embedLinks) {
				currentImg.element.setProperties({
					href: this.galleryData[parseInt(i)].link,
					title: this.galleryData[parseInt(i)].linkTitle
				});
			}

			if (this.options.preloader)
			{
				currentImg.source = this.galleryData[i].image;
				currentImg.loaded = false;
				currentImg.load = function(imageStyle) {
					if (!imageStyle.loaded)	{
						new Asset.image(imageStyle.source, {
		                            'onload'  : function(img){
													img.element.setStyle(
													'backgroundImage',
													"url('" + img.source + "')")
													img.loaded = true;
												}.bind(this, imageStyle)
						});
					}
				}.pass(currentImg, this);
			} else {
				currentImg.element.setStyle('backgroundImage',
									"url('" + this.galleryData[i].image + "')");
			}
			this.galleryElements[parseInt(i)] = currentImg;
		}
	},
	destroySlideShow: function(element) {
		var myClassName = element.className;
		var newElement = new Element('div').addClass('myClassName');
		element.parentNode.replaceChild(newElement, element);
	},
	startSlideShow: function() {
		this.fireEvent('onStart');
		this.loadingElement.style.display = "none";
		this.lastIter = this.maxIter - 1;
		this.currentIter = 0;
		this.galleryInit = 0;
		this.galleryElements[parseInt(this.currentIter)].set({opacity: 1});
		if (this.options.showInfopane)
			this.showInfoSlideShow.delay(0, this);
		this.prepareTimer();
	},
	nextItem: function() {
		this.fireEvent('onNextCalled');
		this.nextIter = this.currentIter+1;
		if (this.nextIter >= this.maxIter)
			this.nextIter = 0;
		this.galleryInit = 0;
		this.goTo(this.nextIter);
	},
	prevItem: function() {
		this.fireEvent('onPreviousCalled');
		this.nextIter = this.currentIter-1;
		if (this.nextIter <= -1)
			this.nextIter = this.maxIter - 1;
		this.galleryInit = 0;
		this.goTo(this.nextIter);
	},
	goTo: function(num) {
		if(this.options.scrollBar) {
			this.slider.set(num*this.sliderHeight);
		}
		this.clearTimer();
		if(this.options.preloader)
		{
			this.galleryElements[num].load();
			if (num==0)
				this.galleryElements[this.maxIter - 1].load();
			else
				this.galleryElements[num - 1].load();
			if (num==(this.maxIter - 1))
				this.galleryElements[0].load();
			else
				this.galleryElements[num + 1].load();
		}
		
		if (this.options.showInfopane)
		{
			this.hideInfoSlideShow().chain(this.changeItem.pass(num, this));
		} else {
			this.currentChangeDelay = this.changeItem.delay(0, this, num);
		}

		this.prepareTimer();

		if(this.options.showMenu) {
			this.tpSlide = $$('#'+this.options.menuSlide+' div');
			this.tpSlide.each(function(el,i){
				if(i == num) {
					el.addClass(this.options.menuSlideActive);
				} else {
					el.removeClass(this.options.menuSlideActive);
				}
			},this);
		}
	},
	changeItem: function(num) {
		this.galleryInit = 0;
		if (this.currentIter != num)
		{
			for(i=0;i<this.maxIter;i++)
			{
				if ((i != this.currentIter)) this.galleryElements[i].set({opacity: 0});
			}
			TPSlideShowClass.Transitions[this.galleryData[num].transition].pass([
				this.galleryElements[this.currentIter],
				this.galleryElements[num],
				this.currentIter,
				num], this)();
			this.currentIter = num;
		}
		this.doSlideShow.bind(this)();
		this.fireEvent('onChanged');
	},
	clearTimer: function() {
		if (this.options.timed)
			$clear(this.timer);
	},
	prepareTimer: function() {
		if (this.options.timed)
			this.timer = this.nextItem.delay(this.options.delay, this);
	},
	doSlideShow: function(position) {
		if (this.galleryInit == 1)
		{
			imgPreloader = new Image();
			imgPreloader.onload=function(){
				this.startSlideShow.delay(10, this);
			}.bind(this);
			imgPreloader.src = this.galleryData[0].image;
			if(this.options.preloader)
				this.galleryElements[0].load();
		} else {
			if (this.options.showInfopane)
			{
				if (this.options.showInfopane)
				{
					this.showInfoSlideShow();
				}
			}
		}
	},
	log: function(value) {
		if(console.log)
			console.log(value);
	},
	initInfoSlideshow: function() {
		this.slideInfoZone = new Fx.Styles(new Element('div').addClass('slideInfoZone').injectInside($(this.galleryElement))).set({'opacity':0});

		this.galleryElement.addEvents({
			'mouseover': function (myself) {
				this.options.timed = false;
				$clear(this.timer);
			}.pass(this.slideInfoZone, this)
		});

		this.galleryElement.addEvents({
			'mouseleave': function (myself) {
				if(this.options.default_timed) {
					this.options.timed = true;
					this.timer = this.nextItem.delay(this.options.delay, this);
				}
			}.pass(this.slideInfoZone, this)
		});
		
		this.slideInfoZoneDiv = new Fx.Styles(new Element('div').addClass('innerSlideInfoZone').injectInside($(this.slideInfoZone.element)));
		if(this.options.showTitle) {
			this.slideInfoZoneTitle = new Element('div').addClass('slideInfoZoneTitle').injectInside(this.slideInfoZoneDiv.element);
		}
		if(this.options.showDescription) {
			this.slideInfoZoneDescription = new Element('div').addClass('slideInfoZoneDescription').injectInside(this.slideInfoZoneDiv.element);
		}
		if(this.options.readMore) {
			this.slideInfoZoneReadMore = new Element('div').addClass('slideInfoZoneReadMore').injectInside(this.slideInfoZoneDiv.element);
		}
		this.slideInfoZone.element.setStyle('opacity',0);
	},
	showInfoSlideShow: function() {
		this.fireEvent('onShowInfopane');
		this.slideInfoZone.clearTimer();

		if(this.options.showTitle) {
			element = this.slideInfoZoneTitle;
			if(this.options.titleLink) {
				this.slideInfoZoneTitle.setHTML('<a href="'+this.galleryData[this.currentIter].link+'" title="'+this.galleryData[this.currentIter].linkTitle+'">'+this.galleryData[this.currentIter].title+'</a>');
			} else {
				this.slideInfoZoneTitle.setHTML(this.galleryData[this.currentIter].title);
			}
		}

		if(this.options.showDescription) {
			element = this.slideInfoZoneDescription;
			element.setHTML(this.galleryData[this.currentIter].description);
		}

		if(this.options.readMore) {
			element = this.slideInfoZoneReadMore;
			this.slideInfoZoneReadMore.setHTML('<a href="'+this.galleryData[this.currentIter].link+'" title="'+this.galleryData[this.currentIter].linkTitle+'">'+this.options.readMoreText+'</a>');
		}

		this.slideInfoZone.start({'opacity': [0, this.options.slideInfoZoneOpacity]});
		return this.slideInfoZone;
	},
	hideInfoSlideShow: function() {
		this.fireEvent('onHideInfopane');
		this.slideInfoZone.clearTimer();
		this.slideInfoZone.start({'opacity': 0});
		return this.slideInfoZone;
	},
	/* To change the gallery data, those two functions : */
	flushGallery: function() {
		this.galleryElements.each(function(myFx) {
			myFx.element.remove();
			myFx = myFx.element = null;
		});
		this.galleryElements = [];
	},
	changeData: function(data) {
		this.galleryData = data;
		this.clearTimer();
		this.flushGallery();
		this.constructElements();
		if (this.options.showInfopane) this.hideInfoSlideShow();
		this.galleryInit=1;
		this.lastIter=0;
		this.currentIter=0;
		this.doSlideShow(1);
	},
	makeScrollbar: function(){
	}
};
TPSlideShowClass = new Class(TPSlideShowClass);
TPSlideShowClass.implement(new Events);
TPSlideShowClass.implement(new Options);

TPSlideShowClass.Transitions = new Abstract ({
	fade: function(oldFx, newFx, oldPos, newPos){
		oldFx.options.transition = newFx.options.transition = Fx.Transitions.linear;
		oldFx.options.duration = newFx.options.duration = this.options.fadeDuration;
		if (newPos > oldPos) newFx.start({opacity: 1});
		else
		{
			newFx.set({opacity: 1});
			oldFx.start({opacity: 0});
		}
	},
	crossfade: function(oldFx, newFx, oldPos, newPos){
		oldFx.options.transition = newFx.options.transition = Fx.Transitions.linear;
		oldFx.options.duration = newFx.options.duration = this.options.fadeDuration;
		newFx.start({opacity: 1});
		oldFx.start({opacity: 0});
	},
	fadebg: function(oldFx, newFx, oldPos, newPos){
		oldFx.options.transition = newFx.options.transition = Fx.Transitions.linear;
		oldFx.options.duration = newFx.options.duration = this.options.fadeDuration / 2;
		oldFx.start({opacity: 0}).chain(newFx.start.pass([{opacity: 1}], newFx));
	}
});

/* All code copyright 2007 Jonathan Schemoul */

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Follows: Preloader (class)
 * Simple class for preloading images with support for progress reporting
 * Copyright 2007 Tomocchino.
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

var Preloader = new Class({
  
  Implements: [Events, Options],

  options: {
    root        : '',
    period      : 100
  },
  
  initialize: function(options){
    this.setOptions(options);
  },
  
  load: function(sources) {
    this.index = 0;
    this.images = [];
    this.sources = this.temps = sources;
    this.total = this. sources.length;
    
    this.fireEvent('onStart', [this.index, this.total]);
    this.timer = this.progress.periodical(this.options.period, this);
    
    this.sources.each(function(source, index){
      this.images[index] = new Asset.image(this.options.root + source, {
        'onload'  : function(){ this.index++; if(this.images[index]) this.fireEvent('onLoad', [this.images[index], index, source]); }.bind(this),
        'onerror' : function(){ this.index++; this.fireEvent('onError', [this.images.splice(index, 1), index, source]); }.bind(this),
        'onabort' : function(){ this.index++; this.fireEvent('onError', [this.images.splice(index, 1), index, source]); }.bind(this)
      });
    }, this);
  },
  
  progress: function() {
    this.fireEvent('onProgress', [Math.min(this.index, this.total), this.total]);
    if(this.index >= this.total) this.complete();
  },
  
  complete: function(){
    $clear(this.timer);
    this.fireEvent('onComplete', [this.images]);
  },
  
  cancel: function(){
    $clear(this.timer);
  }
    
});

Preloader.implement(new Events, new Options);
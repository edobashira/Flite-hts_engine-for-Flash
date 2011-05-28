/**
 * HUISlider.as
 * Keith Peters
 * version 0.91
 * 
 * A Horizontal slider with a label and a value label.
 * 
 * Source code licensed under a Creative Commons Attribution-Share Alike 3.0 License.
 * http://creativecommons.org/licenses/by-sa/3.0/
 * Some Rights Reserved.
 */
 
package com.bit101.components
{
	import flash.display.DisplayObjectContainer;
	import flash.events.Event;

	public class HUISlider extends UISlider
	{
		
		/**
		 * Constructor
		 * @param parent The parent DisplayObjectContainer on which to add this HUISlider.
		 * @param xpos The x position to place this component.
		 * @param ypos The y position to place this component.
		 * @param label The string to use as the label for this component.
		 * @param defaultHandler The event handling function to handle the default event for this component.
		 */
		public function HUISlider(parent:DisplayObjectContainer = null, x:Number = 0, y:Number = 0, label:String = "", defaultEventHandler:Function = null)
		{
			_sliderClass = HSlider;
			super(parent, x, y, label, defaultEventHandler);
		}
		
		/**
		 * Initializes the component.
		 */
		override protected function init():void
		{
			super.init();
			setSize(200, 18);
		}
		
		/**
		 * Centers the label when label text is changed.
		 */
		override protected function positionLabel():void
		{
			_valueLabel.x = _slider.x + _slider.width + 5;
		}
		
		
		
		
		///////////////////////////////////
		// public methods
		///////////////////////////////////
		
		/**
		 * Draws the visual ui of this component.
		 */
		override public function draw():void
		{
			super.draw();
			_slider.x = _label.width + 5;
			_slider.y = height / 2 - _slider.height / 2;
			_slider.width = width - _label.width - 50 - 10;
			
			_valueLabel.x = _slider.x + _slider.width + 5;
		}
		
		///////////////////////////////////
		// event handlers
		///////////////////////////////////
		
		///////////////////////////////////
		// getter/setters
		///////////////////////////////////
		
	}
}
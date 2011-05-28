/**
 * VUISlider.as
 * Keith Peters
 * version 0.91
 * 
 * A vertical Slider with a label and value label.
 * 
 * Source code licensed under a Creative Commons Attribution-Share Alike 3.0 License.
 * http://creativecommons.org/licenses/by-sa/3.0/
 * Some Rights Reserved.
 */
 
package com.bit101.components
{
	import flash.display.DisplayObjectContainer;
	import flash.events.Event;

	public class VUISlider extends UISlider
	{
		
		
		/**
		 * Constructor
		 * @param parent The parent DisplayObjectContainer on which to add this VUISlider.
		 * @param xpos The x position to place this component.
		 * @param ypos The y position to place this component.
		 * @param label The string to use as the label for this component.
		 * @param defaultHandler The event handling function to handle the default event for this component.
		 */
		public function VUISlider(parent:DisplayObjectContainer = null, x:Number = 0, y:Number = 0, label:String = "", defaultEventHandler:Function = null)
		{
			_sliderClass = VSlider;
			super(parent, x, y, label, defaultEventHandler);
		}
		
		/**
		 * Initializes this component.
		 */
		protected override function init():void
		{
			super.init();
			setSize(20, 146);
		}
		
		
		
		
		///////////////////////////////////
		// public methods
		///////////////////////////////////
		
		override public function draw():void
		{
			super.draw();
			_label.x = width / 2 - _label.width / 2;
			
			_slider.x = width / 2 - _slider.width / 2;
			_slider.y = _label.height + 5;
			_slider.height = height - _label.height - _valueLabel.height - 10;
			
			_valueLabel.x = width / 2 - _valueLabel.width / 2;
			_valueLabel.y = _slider.y + _slider.height + 5;
		}
		
		override protected function positionLabel():void
		{
			_valueLabel.x = width / 2 - _valueLabel.width / 2;
		}
		
		
		
		
		///////////////////////////////////
		// event handlers
		///////////////////////////////////
		
		///////////////////////////////////
		// getter/setters
		///////////////////////////////////
		
		override public function get width():Number
		{
			if(_label == null) return _width;
			return Math.max(_width, _label.width);
		}
		
	}
}
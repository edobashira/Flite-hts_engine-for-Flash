/**
 * UISlider.as
 * Keith Peters
 * version 0.91
 * 
 * A Slider with a label and value label. Abstract base class for VUISlider and HUISlider
 * 
 * Source code licensed under a Creative Commons Attribution-Share Alike 3.0 License.
 * http://creativecommons.org/licenses/by-sa/3.0/
 * Some Rights Reserved.
 */
 
package com.bit101.components
{
	import flash.display.DisplayObjectContainer;
	import flash.events.Event;

	public class UISlider extends Component
	{
		protected var _label:Label;
		protected var _valueLabel:Label;
		protected var _slider:Slider;
		private var _precision:int = 1;
		protected var _sliderClass:Class;
		private var _labelText:String;
		
		
		/**
		 * Constructor
		 * @param parent The parent DisplayObjectContainer on which to add this UISlider.
		 * @param xpos The x position to place this component.
		 * @param ypos The y position to place this component.
		 * @param label The initial string to display as this component's label.
		 * @param defaultHandler The event handling function to handle the default event for this component (change in this case).
		 */
		public function UISlider(parent:DisplayObjectContainer = null, x:Number = 0, y:Number = 0, label:String = "", defaultEventHandler:Function = null)
		{
			_labelText = label;
			super(parent, x, y);
			if(defaultEventHandler != null)
			{
				addEventListener(Event.CHANGE, defaultEventHandler);
			}
			formatValueLabel();
		}
		
		/**
		 * Creates and adds the child display objects of this component.
		 */
		override protected function addChildren():void
		{
			_label = new Label(this, 0, 0);
			_slider = new _sliderClass(this, 0, 0, onSliderChange);
			_valueLabel = new Label(this);
		}
		
		/**
		 * Formats the value of the slider to a string based on the current level of precision.
		 */
		protected function formatValueLabel():void
		{
			var mult:Number = Math.pow(10, _precision);
			var val:String = (Math.round(_slider.value * mult) / mult).toString();
			var parts:Array = val.split(".");
			if(parts[1] == null)
			{
				val += "."
				for(var i:uint = 0; i < _precision; i++)
				{
					val += "0";
				}
			}
			else if(parts[1].length < _precision)
			{
				for(i = 0; i < _precision - parts[1].length; i++)
				{
					val += "0";
				}
			}
			_valueLabel.text = val;
			positionLabel();
		}
		
		/**
		 * Positions the label when it has changed. Implemented in child classes.
		 */
		protected function positionLabel():void
		{
			
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
			_label.text = _labelText
			_label.draw();
		}
		
		/**
		 * Convenience method to set the three main parameters in one shot.
		 * @param min The minimum value of the slider.
		 * @param max The maximum value of the slider.
		 * @param value The value of the slider.
		 */
		public function setSliderParams(min:Number, max:Number, value:Number):void
		{
			_slider.setSliderParams(min, max, value);
		}
		
		
		
		
		///////////////////////////////////
		// event handlers
		///////////////////////////////////
		
		/**
		 * Handler called when the slider's value changes.
		 * @param event The Event passed by the slider.
		 */
		private function onSliderChange(event:Event):void
		{
			formatValueLabel();
			dispatchEvent(new Event(Event.CHANGE));
		}
		
		
		
		
		///////////////////////////////////
		// getter/setters
		///////////////////////////////////
		
		/**
		 * Sets / gets the current value of this slider.
		 */
		public function set value(v:Number):void
		{
			_slider.value = v;
			formatValueLabel();
		}
		public function get value():Number
		{
			return _slider.value;
		}
		
		/**
		 * Gets / sets the maximum value of this slider.
		 */
		public function set maximum(m:Number):void
		{
			_slider.maximum = m;
		}
		public function get maximum():Number
		{
			return _slider.maximum;
		}
		
		/**
		 * Gets / sets the minimum value of this slider.
		 */
		public function set minimum(m:Number):void
		{
			_slider.minimum = m;
		}
		public function get minimum():Number
		{
			return _slider.minimum;
		}
		
		/**
		 * Gets / sets the number of decimals to format the value label.
		 */
		public function set labelPrecision(decimals:int):void
		{
			_precision = decimals;
		}
		public function get labelPrecision():int
		{
			return _precision;
		}
		
		/**
		 * Gets / sets the text shown in this component's label.
		 */
		public function set label(str:String):void
		{
			_labelText = str;
//			invalidate();
			draw();
		}
		public function get label():String
		{
			return _labelText;
		}
	}
}
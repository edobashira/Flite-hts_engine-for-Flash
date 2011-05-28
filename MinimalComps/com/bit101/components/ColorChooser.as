/**
 * ColorChooser.as
 * Keith Peters
 * version 0.91
 * 
 * A bare bones Color Chooser component, allowing for textual input only.
 * 
 * Source code licensed under a Creative Commons Attribution-Share Alike 3.0 License.
 * http://creativecommons.org/licenses/by-sa/3.0/
 * Some Rights Reserved.
 */
 
package com.bit101.components
{
	import flash.display.DisplayObjectContainer;
	import flash.display.Sprite;
	import flash.events.Event;
	
	public class ColorChooser extends Component
	{
		private var _input:InputText;
		private var _swatch:Sprite;
		private var _value:uint = 0xff0000;
		
		/**
		 * Constructor
		 * @param parent The parent DisplayObjectContainer on which to add this ColorChooser.
		 * @param xpos The x position to place this component.
		 * @param ypos The y position to place this component.
		 * @param value The initial color value of this component.
		 * @param defaultHandler The event handling function to handle the default event for this component (change in this case).
		 */
		public function ColorChooser(parent:DisplayObjectContainer = null, xpos:Number = 0, ypos:Number =  0, value:uint = 0xff0000, defaultHandler:Function = null)
		{
			_value = value;
			super(parent, xpos, ypos);
			if(defaultHandler != null)
			{
				addEventListener(Event.CHANGE, defaultHandler);
			}
		}
		
		
		/**
		 * Initializes the component.
		 */
		override protected function init():void
		{
			super.init();

			_width = 65;
			_height = 15;
			value = _value;
		}
		
		override protected function addChildren():void
		{
			_input = new InputText();
			_input.width = 45;
			_input.restrict = "0123456789ABCDEFabcdef";
			_input.maxChars = 6;
			addChild(_input);
			_input.addEventListener(Event.CHANGE, onChange);
			
			_swatch = new Sprite();
			_swatch.x = 50;
			_swatch.filters = [getShadow(2, true)];
			addChild(_swatch);
			
		}
		
		
		
		
		///////////////////////////////////
		// public methods
		///////////////////////////////////
		
		/**
		 * Draws the visual ui of the component.
		 */
		override public function draw():void
		{
			super.draw();
			_value = parseInt("0x" + _input.text, 16);
			_swatch.graphics.clear();
			_swatch.graphics.beginFill(_value);
			_swatch.graphics.drawRect(0, 0, 16, 16);
			_swatch.graphics.endFill();
		}
		
		
		
		
		///////////////////////////////////
		// event handlers
		///////////////////////////////////
		
		/**
		 * Internal change handler.
		 * @param event The Event passed by the system.
		 */
		private function onChange(event:Event):void
		{
			_input.text = _input.text.toUpperCase();
			invalidate();
		}
		
		///////////////////////////////////
		// getter/setters
		///////////////////////////////////
		
		/**
		 * Gets / sets the color value of this ColorChooser.
		 */
		public function set value(n:uint):void
		{
			var str:String = n.toString(16).toUpperCase();
			while(str.length < 6)
			{
				str = "0" + str;
			}
			_input.text = str;
			invalidate();
		}
		public function get value():uint
		{
			return _value;
		}
	}
}
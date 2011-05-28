/**
 * CheckBox.as
 * Keith Peters
 * version 0.91
 * 
 * A basic CheckBox component.
 * 
 * Source code licensed under a Creative Commons Attribution-Share Alike 3.0 License.
 * http://creativecommons.org/licenses/by-sa/3.0/
 * Some Rights Reserved.
 */
 
package com.bit101.components
{
	import flash.display.DisplayObjectContainer;
	import flash.display.Sprite;
	import flash.events.MouseEvent;
	
	public class CheckBox extends Component
	{
		private var _back:Sprite;
		private var _button:Sprite;
		private var _label:Label;
		private var _labelText:String = "";
		private var _selected:Boolean = false;
		
		
		/**
		 * Constructor
		 * @param parent The parent DisplayObjectContainer on which to add this CheckBox.
		 * @param xpos The x position to place this component.
		 * @param ypos The y position to place this component.
		 * @param label String containing the label for this component.
		 * @param defaultHandler The event handling function to handle the default event for this component (click in this case).
		 */
		public function CheckBox(parent:DisplayObjectContainer = null, xpos:Number = 0, ypos:Number =  0, label:String = "", defaultHandler:Function = null)
		{
			_labelText = label;
			super(parent, xpos, ypos);
			if(defaultHandler != null)
			{
				addEventListener(MouseEvent.CLICK, defaultHandler);
			}
		}
		
		/**
		 * Initializes the component.
		 */
		override protected function init():void
		{
			super.init();
			buttonMode = true;
			useHandCursor = true;
			setSize(10, 10);
		}
		
		/**
		 * Creates the children for this component
		 */
		override protected function addChildren():void
		{
			_back = new Sprite();
			_back.filters = [getShadow(2, true)];
			addChild(_back);
			
			_button = new Sprite();
			_button.filters = [getShadow(1)];
			_button.visible = false;
			addChild(_button);
			
			_label = new Label();
			addChild(_label);
			
			addEventListener(MouseEvent.CLICK, onClick);
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
			_back.graphics.clear();
			_back.graphics.beginFill(Style.BACKGROUND);
			_back.graphics.drawRect(0, 0, _width, _height);
			_back.graphics.endFill();
			
			_button.graphics.clear();
			_button.graphics.beginFill(Style.BUTTON_FACE);
			_button.graphics.drawRect(2, 2, _width - 4, _height - 4);
			
			_label.text = _labelText;
			_label.x = _width + 2;
			_label.y = _height / 2 - _label.height / 2;
		}
		
		
		
		
		///////////////////////////////////
		// event handler
		///////////////////////////////////
		
		/**
		 * Internal click handler.
		 * @param event The MouseEvent passed by the system.
		 */
		private function onClick(event:MouseEvent):void
		{
			_selected = !_selected;
			_button.visible = _selected;
		}
		
		
		
		
		///////////////////////////////////
		// getter/setters
		///////////////////////////////////
		
		/**
		 * Sets / gets the label text shown on this CheckBox.
		 */
		public function set label(str:String):void
		{
			_labelText = str;
			invalidate();
		}
		public function get label():String
		{
			return _labelText;
		}
		
		/**
		 * Sets / gets the selected state of this CheckBox.
		 */
		public function set selected(s:Boolean):void
		{
			_selected = s;
			_button.visible = _selected;
		}
		public function get selected():Boolean
		{
			return _selected;
		}

	}
}
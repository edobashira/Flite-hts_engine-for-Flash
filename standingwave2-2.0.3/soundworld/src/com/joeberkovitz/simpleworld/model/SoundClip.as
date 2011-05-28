package com.joeberkovitz.simpleworld.model
{
    import com.joeberkovitz.simpleworld.editor.SoundRenderer;
    
    import flash.events.Event;
    import flash.geom.Point;
    import flash.geom.Rectangle;
    import flash.media.Sound;
    import flash.net.URLRequest;
        
    /**
     * Value object representing a sound clip. 
     */
    [Event(type="flash.events.Event",name="complete")]
    [RemoteClass]
    public class SoundClip extends SonicElement
    {
        [Bindable]
        public var height:Number;

        private var _url:String;
        private var _sound:Sound;
        private var _soundComplete:Boolean = false;
        
        [Bindable]
        public function get url():String
        {
            return _url;
        } 
        
        public function set url(value:String):void
        {
            _url = value;
            _sound = new Sound(new URLRequest(url));
            _sound.addEventListener(Event.COMPLETE, handleSoundComplete);
        }
        
        public function get sound():Sound
        {
            return _sound;
        }
        
        public function get complete():Boolean
        {
            return _soundComplete;
        }
        
        public function get duration():Number
        {
            return _sound.length / 1000;
        }
        
        private function handleSoundComplete(e:Event):void
        {
            _soundComplete = true;
            dispatchEvent(e);
        }

        public function get width():Number
        {
            if (_soundComplete)
            {
                return Math.floor(duration * SoundRenderer.PIX_PER_SECOND / SoundRenderer.yToFactor(y));
            }
            else
            {
                return 1;
            }
        }
              
        public function get cornerX():Number
        {
            return x + width; 
        }
        
        public function get cornerY():Number
        {
            return y + height; 
        }
        
        public function get corner():Point
        {
            return new Point(cornerX, cornerY);
        }
        
        public function set corner(p:Point):void
        {
            height = p.y - y;
        }

        override public function get bounds():Rectangle
        {
            return new Rectangle(x, y, width, height);
        }
    }
}

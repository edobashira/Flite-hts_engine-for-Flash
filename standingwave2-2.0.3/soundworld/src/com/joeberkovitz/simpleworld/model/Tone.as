package com.joeberkovitz.simpleworld.model
{
    import flash.geom.Point;
    import flash.geom.Rectangle;
        
    /**
     * Value object representing a sine tone. 
     */
    [RemoteClass]
    public class Tone extends SonicElement
    {
        [Bindable]
        public var width:Number;
        
        [Bindable]
        public var height:Number;
        
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
            width = p.x - x;
            height = p.y - y;
        }
 
        override public function get bounds():Rectangle
        {
            return new Rectangle(x, y, width, height);
        }
     }
}

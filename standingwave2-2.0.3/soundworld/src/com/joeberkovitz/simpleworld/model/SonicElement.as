package com.joeberkovitz.simpleworld.model
{
    import flash.events.EventDispatcher;
    import flash.geom.Rectangle;
    
    /**
     * Abstract value object representing some sonic element in the composition.
     */
    [RemoteClass]
    public class SonicElement extends EventDispatcher
    {
        [Bindable]
        public var color:uint;
        
        [Bindable]
        public var x:Number = 0;

        [Bindable]
        public var y:Number = 0;
        
        public function get bounds():Rectangle
        {
            throw new Error("bounds() not overridden");
        }
    }
}
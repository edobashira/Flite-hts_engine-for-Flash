package com.joeberkovitz.simpleworld.model
{
    import mx.collections.ArrayCollection;
    import mx.collections.IList;
    
    /**
     * Value object representing a set of sonic elements in a 2-dimensional graphical composition.
     */
    [RemoteClass]
    public class Composition
    {
        public static const MOCCASIN_CHILDREN_PROPERTY:String = "elements";
        
        [Bindable]
        public var width:Number = 2000;

        [Bindable]
        public var height:Number = 2000;

        [Bindable]
        public var elements:IList = new ArrayCollection();   
    }
}
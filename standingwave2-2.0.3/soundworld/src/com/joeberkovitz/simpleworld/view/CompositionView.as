package com.joeberkovitz.simpleworld.view
{
    import com.joeberkovitz.moccasin.model.MoccasinModel;
    import com.joeberkovitz.moccasin.view.IMoccasinView;
    import com.joeberkovitz.moccasin.view.MoccasinView;
    import com.joeberkovitz.moccasin.view.ViewContext;
    import com.joeberkovitz.simpleworld.controller.CompositionMediator;
    import com.joeberkovitz.simpleworld.editor.SoundRenderer;
    import com.joeberkovitz.simpleworld.model.Composition;
    import com.joeberkovitz.simpleworld.model.SoundClip;
    import com.joeberkovitz.simpleworld.model.Tone;
    
    import flash.utils.getQualifiedClassName;

    /**
     * View of the entire sample application's Composition value object.
     */
    public class CompositionView extends MoccasinView
    {
        public function CompositionView(context:ViewContext, model:MoccasinModel=null)
        {
            super(context, model);
            initialize();
        }
        
        /**
         * The World of which this is a view.
         */
        public function get world():Composition
        {
            return model.value as Composition;
        }
        
        /**
         * Initialize this object by creating the appropriate mediator to handle events.
         */
        override protected function initialize():void
        {
            super.initialize();
            new CompositionMediator(context).handleViewEvents(this);
        }
        
        /**
         * Update the view by drawing the backdrop for the world.
         */
        override protected function updateView():void
        {
            super.updateView();
            
            graphics.beginFill(0xFFFFFF);
            graphics.drawRect(0, 0, world.width, world.height);
            graphics.endFill();
            
            graphics.lineStyle(1, 0x999999);
            graphics.moveTo(0, SoundRenderer.CENTER_Y);
            graphics.lineTo(world.width, SoundRenderer.CENTER_Y);
        }

        /**
         * Create a child view for some child model of our model. 
         * @param child a child MoccasinModel
         * @return the appropriate type of view for the value object belonging to that child.
         */
        override public function createChildView(child:MoccasinModel):IMoccasinView
        {
            if (child.value is SoundClip)
            {
                return new SoundClipView(context, child);
            }
            else if (child.value is Tone)
            {
                return new ToneView(context, child);
            }
            else
            {
                throw new Error("Unrecognized model type: " + getQualifiedClassName(child));
            }
        }
    }
}
package com.joeberkovitz.simpleworld.view
{
    import com.joeberkovitz.moccasin.model.MoccasinModel;
    import com.joeberkovitz.moccasin.view.ViewContext;
    import com.joeberkovitz.simpleworld.model.Tone;
    
    import flash.display.DisplayObject;

    /**
     * View of a Tone value object in the world. 
     */
    public class ToneView extends SonicElementView
    {
        public function ToneView(context:ViewContext, model:MoccasinModel=null)
        {
            super(context, model);
            initialize();
        }
        
        /**
         * The Tone object associated with this view's MoccasinModel.
         */
        public function get tone():Tone
        {
            return model.value as Tone;
        }
        
        /**
         * Update this view by drawing the appropriate graphics.
         */
        override protected function updateView():void
        {
            super.updateView();
            
            graphics.beginFill(0);
            graphics.drawRect(0, 0, tone.width, tone.height);
            graphics.endFill();
        }
        
        /**
         * Create a feedback view that will be superimposed on this view in a transparent layer.
         */        
        override protected function createFeedbackView():DisplayObject
        {
            return new ToneFeedback(context, model);
        }
    }
}

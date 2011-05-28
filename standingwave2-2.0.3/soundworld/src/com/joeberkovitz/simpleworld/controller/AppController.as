package com.joeberkovitz.simpleworld.controller
{
    import com.joeberkovitz.moccasin.controller.MoccasinController;
    import com.joeberkovitz.moccasin.document.ISelection;
    import com.joeberkovitz.moccasin.document.MoccasinDocument;
    import com.joeberkovitz.moccasin.document.ObjectSelection;
    import com.joeberkovitz.moccasin.event.ModelUpdateEvent;
    import com.joeberkovitz.moccasin.model.MoccasinModel;
    import com.joeberkovitz.simpleworld.model.SoundClip;
    import com.joeberkovitz.simpleworld.model.Composition;
    import com.joeberkovitz.simpleworld.model.SonicElement;
    
    import flash.events.Event;
    import flash.geom.Point;

    /**
     * Application specific subclass of MoccasinController.  This class is responsible
     * for applying modifications to the application model on behalf of the presentation layer. 
     */
    public class AppController extends MoccasinController
    {
        public function AppController(document:MoccasinDocument)
        {
            super(document);
        }
        
        /**
         * The World that is the root value object of our document model.
         */
        public function get world():Composition
        {
            return document.root.value as Composition;
        }
        
        /**
         * Transform pasted model objects appropriately, in this case offsetting them so that they
         * appear distinctly from the originals (if present).
         */
        override protected function transformPastedModel(model:MoccasinModel):MoccasinModel
        {
            if (model.value is SonicElement)
            {
                SonicElement(model.value).x += 10;
                SonicElement(model.value).y += 10;
            }
            return model;
        }
        
    }
}

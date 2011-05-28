package com.joeberkovitz.simpleworld.view
{
    import com.joeberkovitz.moccasin.model.MoccasinModel;
    import com.joeberkovitz.moccasin.view.SelectionHandle;
    import com.joeberkovitz.moccasin.view.ViewContext;
    import com.joeberkovitz.simpleworld.controller.PointDragMediator;
    import com.joeberkovitz.simpleworld.model.Tone;
    
    import flash.display.Sprite;

    /**
     * Feedback variation of a ToneView that displays associated resizing handle in the editor's
     * feedbackLayer UIComponent. Note that this extends SonicElementView and thus inherits its position-tracking behavior.
     */
    public class ToneFeedback extends SonicElementView
    {
        private var _sizeHandle:SelectionHandle;
        private var _outline:Sprite;
        
        public function ToneFeedback(context:ViewContext, model:MoccasinModel=null)
        {
            super(context, model, false);
            initialize();
            
            new PointDragMediator(context, "corner").handleViewEvents(this, _sizeHandle);
        }
        
        public function get tone():Tone
        {
            return model.value as Tone;
        }
        
        override protected function createChildren():void
        {
            super.createChildren();

            // create the resize handle
            _sizeHandle = new SelectionHandle(context);
            addChild(_sizeHandle);
            
            _outline = new Sprite();
            addChild(_outline);
        }

        override protected function updateView():void
        {
            super.updateView();
                        
            // Draw a gray selection border around the square
            _outline.graphics.clear();
            _outline.graphics.lineStyle(1, 0x999999);
            _outline.graphics.drawRect(0, 0, tone.width, tone.height);

            // reposition the resizing handle
            _sizeHandle.x = tone.cornerX - tone.x;
            _sizeHandle.y = tone.cornerY - tone.y;
        }
    }
}
package com.joeberkovitz.simpleworld.view
{
    import com.joeberkovitz.moccasin.model.MoccasinModel;
    import com.joeberkovitz.moccasin.view.SelectionHandle;
    import com.joeberkovitz.moccasin.view.ViewContext;
    import com.joeberkovitz.simpleworld.controller.PointDragMediator;
    import com.joeberkovitz.simpleworld.model.SoundClip;
    
    import flash.display.Sprite;
    import flash.geom.Point;

    /**
     * Feedback variation of a SoundClipView that displays associated resizing handle in the editor's
     * feedbackLayer UIComponent. Note that this extends SoundClipView and thus inherits its position-tracking behavior.
     */
    public class SoundClipFeedback extends SonicElementView
    {
        private var _sizeHandle:SelectionHandle;
        private var _outline:Sprite;
        
        public function SoundClipFeedback(context:ViewContext, model:MoccasinModel=null)
        {
            super(context, model, false);
            initialize();
            
            new PointDragMediator(context, "corner").handleViewEvents(this, _sizeHandle);
        }
        
        public function get clip():SoundClip
        {
            return model.value as SoundClip;
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
            _outline.graphics.drawRect(0, 0, clip.width, clip.height);

            // reposition the resizing handle
            _sizeHandle.x = clip.cornerX - clip.x;
            _sizeHandle.y = clip.cornerY - clip.y;
        }
    }
}
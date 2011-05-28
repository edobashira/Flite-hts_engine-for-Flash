package com.joeberkovitz.simpleworld.view
{
    import com.joeberkovitz.moccasin.model.MoccasinModel;
    import com.joeberkovitz.moccasin.view.ViewContext;
    import com.joeberkovitz.simpleworld.model.SoundClip;
    import com.noteflight.standingwave2.elements.IAudioSource;
    import com.noteflight.standingwave2.elements.Sample;
    import com.noteflight.standingwave2.filters.ResamplingFilter;
    import com.noteflight.standingwave2.sources.SoundSource;
    
    import flash.display.DisplayObject;
    import flash.events.Event;

    /**
     * View of a SoundClip value object in the world. 
     */
    public class SoundClipView extends SonicElementView
    {
        public function SoundClipView(context:ViewContext, model:MoccasinModel=null)
        {
            super(context, model);
            clip.addEventListener(Event.COMPLETE, handleComplete);
            initialize();
        }
        
        /**
         * The Square object associated with this view's MoccasinModel.
         */
        public function get clip():SoundClip
        {
            return model.value as SoundClip;
        }
        
        /**
         * Update this view by drawing the appropriate graphics.
         */
        override protected function updateView():void
        {
            super.updateView();
            
            // draw a frame around the sound
            graphics.lineStyle(1, 0);
            graphics.beginFill(0, 0.1);
            graphics.drawRect(0, 0, clip.width, clip.height)
            graphics.endFill();
            
            if (clip.complete)
            {
                graphics.moveTo(0, clip.height / 2);
                var source:IAudioSource = new SoundSource(clip.sound);
                const DISPLAY_FACTOR:Number = 100;
                var pixSample:Sample =
                    new ResamplingFilter(source, DISPLAY_FACTOR)
                        .getSample(Math.floor(source.frameCount / DISPLAY_FACTOR));
                    
                for (var i:int = 0; i < pixSample.frameCount; i++)
                {
                    graphics.lineTo(i * clip.width / pixSample.frameCount, (pixSample.channelData[0][i] * clip.height / 2) + clip.height / 2);
                }
            }
        }
        
        /**
         * Create a feedback view that will be superimposed on this view in a transparent layer.
         */        
        override protected function createFeedbackView():DisplayObject
        {
            return new SoundClipFeedback(context, model);
        }
        
        private function handleComplete(e:Event):void
        {
            updateView();
        }
    }
}

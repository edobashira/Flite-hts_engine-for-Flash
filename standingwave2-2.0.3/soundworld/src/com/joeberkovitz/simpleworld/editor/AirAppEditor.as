package com.joeberkovitz.simpleworld.editor
{
    import com.joeberkovitz.moccasin.controller.IMoccasinController;
    import com.joeberkovitz.moccasin.editor.AirMoccasinEditor;
    import com.joeberkovitz.moccasin.editor.EditorKeyMediator;
    import com.joeberkovitz.moccasin.model.ModelRoot;
    import com.joeberkovitz.moccasin.service.IMoccasinDocumentService;
    import com.joeberkovitz.moccasin.service.MoccasinDocumentData;
    import com.joeberkovitz.moccasin.view.IMoccasinView;
    import com.joeberkovitz.moccasin.view.ViewContext;
    import com.joeberkovitz.simpleworld.controller.AppController;
    import com.joeberkovitz.simpleworld.model.Composition;
    import com.joeberkovitz.simpleworld.service.AirAppDocumentService;
    import com.joeberkovitz.simpleworld.view.CompositionView;
    import com.noteflight.standingwave2.output.AudioPlayer;
    import com.noteflight.standingwave2.performance.AudioPerformer;
    import com.noteflight.standingwave2.performance.IPerformance;
    
    import flash.display.Sprite;
    
    import mx.binding.utils.BindingUtils;
    

    /**
     * AIR version of the AppEditor/
     */
    public class AirAppEditor extends AirMoccasinEditor
    {
        private var _player:AudioPlayer;
        private var _playbackCursor:Sprite;

        private var _renderer:SoundRenderer;
        
        override public function initializeEditor():void
        {
            super.initializeEditor();
            loadFromDocumentData(new MoccasinDocumentData(new ModelRoot(new Composition()), null));
            _player = new AudioPlayer();
            
            _playbackCursor = new Sprite();
            _playbackCursor.graphics.lineStyle(1, 0);
            _playbackCursor.graphics.moveTo(0, 0);
            _playbackCursor.graphics.lineTo(0, 1000);
            feedbackLayer.addChild(_playbackCursor);
            
            _renderer = new SoundRenderer();
            
            BindingUtils.bindProperty(this, "playbackPosition", _player, "position");
        }
        
        public function get renderer():SoundRenderer
        {
            return _renderer;
        }
        
        public function set playbackPosition(value:Number):void
        {
            _playbackCursor.x = value * SoundRenderer.PIX_PER_SECOND;
        }
        
        /**
         * Override base class to create application-specific controller. 
         */
        override protected function createController():IMoccasinController
        {
            return new AppController(null);
        }
        
        /**
         * Override base class to create application-specific keystroke mediator. 
         */
        override protected function createKeyMediator(controller:IMoccasinController):EditorKeyMediator
        {
            return new AppKeyMediator(controller, this);
        }
        
        /**
         * Override base class to create application-specific top-level view of model. 
         */
        override protected function createDocumentView(context:ViewContext):IMoccasinView
        {
            return new CompositionView(context, controller.document.root);
        } 

        /**
         * Override base class to instantiate application-specific service to load and save documents. 
         */
        override protected function createDocumentService():IMoccasinDocumentService
        {
            return new AirAppDocumentService();
        }
        
        public function startPlayback():void
        {
            var p:IPerformance;
            p = _renderer.renderComposition(Composition(moccasinDocument.root.value));
            _player.play(new AudioPerformer(p));
        }
        
        public function stopPlayback():void
        {
            _player.stop();
        }
    }
}
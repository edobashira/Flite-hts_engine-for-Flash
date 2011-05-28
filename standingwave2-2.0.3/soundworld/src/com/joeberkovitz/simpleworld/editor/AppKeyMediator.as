package com.joeberkovitz.simpleworld.editor
{
    import com.joeberkovitz.moccasin.controller.IMoccasinController;
    import com.joeberkovitz.moccasin.editor.EditorKeyMediator;
    import com.joeberkovitz.moccasin.editor.MoccasinEditor;
    
    import flash.events.KeyboardEvent;
    
    /**
     * Application specific keystroke handler.  This is currently just a skeleton; there
     * are no application-specific keystrokes.
     */
    public class AppKeyMediator extends EditorKeyMediator
    {
        
        public function AppKeyMediator(controller:IMoccasinController, editor:MoccasinEditor)
        {
            super(controller, editor);
        }
        
        override public function handleKey(e:KeyboardEvent):void
        {
            var ch:String = String.fromCharCode(e.charCode);
            switch (ch)
            {
            default:
                super.handleKey(e);
                return;
            }
            
            e.stopImmediatePropagation();
            e.stopPropagation();
        }
    }
}
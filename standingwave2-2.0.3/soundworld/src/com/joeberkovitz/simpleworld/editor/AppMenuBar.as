package com.joeberkovitz.simpleworld.editor
{
    import com.joeberkovitz.moccasin.editor.EditorMenuBar;
    import com.joeberkovitz.moccasin.event.SelectEvent;
    import com.joeberkovitz.simpleworld.controller.AppController;
    
    /**
     * Application specific menu bar.
     */
    public class AppMenuBar extends EditorMenuBar
    {
        public function AppMenuBar()
        {
            super();
        }
        
        public function get simpleController():AppController
        {
            return editor.controller as AppController;
        }
        
        public function get simpleEditor():AirAppEditor
        {
            return editor as AirAppEditor;
        }
        
        override protected function initializeMenuItems():void
        {
            super.initializeMenuItems();
            
            menuBarDefinition +=
                <menuitem id="performance" label="Performance">
                    <menuitem id="play" label="Play"/>
                </menuitem>;
        }
        
        override protected function handleChangeSelection(evt:SelectEvent):void
        {
        }
        
        override protected function handleCommand(commandName:String):void
        {
            switch(commandName)
            {
            case "play":
                simpleEditor.startPlayback();
                break;
            case "stop":
                simpleEditor.stopPlayback();
                break;
                
            default:
                super.handleCommand(commandName);
            }
        }
    }
}

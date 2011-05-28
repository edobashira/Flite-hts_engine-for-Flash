package com.joeberkovitz.simpleworld.editor
{
    import com.joeberkovitz.moccasin.editor.EditorMenuBar;
    import com.joeberkovitz.simpleworld.controller.AppController;
    
    /**
     * Application specific menu bar.
     */
    public class AirAppMenuBar extends AppMenuBar
    {
        public function AirAppMenuBar()
        {
            super();
        }
        
        public function get airEditor():AirAppEditor
        {
            return editor as AirAppEditor;
        }
        
        override protected function initializeMenuItems():void
        {
            super.initializeMenuItems();
            
            var fileMenu:XML = menuBarDefinition.(@id == "file")[0];
            fileMenu.insertChildBefore(fileMenu.menuitem.(@id == "save"), <menuitem id="open" label="Open..."/>);
            fileMenu.insertChildAfter(fileMenu.menuitem.(@id == "save"), <menuitem id="saveAs" label="Save As..."/>);
        }
        
        override protected function handleCommand(commandName:String):void
        {
            switch(commandName)
            {
            case "open":
                airEditor.openFile();
                break;
                
            case "saveAs":
                airEditor.saveAsFile();
                break;

            default:
                super.handleCommand(commandName);
            }
        }
    }
}
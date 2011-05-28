package com.joeberkovitz.simpleworld.view
{
    import com.joeberkovitz.moccasin.model.MoccasinModel;
    import com.joeberkovitz.moccasin.view.SelectableView;
    import com.joeberkovitz.moccasin.view.ViewContext;
    import com.joeberkovitz.simpleworld.controller.ElementDragMediator;
    import com.joeberkovitz.simpleworld.model.SonicElement;

    /**
     * View of a SonicElement value object in this application. 
     */
    public class SonicElementView extends SelectableView
    {
        private var _allowDrag:Boolean;
        
        /**
         * Create a view of a WorldShape. 
         * 
         * @param context the ViewContext shared by all views in this document view
         * @param model a MoccasinModel whose value object is the sonic element to be displayed
         * @param allowDrag an optional flag controlling the draggability of this view.
         * 
         */
        public function SonicElementView(context:ViewContext, model:MoccasinModel, allowDrag:Boolean = true)
        {
            super(context, model);
            _allowDrag = allowDrag;
        }
        
        /**
         * The element that this view presents.
         */
        public function get element():SonicElement
        {
            return model.value as SonicElement;
        }
        
        /**
         * Initialize this view by adding a mediator for drag handling, if specified.
         */
        override protected function initialize():void
        {
            super.initialize();
            if (_allowDrag)
            {
                new ElementDragMediator(context).handleViewEvents(this);
            }
        }
        
        /**
         * Update this view by adjusting its position.
         */
        override protected function updateView():void
        {
            super.updateView();
            
            x = element.x;
            y = element.y;
        }
    }
}
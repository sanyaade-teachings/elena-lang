#define gui'dictionary'*.
#define gui'dictionary'events'*.
#define std'basic'*.
#define std'dictionary'*.
//#define sys'dictionary'*.
#define sys'io'*.
#define gui'*      = gui'common'*.
#define gui'dialogs'*.

#define controls'* = gui'controls'*.
#define forms'*    = gui'forms'*.

#symbol TextFile : aPath =
{
    write : aText
    [
        #var aWriter := NewTextFileWriter::aPath.
        aWriter write:aText.
        aWriter free.
    ]

    read : aText
    [
        #var aContent := String.
        #var aReader := TextFileReader::aPath.
        #loop aReader isnot &eof
        [
            #var aLine := String.
            aReader >> aLine.

            aContent += aLine += "%r%n".
        ].
        aReader free.

        aText << aContent.
    ]
}.

#symbol OpenDialog : anOwner
    = OpenFileDialog &&file_must_exist:true &control_parent:anOwner &caption:"Open File" 
            &filter_caption:"Text Files" &filter_mask:"*.txt".

#symbol SaveDialog : anOwner
    = SaveFileDialog &&file_must_exist:false&control_parent:anOwner &caption:"Open File" 
            &filter_caption:"Text Files" &filter_mask:"*.txt".

// --- OpenFile ---

#class window(forms'SDI)
{
    #field theMenu.
    #field theStatusBar.
    #field theTextField.
    
    #field theLayoutManager.

    #method oninit : Args
    [
        super oninit:Args.
        
        theMenu := controls'Menu::self.
        theStatusBar := controls'StatusBar::self.
        theTextField := controls'Memo::self.
        theLayoutManager := controls'LayoutManager::self.

        theMenu 
            append &menu_item &caption:"File".

        #var aFileMenu := theMenu@0.
        aFileMenu
            append &menu_item &caption:"Open"
            append &menu_item &caption:"Save".

        (aFileMenu@0)~gui'eevents +=
        {
            onmenu_click'eval
            [
                #var aDialog := OpenDialog::self.
                #if aDialog open &forms'modal
                [
                    TextFile::(aDialog path) read:theTextField.
                ].
            ]
        }.

        (aFileMenu@1)~gui'eevents +=
        {
            onmenu_click'eval
            [
                #var aDialog := SaveDialog::self.
                #if aDialog open &forms'modal
                [
                    TextFile::(aDialog path) write:theTextField.
                ].
            ]
        }.

        self
            set &x:200 &y:200
            set &width:700 &height:600
            set &caption:"Notepad"
            maximized'open.

        theTextField
            set &x:0 &y:46 
            set &width:693 & height:500
            open.

        theStatusBar
            set &x:0 &y:300 
            set &width:700 &height:20.
            
        theStatusBar
            append &control_item &caption:"Notepad sample"
            append &control_item &caption:"".
            
        (theStatusBar @ 0) set &width:204.
        (theStatusBar @ 1) set &width:690.
                       
        theStatusBar open.

        theLayoutManager set &controls'dock_center:theTextField &controls'dock_bottom:theStatusBar.
        
//        theTextField~gui'eevents +=
//        {
//            onSelectionChange : Args
//            [
//            ]
//        }.
    ]
}

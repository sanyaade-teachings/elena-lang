#define system.

// --- LRLN ---

#symbol newLine
    = "%r%n".

// --- $ConsoleHandler --

#class $ConsoleHandler : system'Handler
{
    #constructor new &type'int:anAttributes
    [
        theData << system'external'KERNEL32 GetStdHandle 
                      &type'int        : anAttributes.
    ]
}

// --- ConsoleWriter ---

#class $ConsoleWriter
{
    #field theHandler.
    
    #constructor new &type'handle:aHandler 
    [
        theHandler := aHandler.
    ]
    
    #method write &type'widestr : aLiteral
    [
        system'external'KERNEL32 WriteConsoleW
            &type'handle  : theHandler
            &type'widestr : aLiteral
            &type'int     : (aLiteral type'length)
            &type'int     : 0                       // !! temporally we do not use it
            &type'int     : 0.             

        // !!check the result of the operation
    ]
}

// --- ConsoleReader ---

#class $ConsoleReader
{
    #field theHandler.
    
    #constructor new &type'handle:aHandler 
    [
        theHandler := aHandler.
    ]

    #method read &type'char
    [
        #var aDump := ByteArray new &type'length:20.

        control do:
        [
            system'external'KERNEL32 ReadConsoleInputW
                &type'handle        : theHandler
                &out'type'bytearray : aDump
                &type'int           : 1
                &out'type'int       : nil.
        ]
        &until:[(system'external'CoreAPI equal &type'bytearray:aDump &type'index:0 &type'short:1)
                    and:(system'external'CoreAPI equal &type'bytearray:aDump &type'index:4 &type'int:1)].
                    
        ^ CharValue new &type'short:(system'external'CoreAPI read &type'bytearray:aDump &type'index:14).
    ]

    #method read &type'int
    [
        #var aDump := ByteArray new &type'length:20.

        control do:
        [
            system'external'KERNEL32 ReadConsoleInputW
                &type'handle        : theHandler
                &out'type'bytearray : aDump
                &type'int           : 1
                &out'type'int       : nil.
        ]
        &until:[(system'external'CoreAPI equal &type'bytearray:aDump &type'index:0 &type'short:1)
                    and:(system'external'CoreAPI equal &type'bytearray:aDump &type'index:4 &type'int:1)].
                    
        ^ IntNumber new &type'int:(system'external'CoreAPI read &type'bytearray:aDump &type'index:8).
    ]

    // !! HOTFIX: additional method to pass the literal type
    #method $read &type'widestr:aLiteral &type'length:aLength
    [
        system'external'KERNEL32 ReadConsoleW
            &type'handle      : theHandler
            &out'type'widestr : aLiteral
            &type'int         : aLength
            &out'type'length  : aLiteral
            &type'int         : 0.
    ]

    #method read &type'widestr &type'length:aLength
    [
        #var aLiteral := LiteralValue new &type'length:aLength.

        $self $read &type'widestr:aLiteral &type'length:aLength.
        
        ^ aLiteral.
    ]
    
    #method read &Line
    [
        #var anOutput := emptyLiteralValue.
        
        control do:
        [
            #var aTemp := $self read &type'widestr &type'length:64.
            
            anOutput := anOutput + aTemp.
        ] 
        &until:[ system'external'CoreAPI _trimLine &out'type'widestr:anOutput ].
        
        ^ anOutput.
    ]
}

// --- Console Standard Symbols

#symbol stdConsoleWriter = $ConsoleWriter new &type'handle:($ConsoleHandler new &type'int:0FFFFFFF5h).

#symbol stdConsoleReader = $ConsoleReader new &type'handle:($ConsoleHandler new &type'int:0FFFFFFF6h).
#define system.
#define extensions.

// --- Token ---

#class Token
{
    #field theValue.
    
    #constructor new
    [
        theValue := String new.
    ]
    
    #constructor new : aValue
    [
        theValue := String new:aValue.
    ]
    
    #method ParseOrder = 0.
    
    #method append : aChar
    [
        theValue += aChar.
    ]
    
    #method add : aNode
    [
        ^ aNode += self.
    ]

    #method Number = convertor toReal:theValue.    
}

// --- Node ---

#class Node
{
    #field theLeft.
    #field theRight.
    #field theState.
    
    #method setRight : aNode
    [
        theRight := aNode.
        
        theState := %appendRight.
    ]
    
    #method setLeft : aNode
    [
        theLeft := aNode.
        
        theState := %setRight.
    ]
    
    #constructor new
    [
        theState := %setLeft.
    ]

    #method add : aNode
        = (self ParseOrder > aNode ParseOrder)
            ? [
                self += aNode.
                
                ^ self.
            ]
            ! [
                aNode += self.
                
                ^ aNode.
            ].
        
    #method appendRight : aNode
    [
        (theRight ParseOrder > aNode ParseOrder)
        ? [
            theRight += aNode.
        ]
        ! [
            theRight := aNode += theRight.
        ].        
    ]
    
    #method append : anObject
        = $self~theState eval:anObject. 
    
    #method => theState.
}

// --- SummaryNode

#class SummaryNode : Node
{
    #method ParseOrder = 2.
    
    #method Number = theLeft Number + theRight Number.
}

// --- DifferenceNode ---

#class DifferenceNode : Node
{
    #method ParseOrder = 2.
    
    #method Number = theLeft Number - theRight Number.
}

// --- ProductNode ---

#class ProductNode : Node
{
    #method ParseOrder = 1.
    
    #method Number = theLeft Number * theRight Number.
}

// --- FractionNode ---

#class FractionNode : Node
{
    #method ParseOrder = 1.
    
    #method Number = theLeft Number / theRight Number.
}

// --- SubExpression ---

#class SubExpression
{
    #field theParser.
    #field theCounter.
    
    #constructor new
    [
        theParser := arithmeval'Parser new.
        theCounter := Integer new:1.
    ]

    #method ParseOrder = 0.
    
    #method add : aNode
    [
        ^ aNode += self.
    ]
    
    #method validate
    [
        (theCounter < 0)
            ? [ #throw Exception new:"Invalid expression". ].
            
        ^ (0 == theCounter).
    ]
    
    #method append : aChar
    [
        aChar =>
            41 ? [ 
                theCounter -= 1. 
            ]
            40 ? [ theCounter += 1 ]
            ! [ theParser evaluate:aChar ].
    ]

    #method Number
        = $self validate
            ? [ theParser Number ]
            ! [ #throw Exception new:"Invalid expression". ].
}

// ---- Parser ----

#class Parser : system'routines'BasePattern
{
    #field theToken.
    #field theTopNode.
    #field theState.
    
    #method onBrackets : aChar
    [
        theToken += aChar.
                
        (theToken validate)
          ? [
              theState := %onDigit.
          ].
    ]

    #method onStart : aChar
    [
        aChar =>
             40 ? [ // (
                theToken := SubExpression new.
                theTopNode := theToken.
  
                theState := %onBrackets.
            ]
            45 ? [ // -
                theToken := DifferenceNode new add:(Token new:"0").
                
                theTopNode := theToken.
                
                theState := %onOperator.
            ]
            ! [
                theToken := Token new.
                theTopNode := theToken.
                theState := %onDigit.
                
                $self appendDigit:aChar.                
            ].
    ]

    #method onOperator : aChar
    [
        aChar =>
            40 ? [
                theToken := SubExpression new.
                theTopNode += theToken.
                theState := %onBrackets.
            ]
            ! [
                theToken := Token new.
                theTopNode += theToken.
                theState := %onDigit.

                $self appendDigit:aChar.
            ].
    ]

    #constructor new
    [
        theState := %onStart.
    ]

    #method Number = theTopNode Number.

    #method appendDigit : aChar
    [
        (aChar >= 48) and:(aChar < 58)
        ? [
            theToken += aChar.
        ]
        ! [
            #throw Exception new:"Invalid expression".
        ]
        
    ]

    #method onDigit : aChar
    [
        aChar =>
            40 ? [      // (
                theToken := SubExpression new.
                theTopNode := theToken.
                theState := %onBrackets.
            ]
            42 ? [      // *
                theTopNode := theTopNode + ProductNode new.

                theState := %onOperator.
            ]
            43 ? [      // +
                theTopNode := theTopNode + SummaryNode new.

                theState := %onOperator.
            ]
            45 ? [      // -
                theTopNode := theTopNode + DifferenceNode new.

                theState := %onOperator.
            ]
            47 ?       // /
            [
                theTopNode := theTopNode + FractionNode new.

                theState := %onOperator.
            ]
            ! [
                $self appendDigit:aChar.
            ].
    ]

    #method eval : aChar = $self~theState eval:aChar.
}

#symbol program =
[
    #var aText := String new.

    control while:(consoleEx readLine:aText length > 0) &do:
    [
        #var aParser := Parser new.

        consoleEx writeLine:"=" :(aParser foreach:aText Number)
            | ifFailed: 
                [
                    consoleEx writeLine:"Invalid Expression".
                ].
    ].
].
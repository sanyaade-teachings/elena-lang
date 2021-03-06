// --- true ---

#symbol true =
{
    then : aTrueAction = aTrueAction eval.

    then : aTrueAction &else:aFalseAction = aTrueAction eval.

    else : aTrueAction = nil.

    or : anExpr = $self.

    and : anExpr => &and::anExpr.

    xor : anExpr => &xor::anExpr.

    and &bool:aBool = aBool.

    and &action:aBoolExpr = aBoolExpr eval bool.

    and &int:anInt 
        = 0 != anInt.
                
    xor &bool:aBool = aBool ? [ false ] ! [ true ].
    
    xor &action:aBoolExpr = aBoolExpr eval ? [ false ] ! [ true ].
            
    equal : aBool => &equal::aBool.
    
    equal &bool:aBool = aBool ? [ true ] ! [ false ].
            
    equal &action:aBoolExpr 
        = $self equal &bool::(aBoolExpr eval).
            
    not = false.

    literal = "true".
    
    int = -1.

    bool = $self.

    <= &bool:$self.
}.

// --- false ---

#symbol false =
{
    then : anAction = nil.
    
    then : aTrueAction &else:aFalseAction = aFalseAction eval.

    else : aFalseAction = aFalseAction eval.

    or : anExpr => &or::anExpr.
            
    and : anExpr = $self.

    and &action:aBoolExpr = $self.
    
    xor : anExpr => &xor::anExpr.

    or &bool:aBool = aBool.

    or &action:aBool = aBool eval bool.
        
    or &int:anInt 
        = 0 != anInt.
                
    xor &bool:aBool = aBool ? [ true ] ! [ false ].
    
    xor &action:anExpr = anExpr eval ? [ true ] ! [ false ].

    equal : aBool => &equal::aBool.
    
    equal &bool:aBool = aBool ? [ false ] ! [ true ].
            
    equal &action:aBoolExpr 
        = $self equal &bool::(aBoolExpr eval).
            
    not = true.

    literal = "false".

    int = 0.

    bool = $self.

    <= &bool:$self.    
}.

// --- BreakException ---

#class BreakException
{
    #field theResult.
    
    #method stop = theResult.
}

// --- action role class ---

#class(role) actionOp
{
    #method run &if:aBool
    [
        #loop (true equal::aBool) ?
        [
            self eval.
        ].
    ]

    #method run &until:aBool
    [
        self eval.
        
        #loop (true equal::aBool) !
        [
            self eval.
        ].
    ]    
}

// --- function role class ---

#class(role) functionOp
{    
    #method run &for:anEnumerator
    [
        #loop (anEnumerator next) ?
        [
            self eval:(anEnumerator get).
        ].        
        
        ^ nil.        
    ]
    
    #method run &for:anEnumerator &where:aConditionFunction
    [
        #loop (anEnumerator next) ?
        [
            #var anItem := anEnumerator get.
            
            aConditionFunction eval:anItem
                ? [ self eval:anItem. ].
        ].        
        
        ^ nil.        
    ]
    
    #method run &for:anIterator &from:aStart &to:anEndValue &by:aStep
    [
        anIterator << aStart.
        
        #loop (anIterator <= anEndValue) ?
        [
            self eval:anIterator.
            
            anIterator += aStep.
        ].
        
        ^ nil.        
    ]
    
    #method run &for:anIterator &from:aStart &till:anEndValue &by:aStep
    [
        anIterator << aStart.
        
        #loop (anIterator < anEndValue) ?
        [
            self eval:anIterator.
            
            anIterator += aStep.
        ].
        
        ^ nil.        
    ]
    
    #method reverse &for:anIterator &from:aStart &to:anEndValue &by:aStep
    [
        anIterator << anEndValue.
        
        #loop (anIterator >= aStart) ?
        [
            self eval:anIterator.
            
            anIterator -= aStep.
        ].
        
        ^ nil.
    ]    
    
    #method reverse &for:anIterator &from:aStart &till:anEndValue &by:aStep
    [
        anIterator << anEndValue.
        
        #loop (anIterator > aStart) ?
        [
            anIterator -= aStep.
            
            self eval:anIterator.
        ].
        
        ^ nil.
    ]    
}

// --- control ---

#symbol control =
{
    do:anAction = anAction eval | stop.
    
    do:anAction &until:anExpr
        = anAction~actionOp run &until:anExpr | stop.
        
    while:anExpr
        = $self while:anExpr &do:[].

    while:anExpr &do:anAction
        = anAction~actionOp run &if:anExpr | stop.

    foreach:anEnumerable &do:aFunction
        = aFunction~functionOp run &for:(anEnumerable enumerator) | stop.

    foreach:anEnumerable &until:aFunction
    [
        #var anEnumerator := anEnumerable enumerator.
        
        ^ control while:[anEnumerator next] &do:
            [
                aFunction eval:(anEnumerator get) ?
                    [ #break anEnumerator get. ].
            ].
    ]
    
    for:anIterator &from:aStart &to:anEndValue &by:aStep &do:aFunction
        = aFunction~functionOp run &for:anIterator &from:aStart &to:anEndValue &by:aStep | stop.

    from:aStart &to:anEndValue &do:aFunction
        = aFunction~functionOp run &for:(system'Integer new) &from:aStart &to:anEndValue &by:1 | stop.
            
    from:aStart &till:anEndValue &do:aFunction
        = aFunction~functionOp run &for:(system'Integer new) &from:aStart &till:anEndValue &by:1 | stop.
            
    for:anIterator &from:aStart &backTo:anEndValue &by:aStep &do:aFunction
        = aFunction~functionOp reverse &for:anIterator &from:anEndValue &to:aStart &by:aStep | stop.
            
    from:aStart &backTo:anEndValue &do:aFunction
        = aFunction~functionOp reverse &for:(system'Integer new) &from:anEndValue &to:aStart &by:1 | stop.
}.

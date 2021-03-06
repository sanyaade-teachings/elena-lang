#define system.
#define system'routines.

// --- RandomGenerator ---

#class RandomGenerator
{
    #field(type:long)theData.
    
    #constructor new
        <rnd_init>.

    #method eval &int:aMaxValue &out'int:aRetVal
        <rnd_next>.

    #method eval : aMaxValue
    [
        #var(type:int)aNextValue := $self eval &int::aMaxValue.
        
        ^ IntNumber new &int:aNextValue.
    ]
}

// --- randomGenerator ---

#static randomGenerator = RandomGenerator new.

// -- ERandomizer --

#class(role) ERandomizer
{
    #method eval : aValue
    [
        #var i := aValue.
        #var j := randomGenerator:aValue.
        
        arrayControl exchange:i &with:j &in:self.
    ]
}

// --- randomize ---

#symbol randomControl =
{
    new : aMaxValue
        = randomGenerator eval:aMaxValue.
    
    randomize:aCount &array:anArray
    [
        DownCounter new:Wrap(ERandomizer, anArray) for:aCount.
        
        ^ anArray.
    ]
}.
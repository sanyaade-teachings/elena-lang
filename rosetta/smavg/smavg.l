#define system.
#define system'routines.
#define system'collections.
#define extensions.

#class SMAExtender
{
    #field thePeriod.
    
    #constructor new : aPeriod
    [
        thePeriod := aPeriod.
    ]
    
    #method add : aNumber
    [
        self += aNumber.

        #var aCount := self length.
        ^ aCount =>
            0 ?  [ 0.0r ] 
            ! [
                (aCount > thePeriod)?
                [
                    self remove &index:0.
                ].
        
                #var aSum := Summing new:(Real new:0) foreach:self.
                ^ aSum / self length.
            ].
    ]
}

#symbol sma = (:aPeriod) [ Extension(SMAExtender new:aPeriod, List new) ].

// --- Program ---

#symbol program =
[
    #var SMA3 := sma:3.
    #var SMA5 := sma:5.

    control from:1 &to:5 &do: i
    [
        consoleEx writeLine:"sma3 + " :i :" = ": (SMA3 + i number).
        consoleEx writeLine:"sma5 + " :i :" = ": (SMA5 + i number).
    ].

    control from:5 &backTo:1 &do: i
    [
        consoleEx writeLine:"sma3 + " :i :" = ": (SMA3 + i number).
        consoleEx writeLine:"sma5 + " :i :" = ": (SMA5 + i number).
    ].
].

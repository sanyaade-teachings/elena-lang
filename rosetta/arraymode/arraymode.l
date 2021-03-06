#define system.
#define system'routines.
#define system'collections.
#define extensions.

// Averages/Mode

#symbol mode = (:anArray)
[
    #var aCountMap := Dictionary new &default:0.
    control foreach:anArray &do: anItem
    [
        aCountMap set &key:anItem &value:(aCountMap getAt &key:anItem + 1).
    ].

    listControl sort:aCountMap &with: (:p:n)
        [ p value > n value ].

    #var aResult := List new.

    #var aMax := aCountMap First value.
    control foreach:aCountMap &do: anItem
    [
        aMax == anItem value
            ? [ aResult += anItem key. ].
    ].

    ^ listControl toArray:aResult.
].

#symbol program =
[
    #var anArray1 := (1, 1, 2, 4, 4).
    #var anArray2 := (1, 3, 6, 6, 6, 6, 7, 7, 12, 12, 17).
//    #var anArray3 := (1, "blue", 2, 7.5r, 5, "green", "red", 5, 2, "blue", "white").
    
    #var aMode1 := mode:anArray1.
    #var aMode2 := mode:anArray2.
//    #var aMode3 := mode:anArray3.
    
    consoleEx writeLine:"mode of (":anArray1:") is (":aMode1:")".
    consoleEx writeLine:"mode of (":anArray2:") is (":aMode2:")".
//    consoleEx write:"mode of (" write::anArray3 write:") is (" write::aMode3 writeLine:")".
].

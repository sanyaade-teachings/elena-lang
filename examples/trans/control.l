#define system.
#define extensions.

#symbol program =
[
    control while:true &do:
    [
        #var aSource := consoleEx writeLine:"Enter the cyrillic text for transliteration or prese enter to exit:" readLine:(String new).

        (0 == aSource length)
            ? [ #break BreakException new. ].

        consoleEx writeLine:(Transliteration:aSource).
    ].
].

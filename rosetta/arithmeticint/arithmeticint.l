#define system.
#define extensions.

// --- Program ---

#symbol program =
[
    #var a := consoleEx readLine:(Integer new).
    #var b := consoleEx readLine:(Integer new).
    
    consoleEx << a << " + " << b << " = " << a + b << "%n".
    consoleEx << a << " - " << b << " = " << a - b << "%n".
    consoleEx << a << " * " << b << " = " << a * b << "%n".
    consoleEx << a << " / " << b << " = " << a / b << "%n". // truncates towards 0
    consoleEx << a << " %% " << b << " = " << (a~mathOp mod:b) << "%n". // matches sign of first operand
].
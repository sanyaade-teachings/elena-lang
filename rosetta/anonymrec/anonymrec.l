#define system.

#symbol fibo = (:n)
[
    n < 0
        ? [ #throw InvalidArgumentException new:"Must be non negative". ].
 
//    ^ { eval &int:n [ ^ (n > 1) ? [ $self eval &int:(n - 2) + $self eval &int:(n - 1) ] ! [ n ]. ] } eval &int::n.
    
    ^ { eval:n [ ^ (n > 1) ? [ ($self:(n - 2)) + ($self:(n - 1)) ] ! [ n ]. ] }:n.
].

#symbol program =
[
    control from:-1 &to:10 &do: i
    [
        console << "fib(" << i << ")=".
        
        console writeLine:(fibo:i) | onInvalidArgumentError: e
            [
                console writeLine:"invalid".
            ].
    ].
    
    console readChar.
].

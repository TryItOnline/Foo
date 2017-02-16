# Foo

**The materials in this repository were not produced by the owner of the repository. Please click on the link below to go to the original web site these materials were copied from. The purpose of this copy is to automate setup of a mirror for https://tryitonline.net. It's more straight forward to clone a repository than to parse a web page.**

Implementation of [this language](https://esolangs.org/wiki/Foo) from [here](http://web.archive.org/web/20160505151731/http://feky.bizhat.com/apps/foo.tar.gz).

Foo is an esoteric programming language, created by Feky in 2008.
It is inspired mainly by Brainfuck, and is Turing-complete (since Brainfuck is).

## Memory

Foo programs are supplied with a one-dimensional array of X elements (cells, whatever),
where each element can hold a value from 0 to 65535.
The array pointer wraps around when it exceeds the number X, and points to
the first element in the array.
All elements in the array are initialized to zero on the start of the program.
There's also a stack to help with arithmetic operations and various copying operations,
which can also have a custom number (X) of elements, and the value of each can
vary from 0 to 65535. Conditional jumps (loops) are also controlled
using a stack, that is limited to the number of maximum nested loops.

## Operators

Foo is capable of doing basic and more complex arithmetic operations,
measuring the time (in seconds, and larger units), controlling the flow
of execution and outputting values.
Foo programs end when the interpreter reaches EOF.

- `"` - Everything between these will be printed to stdout (or somewhere else).
- `&` - Set the value of the currently selected cell in the array to some other value. If followed numbers, like `&55`, the cell will be set to 55. If not, it will be set to a popped value from the stack.
- `@` - Similar to the `&` operator, but this one pushes values on the stack.
- `<` - Decrement the array pointer by one
- `>` - Increment the array pointer by one
- `$` - Print out a single value. There are 3 modes - print as an integer (i), hexadecimal integer (h) and ASCII character (c). If the mode isn't specified, a standard Foo interpreter will show and error and continue execution. For example, `$h` will print the current cell in hex, and `$c100` will print 'd'.
- `+`, `-`, `*`, `/`, `%` - Arithmetic operators. They are always applied to the currently pointed element in the array. For example, `+10` will add 10 to the current element, and `/` will divide the element with a value popped from the stack.
- `#` - Pause the execution of the program (sleep) for specified seconds. If no number is given after `#`, it get the value of the current cell and sleep that much.
- `(` - Beginning of a loop. If followed by a number, the loop will be execute as long as the value of the current cell is different than the number specified. If there's no number after `(`, the loop will execute while the cell is not equal to zero.
- `)` - Conditional jump (end of a loop). Jumps back to the last `(` if the current cell isn't equal to the value on the top of the loop stack, or continues the program execution normally.

## Examples

Hello, World!: `"Hello, World!"`

Double a number: `&256*2$i`

Countdown loop: `&10(0#1-1$i$c10)"boom!"$c10`

Infinite loop: `(1)`

Negative numbers: `&30@50-@&65535-+1$i`

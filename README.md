# Brainlike - Extra Brainfuck

## The Repository

Brainlike is a superset of brainfuck.You can use extra grammars to make your code shorter(maybe?).

## What does it supports

### Basic brainfuck grammars

| Symbols | Meaning |
| --- | --- |
| >   | Move the pointer to the right |
| <   | Move the pointer to the left |
| .   | Output the character signified by the cell at the pointer |
| ,   | Input a character and store it in the cell at the pointer |
| +   | Increment the memory cell at the pointer |
| -   | Decrement the memory cell at the pointer |
| [   | Jump past the matching ] if the cell at the pointer is 0 |
| ]   | Jump back to the matching [ if the cell at the pointer is nonzero |

### Extra grammars

| Symbols | Meaning |
| --- | --- |
| :   | Output the memory cell at the pointer directly |
| ;   | Input a number and store it in the memory cell at the pointer |
| ^   | Make **the program pointer** equal the memory cell at the pointer.If the program pointer bigger than the program size,stop the program |
| !   | Make the program's exit code equal the memory cell at the pointer |
| ~   | Negate the memory cell at the pointer.If the memory cell at the pointer bigger than one,make it equal one |
| '   | Free the last memory cell.If the pointer points to it,move the pointer to the last memory cell for now |
| &   | Make the memory cell at the pointer equal the pointer |
| $   | Make the pointer equal the memory cell at the pointer.If the pointer bigger than the size of the memory,allocate new until the size equal it |
| *   | Square the memory cell at the pointer |
| "   | Log the memory cell at the pointer to a file named `blpg.log`(fixed name,app write in)(as a character) |
| \   | Push the memory cell at the pointer to the stack and make the memory cell at the pointer equal 0 |
| /   | Get the top of the stack and free it,then make the memory cell at the pointer equal the value |
| _   | Set the memory cell at the pointer to a random 32-bit unsigned number(max:255) |

###### Latest updated:Interpreter version 1150

### Differences in details

The memory's size is **NOT** fixed.The interpreter should automaticly allocate for the running program.

## Extensions

Extensions can extend the Brainlike Interpreter, enabling it to interpret additional instructions.

### Extension Usage

Refer to the -h option in the command line for details.

### Extension Development

You need to develop a Dynamic Link Library (DLL) (Windows-only for the time being) that contains the function 
`int analyzeCommand(size_t&,std::string&,size_t&,unsigned&,std::string&,std::vector<unsigned>&,stack_agc<unsigned>&,std::uniform_int_distribution<unsigned>&)`.

(You will need `stack_agc.hpp`.)

During runtime, the program will provide the variables you may need in the order of `i/*program ptr*/,program,ptr,exitCode,logOutputBuffer,memory,stack_mem,dist`.

When the parsing function returns `1`, the output will not be skipped in debug mode; otherwise, the output will be skipped.

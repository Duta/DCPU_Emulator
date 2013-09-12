# DCPU Emulator

## Language summary

 - **Emulator** written in C++
 - **Assembler** written in Python

## Usage

### Assembler

    python assembler[27|33].py emulator [assembly]

emulator:   The name of the emulator's
            executable (probably `DCPU`)
assembly:   The name of the assembly file
            to convert code from. If this
            argument is not given, the
            code can be inputted directly
            via a simple prompt.

#### Note

The `assembler27.py` and `assembler33.py` files differ
only in the version of Python that they run under -
2.7 and 3.3 respectively. Which you use depends purely
on what you have installed on your system.

#### Example

    python assembler33.py DCPU test.s

### Emulator

    DCPU instructions

instructions:   The list of (zero or more)
                machine code instructions
                to execute.

#### Note

You should rarely need to call this yourself - 
instead, the assembler will call it for you.

#### Example

    DCPU 37889 37922 37922 38947 44801 49921 54369 24929

Which in assembly is:

    SET A 4
    ADD B 4
    ADD B 4
    SUB B 5
    SET PUSH 10
    SET PUSH 15
    SET X 20
    SET [X] POP



**More to write**

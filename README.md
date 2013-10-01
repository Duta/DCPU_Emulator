# DCPU Emulator

## Language Summary

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

#### Notes

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

#### Notes

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

## Assembler

The first thing to note about the assembler is
that internally everything is initially de-cased
prior to parsing, which means the entire file is
case insensitive. The advantage of this is that
you can use whatever case conventions seem best,
however one thing to be wary of is that you
should **not** have multiple labels which
differ only in case - e.g. `start` & `START`.

Whitespace is also optional - as long as there
is at least one space or tab between the command
and each argument for instructions, you can format
your commands with as many spaces and tabs as you
want. The only thing to mention is that in things
such as `[A+NW]`, you mustn't insert whitespace
within, e.g. `[ A + NW ]`.

Instructions should not span multiple lines, and
each line should have at most one instruction xor
one label on them - you can have blank lines, but
not lines with multiple things on them.

For now there are no comments, however everything
after an instruction or label is ignored, so you
can use that for a makeshift commenting system.

### Opcode List

    +---------+--------------------------------------------------------+
    | Command | Action                                                 |
    +=========+========================================================+
    | SET b a | Sets b to a                                            |
    +---------+--------------------------------------------------------+
    | ADD b a | Adds a to b and stores the result in b.                |
    |         | Sets EX to 0x0001 if there is overflow, 0x0 otherwise  |
    +---------+--------------------------------------------------------+
    | SUB b a | Subtracts a from b and stores the result in b.         |
    |         | Sets EX to 0xFFFF if there is underflow, 0x0 otherwise |
    +---------+--------------------------------------------------------+
    | MUL b a | Multiplies a by b and stores the result in b.          |
    |         | Sets EX to (a * b) >> 16                               |
    +---------+--------------------------------------------------------+
    | MLI b a | The same as MUL, but treats a and b as signed.         |
    +---------+--------------------------------------------------------+
    | DIV b a | Divides b by a and stores the result in b.             |
    |         | Sets EX to ((b << 16) / a) & 0xFFFF.                   |
    |         | If a is 0, sets b and EX to 0 instead                  |
    +---------+--------------------------------------------------------+
    | DVI b a | <TODO>                                                 |
    +---------+--------------------------------------------------------+
    | MOD b a | Sets b to b mod a if a is non-zero, 0 otherwise        |
    +---------+--------------------------------------------------------+
    | MDI b a | The same as MOD, but treats b as signed. -16 % 7 = -16 |
    +---------+--------------------------------------------------------+
    | AND b a | Bitwise-and's a and b and stores the result in b       |
    +---------+--------------------------------------------------------+
    | BOR b a | Bitwise-or's a and b and stores the result in b        |
    +---------+--------------------------------------------------------+
    | XOR b a | Bitwise-xor's a and b and stores the result in b       |
    +---------+--------------------------------------------------------+
    | SHR b a | Logical-right-shift's b by a bits.                     |
    |         | Sets EX to (b << 16) >> a                              |
    +---------+--------------------------------------------------------+
    | ASR b a | Arithmetic-right-shift's b by a bits.                  |
    |         | Sets EX to (b << 16) >> a                              |
    +---------+--------------------------------------------------------+
    | SHL b a | Left-shift's b by a bits.                              |
    |         | Sets EX to (b << a) >> 16                              |
    +---------+--------------------------------------------------------+
    | IFB b a | Executes the next instruction iff (a & b) != 0         |
    +---------+--------------------------------------------------------+
    | IFC b a | Executes the next instruction iff (a & b) == 0         |
    +---------+--------------------------------------------------------+
    | IFE b a | Executes the next instruction iff a == b               |
    +---------+--------------------------------------------------------+
    | IFN b a | Executes the next instruction iff a != b               |
    +---------+--------------------------------------------------------+
    | IFG b a | Executes the next instruction iff b > a                |
    +---------+--------------------------------------------------------+
    | IFA b a | Same as IFG but treating the values as signed          |
    +---------+--------------------------------------------------------+
    | IFL b a | Executes the next instruction iff b < a                |
    +---------+--------------------------------------------------------+
    | IFU b a | Same as IFL but treating the values as signed          |
    +---------+--------------------------------------------------------+
    | ADX b a | Stores the sum of a, b and EX in b.                    |
    |         | Sets EX to 0x0001 if there is overflow, 0x0 otherwise  |
    +---------+--------------------------------------------------------+
    | SBX b a | Stores the sum of -a, b and EX in b.                   |
    |         | Sets EX to 0xFFFF if there is underflow, 0x0 otherwise |
    +---------+--------------------------------------------------------+
    | STI b a | Sets b to a and increments I and J                     |
    +---------+--------------------------------------------------------+
    | STD b a | Sets b to a and decrements I and J                     |
    +---------+--------------------------------------------------------+
    | JSR a   | Pushes PC onto the stack and sets PC to a              |
    +---------+--------------------------------------------------------+
    | INT a   | Triggers a software interrupt with message a           |
    +---------+--------------------------------------------------------+
    | IAG a   | Sets a to IA                                           |
    +---------+--------------------------------------------------------+
    | IAS a   | Sets IA to a                                           |
    +---------+--------------------------------------------------------+
    | RFI a   | Disables interrupt queueing, pops A from the stack and |
    |         | pops PC from the stack                                 |
    +---------+--------------------------------------------------------+
    | IAQ a   | Interrupts will be added to the queue instead of       |
    |         | triggered if a is non-zero, otherwise interrupts will  |
    |         | be triggered as normal again                           |
    +---------+--------------------------------------------------------+
    | HWN a   | Sets a to the number of connected hardware devices     |
    +---------+--------------------------------------------------------+
    | HWQ a   | Sets registers A, B, C, X, Y to info about hardware a. |
    |         | A+(B<<16) is a 32bit word identifying the hardware id. |
    |         | C is the hardware version.                             |
    |         | X+(Y<<16) is a 32bit word identifying the manufacturer |
    +---------+--------------------------------------------------------+
    | HWI a   | Sends an interrupt to hardware a                       |
    +---------+--------------------------------------------------------+

### Values

Note that values enclosed in square brackets, such as:

    [a]

means the value at index `a` in RAM.

    +--------+--------------------------------------------------------+
    | Value  | Meaning                                                |
    +========+========================================================+
    | A      | Register A                                             |
    +--------+--------------------------------------------------------+
    | B      | Register B                                             |
    +--------+--------------------------------------------------------+
    | C      | Register C                                             |
    +--------+--------------------------------------------------------+
    | X      | Register X                                             |
    +--------+--------------------------------------------------------+
    | Y      | Register Y                                             |
    +--------+--------------------------------------------------------+
    | Z      | Register Z                                             |
    +--------+--------------------------------------------------------+
    | I      | Register I                                             |
    +--------+--------------------------------------------------------+
    | J      | Register J                                             |
    +--------+--------------------------------------------------------+
    | [A]    | RAM[A]                                                 |
    +--------+--------------------------------------------------------+
    | [B]    | RAM[B]                                                 |
    +--------+--------------------------------------------------------+
    | [C]    | RAM[C]                                                 |
    +--------+--------------------------------------------------------+
    | [X]    | RAM[X]                                                 |
    +--------+--------------------------------------------------------+
    | [Y]    | RAM[Y]                                                 |
    +--------+--------------------------------------------------------+
    | [Z]    | RAM[Z]                                                 |
    +--------+--------------------------------------------------------+
    | [I]    | RAM[I]                                                 |
    +--------+--------------------------------------------------------+
    | [J]    | RAM[J]                                                 |
    +--------+--------------------------------------------------------+
    | [A+NW] | RAM[A + NEXT_WORD]             (NEXT_WORD = RAM[PC++]) |
    +--------+--------------------------------------------------------+
    | [B+NW] | RAM[B + NEXT_WORD]             (NEXT_WORD = RAM[PC++]) |
    +--------+--------------------------------------------------------+
    | [C+NW] | RAM[C + NEXT_WORD]             (NEXT_WORD = RAM[PC++]) |
    +--------+--------------------------------------------------------+
    | [X+NW] | RAM[X + NEXT_WORD]             (NEXT_WORD = RAM[PC++]) |
    +--------+--------------------------------------------------------+
    | [Y+NW] | RAM[Y + NEXT_WORD]             (NEXT_WORD = RAM[PC++]) |
    +--------+--------------------------------------------------------+
    | [Z+NW] | RAM[Z + NEXT_WORD]             (NEXT_WORD = RAM[PC++]) |
    +--------+--------------------------------------------------------+
    | [I+NW] | RAM[I + NEXT_WORD]             (NEXT_WORD = RAM[PC++]) |
    +--------+--------------------------------------------------------+
    | [J+NW] | RAM[J + NEXT_WORD]             (NEXT_WORD = RAM[PC++]) |
    +--------+--------------------------------------------------------+
    | PUSH   | RAM[--SP]                                              |
    +--------+--------------------------------------------------------+
    | POP    | RAM[SP++]                                              |
    +--------+--------------------------------------------------------+
    | PEEK   | RAM[SP]                                                |
    +--------+--------------------------------------------------------+
    | PICK   | RAM[SP + NEXT_WORD]            (NEXT_WORD = RAM[PC++]) |
    +--------+--------------------------------------------------------+
    | SP     | Stack Pointer                                          |
    +--------+--------------------------------------------------------+
    | PC     | Program Counter                                        |
    +--------+--------------------------------------------------------+
    | EX     | Extra/Excess                                           |
    +--------+--------------------------------------------------------+
    | [NW]   | RAM[NEXT_WORD]                 (NEXT_WORD = RAM[PC++]) |
    +--------+--------------------------------------------------------+
    | NW     | NEXT_WORD                      (NEXT_WORD = RAM[PC++]) |
    +--------+--------------------------------------------------------+

Literal values between -1 and 30 inclusive are also allowed.

#### Notes

By using PUSH and POP, there is a reverse stack starting at
the end of memory (i.e. memory location 65535) and extending
leftwards into memory. Because of this, be careful not to
accidentally overwrite values on the stack.
To avoid this, if writing values close to the end of memory
(or if just managing an abnormally large stack), you can
check the stack pointer, which holds the index of the head
(leftmost value) of the stack, or 0 when the stack is empty.
For example code to achieve this, see the examples section.

The difference between PUSH and POP is not which of them
you write, but rather where they are placed. For example:

    // Both of these push 5 onto the stack:
    SET PUSH 5
    SET POP  5

    // Both of these pop a value and store it in A:
    SET A POP
    SET A PUSH

    // Both of these pop a value and store it in IA:
    IAS POP
    IAS PUSH

The rule is:

 - For regular instructions, if it is the first value
   then it always means PUSH, and if it is the second
   then it always means POP.
 - For special instructions (which all only take one
   value) it always means POP.

The assembler will issue a warning if you use them
incorrectly, however it will still assemble the code.

In regular instructions of the form:

    INS b a

The DCPU evaluates a before b, and both values are
evaluated before execution of the instruction.
This is important to know for cases like this:

    SET [NW] NW
    1234
    5678

Because of the order of evaluation, this actually means:

    SET [5678] 1234
    // i.e. RAM[5678] = 1234;

### Examples

A basic loop structure:

    // Equivalent to for(A = 0; A < 20; A += 9) { B++; }

    // In "full" form:

    loop_pre:
        SET A 0
        SET PC loop_condition
    loop_condition:
        IFL A 20
          SET PC loop_body
        SET PC loop_end
    loop_body:
        ADD B 1
        SET PC loop_inc
    loop_inc:
        ADD A 9
        SET PC loop_condition
    loop_end:

    // It can, however, simply be written as:

        SET A 0
    loop_condition:
        IFL A 20
          SET PC loop_body
        SET PC loop_end
    loop_body:
        ADD B 1
        ADD A 9
        SET PC loop_condition
    loop_end:

Avoiding corruption of the stack:

    // The following code only sets RAM[65500]
    // to 17 if doing so will not overwrite a
    // value on the stack.

        IFE SP 0   // The stack is empty, no worries
          SET PC some_dangerous_code
        IFL NW SP  // Not part of the stack, no worries
        65500
          SET PC some_dangerous_code
        SET PC end
    some_dangerous_code:
        SET [NW] 17
        65500
    end:

**More to write**

## Emulator

Each instruction is 16 bits (the word size of the
DCPU - hence it's name DCPU-16).
The bits for a regular instruction are as follows:

    aaaaaabbbbbooooo

i.e. the first 6 bits are a, the next 5 b and the
final 5 o, where:

 - a and b are values that are the arguments to
   the instructions.
 - o is the opcode of the instruction.

When the last 5 (opcode) bits are unset (i.e.
all zero), it is a special instruction.
Special instructions have the form:

    aaaaaaooooo00000

In this case the opcode selects a command from
the special instruction set rather than the
regular one.

Note that regular instructions take two values
(arguments), whereas specials only take one.

**More to write**

### Opcode List

**More to write**

    +---------+------+--------------+
    | Command | Code | Implemented? |
    +=========+======+==============+
    | SPECIAL | 0x00 |     DONE     |
    +---------+------+--------------+
    | SET b a | 0x01 |     DONE     |
    +---------+------+--------------+
    | ADD b a | 0x02 |     DONE     |
    +---------+------+--------------+
    | SUB b a | 0x03 |     DONE     |
    +---------+------+--------------+
    | MUL b a | 0x04 |     DONE     |
    +---------+------+--------------+
    | MLI b a | 0x05 |     DONE     |
    +---------+------+--------------+
    | DIV b a | 0x06 |     DONE     |
    +---------+------+--------------+
    | DVI b a | 0x07 |   ? DONE ?   |
    +---------+------+--------------+
    | MOD b a | 0x08 |     DONE     |
    +---------+------+--------------+
    | MDI b a | 0x09 |     DONE     |
    +---------+------+--------------+
    | AND b a | 0x0A |     DONE     |
    +---------+------+--------------+
    | BOR b a | 0x0B |     DONE     |
    +---------+------+--------------+
    | XOR b a | 0x0C |     DONE     |
    +---------+------+--------------+
    | SHR b a | 0x0D |     DONE     |
    +---------+------+--------------+
    | ASR b a | 0x0E |     DONE     |
    +---------+------+--------------+
    | SHL b a | 0x0F |     DONE     |
    +---------+------+--------------+
    | IFB b a | 0x10 |     DONE     |
    +---------+------+--------------+
    | IFC b a | 0x11 |     DONE     |
    +---------+------+--------------+
    | IFE b a | 0x12 |     DONE     |
    +---------+------+--------------+
    | IFN b a | 0x13 |     DONE     |
    +---------+------+--------------+
    | IFG b a | 0x14 |     DONE     |
    +---------+------+--------------+
    | IFA b a | 0x15 |     DONE     |
    +---------+------+--------------+
    | IFL b a | 0x16 |     DONE     |
    +---------+------+--------------+
    | IFU b a | 0x17 |     DONE     |
    +---------+------+--------------+
    | ADX b a | 0x1A |     DONE     |
    +---------+------+--------------+
    | SBX b a | 0x1B |     DONE     |
    +---------+------+--------------+
    | STI b a | 0x1E |     DONE     |
    +---------+------+--------------+
    | STD b a | 0x1F |     DONE     |
    +---------+------+--------------+
    |     Special instructions:     |
    +---------+------+--------------+
    | JSR a   | 0x01 |     DONE     |
    +---------+------+--------------+
    | INT a   | 0x08 |      NO      |
    +---------+------+--------------+
    | IAG a   | 0x09 |     DONE     |
    +---------+------+--------------+
    | IAS a   | 0x0A |     DONE     |
    +---------+------+--------------+
    | RFI a   | 0x0B |      NO      |
    +---------+------+--------------+
    | IAQ a   | 0x0C |      NO      |
    +---------+------+--------------+
    | HWN a   | 0x10 |     DONE     |
    +---------+------+--------------+
    | HWQ a   | 0x11 |     DONE     |
    +---------+------+--------------+
    | HWI a   | 0x12 |     DONE     |
    +---------+------+--------------+

**More to write**

### Values

    +-------+--------------------------------------------------------+
    | Value | Meaning                                                |
    +=======+========================================================+
    | 0x00  | Register A                                             |
    +-------+--------------------------------------------------------+
    | 0x01  | Register B                                             |
    +-------+--------------------------------------------------------+
    | 0x02  | Register C                                             |
    +-------+--------------------------------------------------------+
    | 0x03  | Register X                                             |
    +-------+--------------------------------------------------------+
    | 0x04  | Register Y                                             |
    +-------+--------------------------------------------------------+
    | 0x05  | Register Z                                             |
    +-------+--------------------------------------------------------+
    | 0x06  | Register I                                             |
    +-------+--------------------------------------------------------+
    | 0x07  | Register J                                             |
    +-------+--------------------------------------------------------+
    | 0x08  | RAM[A]                                                 |
    +-------+--------------------------------------------------------+
    | 0x09  | RAM[B]                                                 |
    +-------+--------------------------------------------------------+
    | 0x0A  | RAM[C]                                                 |
    +-------+--------------------------------------------------------+
    | 0x0B  | RAM[X]                                                 |
    +-------+--------------------------------------------------------+
    | 0x0C  | RAM[Y]                                                 |
    +-------+--------------------------------------------------------+
    | 0x0D  | RAM[Z]                                                 |
    +-------+--------------------------------------------------------+
    | 0x0E  | RAM[I]                                                 |
    +-------+--------------------------------------------------------+
    | 0x0F  | RAM[J]                                                 |
    +-------+--------------------------------------------------------+
    | 0x10  | RAM[A + NEXT_WORD]             (NEXT_WORD = RAM[PC++]) |
    +-------+--------------------------------------------------------+
    | 0x11  | RAM[B + NEXT_WORD]             (NEXT_WORD = RAM[PC++]) |
    +-------+--------------------------------------------------------+
    | 0x12  | RAM[C + NEXT_WORD]             (NEXT_WORD = RAM[PC++]) |
    +-------+--------------------------------------------------------+
    | 0x13  | RAM[X + NEXT_WORD]             (NEXT_WORD = RAM[PC++]) |
    +-------+--------------------------------------------------------+
    | 0x14  | RAM[Y + NEXT_WORD]             (NEXT_WORD = RAM[PC++]) |
    +-------+--------------------------------------------------------+
    | 0x15  | RAM[Z + NEXT_WORD]             (NEXT_WORD = RAM[PC++]) |
    +-------+--------------------------------------------------------+
    | 0x16  | RAM[I + NEXT_WORD]             (NEXT_WORD = RAM[PC++]) |
    +-------+--------------------------------------------------------+
    | 0x17  | RAM[J + NEXT_WORD]             (NEXT_WORD = RAM[PC++]) |
    +-------+--------------------------------------------------------+
    | 0x18  | POP if in a                          (POP = RAM[SP++]) |
    |       | PUSH if in b                        (PUSH = RAM[--SP]) |
    +-------+--------------------------------------------------------+
    | 0x19  | PEEK                                  (PEEK = RAM[SP]) |
    +-------+--------------------------------------------------------+
    | 0x1A  | PICK                      (PICK = RAM[SP + NEXT_WORD]) |
    |       |                                (NEXT_WORD = RAM[PC++]) |
    +-------+--------------------------------------------------------+
    | 0x1B  | Stack Pointer                                          |
    +-------+--------------------------------------------------------+
    | 0x1C  | Program Counter                                        |
    +-------+--------------------------------------------------------+
    | 0x1D  | Extra/Excess                                           |
    +-------+--------------------------------------------------------+
    | 0x1E  | RAM[NEXT_WORD]                 (NEXT_WORD = RAM[PC++]) |
    +-------+--------------------------------------------------------+
    | 0x1F  | NEXT_WORD                      (NEXT_WORD = RAM[PC++]) |
    +-------+--------------------------------------------------------+

#### Notes

While the above table fully describes the values the 5 bits of b may take
on, a is 6 bits and so may also take on the values `0x20` to `0x3F` inclusive.
This extra range of a is used for literal values in the range `0xFFFF-0x1E` -
i.e. -1 to 30 inclusive.

With regards to `0x18`, where it may be PUSH or POP depending on whether
it is the first or second argument to a regular instruction, with special
instructions it is *always* POP, since a is the only value used by them.

**More to write**

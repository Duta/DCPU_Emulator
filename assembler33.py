"""
USAGE:
    python assembler.py emulator [assembly]

        emulator: The name of the emulator's
                  executable file.
        assembly: The name of the assembly file
                  to convert code from. If this
                  argument is not given, the
                  code can be inputted directly
                  via a simple prompt.
"""

import sys, os

# Opcodes
opcodes = {
    'SET': 0x01,
    'ADD': 0x02,
    'SUB': 0x03,
    'MUL': 0x04,
    'MLI': 0x05,
    'DIV': 0x06,
    'DVI': 0x07,
    'MOD': 0x08,
    'MDI': 0x09,
    'AND': 0x0A,
    'BOR': 0x0B,
    'XOR': 0x0C,
    'SHR': 0x0D,
    'ASR': 0x0E,
    'SHL': 0x0F,
    'IFB': 0x10,
    'IFC': 0x11,
    'IFE': 0x12,
    'IFN': 0x13,
    'IFG': 0x14,
    'IFA': 0x15,
    'IFL': 0x16,
    'IFU': 0x17,
    'ADX': 0x1A,
    'SBX': 0x1B,
    'STI': 0x1E,
    'STD': 0x1F
}
special_opcodes = {
    'JSR': 0x01,
    'INT': 0x08,
    'IAG': 0x09,
    'IAS': 0x0A,
    'RFI': 0x0B,
    'IAQ': 0x0C,
    'HWN': 0x10,
    'HWQ': 0x11,
    'HWI': 0x12
}

# Special values
specials = {
    'A': 0x00,
    'B': 0x01,
    'C': 0x02,
    'X': 0x03,
    'Y': 0x04,
    'Z': 0x05,
    'I': 0x06,
    'J': 0x07,
    '[A]': 0x08,
    '[B]': 0x09,
    '[C]': 0x0A,
    '[X]': 0x0B,
    '[Y]': 0x0C,
    '[Z]': 0x0D,
    '[I]': 0x0E,
    '[J]': 0x0F,
    '[A+NW]': 0x10,
    '[B+NW]': 0x11,
    '[C+NW]': 0x12,
    '[X+NW]': 0x13,
    '[Y+NW]': 0x14,
    '[Z+NW]': 0x15,
    '[I+NW]': 0x16,
    '[J+NW]': 0x17,
    'PUSH': 0x18,
    'POP': 0x18,
    'PEEK': 0x19,
    'PICK': 0x1A,
    'SP': 0x1B,
    'PC': 0x1C,
    'EX': 0x1D,
    '[NW]': 0x1E,
    'NW': 0x1F
}

# Read the input
lines = []
if len(sys.argv) > 2:
    # Read from file
    with open(sys.argv[2]) as f:
        lines = f.readlines()
else:
    # Have the code entered via a prompt
    line = '.'
    while line:
        line = input('> ')
        lines.append(line)

# Format the input
lines = [line.strip().replace(r'\s', ' ').upper() for line in lines]

# Remove blank lines
lines = [line for line in lines if line]

# If no code was entered, exit immediately
if lines == []:
    print('No code entered, exiting without emulation')
    sys.exit()

# Parse the input
print('Parsing input...')
instructions = []
labels = {}

def get_value(item):
    if item in specials:
        return specials[item]
    if item in labels:
        return labels[item]
    return int(item) + 0x21

for i, line in enumerate(lines):
    parts = line.split(' ')
    if parts[0] in opcodes:
        opcode = opcodes[parts[0]]
        a = get_value(parts[2])
        b = get_value(parts[1])
        instructions.append((a << 10) + (b << 5) + opcode)
    elif parts[0] in special_opcodes:
        opcode = special_opcodes[parts[0]]
        a = get_value(parts[1])
        instructions.append((a << 10) + (opcode << 5))
    elif parts[0][-1] == ':':
        labels[parts[0][:-1]] = i

print(len(instructions), 'instructions parsed. Beginning emulation.')

args = (str(x) for x in instructions)
os.execl(sys.argv[1], *args)

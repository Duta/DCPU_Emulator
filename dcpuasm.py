import sys, os, array

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
special_values = {
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
if len(sys.argv) == 3:
    # Read from file
    with open(sys.argv[2]) as f:
        lines = f.readlines()
elif len(sys.argv) == 2:
    # Have the code entered via a prompt
    line = '.'
    while line:
        line = input('dcpuasm> ')
        lines.append(line)
else:
    # Incorrect number of arguments
    print('ERROR: Incorrect number of arguments.')
    print('USAGE:')
    print('    python dcpuasm.py out_file [in_file]')
    sys.exit(1)

# Format the input
lines = [' '.join(line.split()).upper() for line in lines]

# Remove blank lines
lines = [line for line in lines if line]

# Parse the input
instructions = []
labels = {}

def get_value(item):
    if item in special_values:
        return special_values[item]
    if item in labels:
        return labels[item]
    return int(item) + 0x21

def is_number(item):
    try:
        int(item)
        return True
    except:
        return False

# On the first pass, collect all the labels
for i, line in enumerate(lines):
    parts = line.split(' ')
    if parts[0][-1] == ':':
            label_value = i - len(labels)
            labels[parts[0][:-1]] = label_value

# Do a second pass correcting the label values
for line in lines:
    parts = line.split(' ')
    for i, part in enumerate(parts):
        if 0 < i < 3 and part in labels and labels[part] > 30:
            # We're going to have to insert a line
            # here on the third pass, so inc all
            # label values greater than this line
            for key in labels:
                if labels[key] > labels[part]:
                    pass
                    #labels[key] += 1

# Now on the third pass convert any
# references to the labels to the actual
# numbers
new_lines = lines[:]
num_lines_added = 0
for i, line in enumerate(lines):
    parts = line.split(' ')
    old_num_lines_added = num_lines_added
    parts.reverse()
    for j, part in enumerate(parts):
        if part in labels:
            label_val = labels[part]
            if label_val > 30:
                parts[j] = 'NW'
                num_lines_added += 1
                new_lines.insert(i + num_lines_added, str(label_val))
            else:
                parts[j] = str(label_val)
    parts.reverse()
    new_lines[i + old_num_lines_added] = ' '.join(parts)
lines = new_lines

# Now do a final pass actually converting
incorrect_push_pop_usage = False
pretty_lines = []
for line in lines:
    parts = line.split(' ')
    if parts[0] in opcodes:
        opcode = opcodes[parts[0]]
        a = get_value(parts[2])
        b = get_value(parts[1])
        instructions.append((a << 10) + (b << 5) + opcode)
        if parts[1] == 'POP' or parts[2] == 'PUSH':
            incorrect_push_pop_usage = True
        pretty_lines.append(' '.join(parts[:3]))
    elif parts[0] in special_opcodes:
        opcode = special_opcodes[parts[0]]
        a = get_value(parts[1])
        instructions.append((a << 10) + (opcode << 5))
        if parts[1] == 'PUSH':
            incorrect_push_pop_usage = True
        pretty_lines.append(' '.join(parts[:2]))
    elif is_number(parts[0]):
        instructions.append(int(parts[0]) & 0xFFFF)
        pretty_lines.append(parts[0])

for i, line in enumerate(pretty_lines):
    print(i, line)

if incorrect_push_pop_usage:
    print('WARNING: Incorrect usage of PUSH/POP at least once.')
    print('         Refer to the README section Assembler::Values::Notes')

data = array.array('B')
for instruction in instructions:
    upper = (instruction >> 8) & 0xFF
    lower = instruction & 0xFF
    data.append(upper)
    data.append(lower)

with open(sys.argv[1], 'wb') as f:
    data.tofile(f)

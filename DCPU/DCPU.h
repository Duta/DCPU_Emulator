#pragma once

#include <cstdint>
#include "Hardware.h"

#define DCPU_WORD uint16_t
#define DCPU_WORD_S int16_t
#define NEXT_WORD RAM[PC++]
#define PUSH RAM[--SP]
#define POP RAM[SP++]
#define PEEK RAM[SP]
#define PICK RAM[SP + NEXT_WORD]
#define RAM_SIZE 0x10000
#define MAX_CONNECTED_HARDWARE 0x10000

class Hardware;

class DCPU
{
public:
    DCPU();
    ~DCPU();

    void run(unsigned int);
    void reset();
    void connectHardware(Hardware *);
    void disconnectHardware(Hardware *);
    unsigned int getNumConnectedHardware() const;
    void printState() const;

    // RAM
    DCPU_WORD RAM[RAM_SIZE];
    // Registers
    DCPU_WORD A, B, C, X, Y, Z, I, J;
    // Program counter
    DCPU_WORD PC;
    // Stack pointer
    DCPU_WORD SP;
    // Extra/excess
    DCPU_WORD EX;
    // Interrupt address
    DCPU_WORD IA;
private:
    void executeInstruction(const DCPU_WORD);
    void executeSpecialInstruction(const DCPU_WORD, DCPU_WORD *);
    DCPU_WORD * getValue(const DCPU_WORD, const bool);
    void resetRAM();
    void clearHardwareConnections();

    bool shouldExecute;
    // Connected hardware
    Hardware * hardware[MAX_CONNECTED_HARDWARE];
};

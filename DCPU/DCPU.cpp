#include <cstddef>
#include <iostream>
#include "DCPU.h"

DCPU::DCPU()
{
	reset();
}

void DCPU::run(unsigned int numSteps)
{
	while(PC < numSteps)
	//for(unsigned int i = 0; i < numSteps; ++i)
	{
		executeInstruction(NEXT_WORD);
	}
}

void DCPU::reset()
{
	shouldExecute = true;
	A = B = C = X = Y = Z = I = J = PC = SP = EX = IA = 0;
	resetRAM();
	clearHardwareConnections();
}

void DCPU::connectHardware(Hardware *_hardware)
{
	for(unsigned int i = 0; i < MAX_CONNECTED_HARDWARE; ++i)
	{
		if(hardware[i] == nullptr)
		{
			hardware[i] = _hardware;
			return;
		}
	}
	// If we reach here, the maximum number of
	// connected hardware has been exceeded.
	std::cerr << "Attempt to connect new hardware failed." << std::endl;
	std::cerr << "Reason: Max number of connected hardware exceeded" << std::endl;
	std::cerr << _hardware->toString() << std::endl;
}

void DCPU::disconnectHardware(Hardware *_hardware)
{
	for(unsigned int i = 0; i < MAX_CONNECTED_HARDWARE; ++i)
	{
		if(hardware[i] == _hardware)
		{
			//Disconnect
			hardware[i] = nullptr;
			return;
		}
	}
	// If we reach here, _hardware wasn't
	// connected anyway, so we don't have
	// to do anything.
	// TODO: Consider adding a warning if
	//       an attempt to disconnect a
	//       disconnected hardware is made
}

unsigned int DCPU::getNumConnectedHardware() const
{
	unsigned int num = 0;
	for(unsigned int i = 0; i < MAX_CONNECTED_HARDWARE; ++i)
	{
		if(hardware[i])
		{
			num++;
		}
	}
	return num;
}

void DCPU::printState() const
{
	std::cout
		<<   "A="  << A  << ", B="  << B  << ", C="  << C  << ", X="  << X
		<< ", Y="  << Y  << ", Z="  << Z  << ", I="  << I  << ", J="  << J
		<< ", PC=" << PC << ", SP=" << SP << ", EX=" << EX << ", IA=" << IA
		<< ", shExe=" << shouldExecute << std::endl;
	for(unsigned int i = 0; i < RAM_SIZE; ++i)
	{
		if(RAM[i])
		{
			std::cout << "RAM[" << i << "] =\t" << RAM[i] << std::endl;
		}
	}
	for(unsigned int i = 0; i < MAX_CONNECTED_HARDWARE; ++i)
	{
		if(hardware[i])
		{
			std::cout << "hardware[" << i << "] =\t" << hardware[i] << std::endl;
		}
	}
}

void DCPU::executeInstruction(const DCPU_WORD instruction)
{
	if(!shouldExecute)
	{
		shouldExecute = true;
		return;
	}
	DCPU_WORD a = (instruction >> 10) & 0x3F;
	DCPU_WORD b = (instruction >> 5) & 0x1F;
	DCPU_WORD opcode = instruction & 0x1F;
	DCPU_WORD *ap = nullptr, *bp = nullptr;
	// 0x20-0x3F: literal value 0xFFFF-0x1E (-1..30) (only for a)
	if(a >= 0x20 && a <= 0x3F)
	{
		DCPU_WORD value = a - 0x21;
		ap = &value;
	}
	else
	{
		ap = getValue(a, true);
	}
	bp = getValue(b, false);
	switch(opcode)
	{
	case 0x00:
		// Special instruction
		executeSpecialInstruction(b, ap);
		break;
	case 0x01:
		// SET b, a
		*bp = *ap;
		break;
	case 0x02:
		// ADD b, a
		{
			DCPU_WORD result = *bp + *ap;
			EX = result < *bp ? 0x0001 : 0x0;
			*bp = result;
		}
		break;
	case 0x03:
		// SUB b, a
		{
			DCPU_WORD result = *bp - *ap;
			EX = result > *bp ? 0xFFFF : 0x0;
			*bp = result;
		}
		break;
	case 0x04:
		// MUL b, a
		*bp *= *ap;
		EX = (*bp * *ap) >> 16;
		break;
	case 0x05:
		// MLI b, a
		*bp = (DCPU_WORD_S)*bp * (DCPU_WORD_S)*ap;
		EX = ((DCPU_WORD_S)*bp * (DCPU_WORD_S)*ap) >> 16;
		break;
	case 0x06:
		// DIV b, a
		if(*ap)
		{
			*bp /= *ap;
			EX = (((uint32_t)b << 16) / a) & 0xFFFF;
		}
		else
		{
			*bp = EX = 0;
		}
		break;
	case 0x07:
		// DVI b, a
		*bp = (DCPU_WORD_S)*bp / (DCPU_WORD_S)*ap;
		// TODO: Check whether there should be EX
		//       and divide-by-zero handling here.
		break;
	case 0x08:
		// MOD b, a
		if(*ap)
		{
			*bp %= *ap;
		}
		else
		{
			*bp = 0;
		}
		break;
	case 0x09:
		// MDI b, a
		if((DCPU_WORD_S)*bp > 0)
		{
			if(*ap)
			{
				*bp %= *ap;
			}
			else
			{
				*bp = 0;
			}
		}
		// TODO: Check this is actually right
		break;
	case 0x0A:
		// AND b, a
		*bp &= *ap;
		break;
	case 0x0B:
		// BOR b, a
		*bp |= *ap;
		break;
	case 0x0C:
		// XOR b, a
		*bp ^= *ap;
		break;
	case 0x0D:
		// SHR b, a
		*bp >>= *ap;
		EX = (*bp << 16) >> *ap;
		break;
	case 0x0E:
		// ASR b, a
		// TODO: Ensure this is working.
		*bp = ((DCPU_WORD_S)*bp) >> *ap;
		EX = (*bp << 16) >> *ap;
		break;
	case 0x0F:
		// SHL b, a
		*bp <<= *ap;
		EX = (*bp << *ap) >> 16;
		break;
	case 0x10:
		// IFB b, a
		shouldExecute = (*bp & *ap) != 0;
		break;
	case 0x11:
		// IFC b, a
		shouldExecute = (*bp & *ap) == 0;
		break;
	case 0x12:
		// IFE b, a
		shouldExecute = *bp == *ap;
		break;
	case 0x13:
		// IFN b, a
		shouldExecute = *bp != *ap;
		break;
	case 0x14:
		// IFG b, a
		shouldExecute = *bp > *ap;
		break;
	case 0x15:
		// IFA b, a
		shouldExecute = (DCPU_WORD_S)*bp > (DCPU_WORD_S)*ap;
		break;
	case 0x16:
		// IFL b, a
		shouldExecute = *bp < *ap;
		break;
	case 0x17:
		// IFU b, a
		shouldExecute = (DCPU_WORD_S)*bp < (DCPU_WORD_S)*ap;
		break;
	case 0x1A:
		// ADX b, a
		{
			DCPU_WORD result = *bp + *ap + EX;
			EX = result < *bp ? 0x0001 : 0x0;
			*bp = result;
		}
		break;
	case 0x1B:
		// SBX b, a
		{
			DCPU_WORD result = *bp - *ap + EX;
			EX = result > *bp ? 0xFFFF : 0x0;
			*bp = result;
		}
		break;
	case 0x1E:
		// STI b, a
		*bp = *ap;
		I++;
		J++;
		break;
	case 0x1F:
		// STD b, a
		*bp = *ap;
		I--;
		J--;
		break;
	default:
		// Opcode not recognized. Just NOP
		break;
	}
}

void DCPU::executeSpecialInstruction(const DCPU_WORD opcode, DCPU_WORD *ap)
{
	switch(opcode)
	{
	case 0x01:
		// JSR a
		// exe(SET PUSH, next word)
		// exe(SET PC a)
		(--SP) = PC + 1;
		PC = *ap;
		break;
	case 0x08:
		// INT a
		// TODO: Triggers a software interrupt with message a
		break;
	case 0x09:
		// IAG a
		*ap = IA;
		break;
	case 0x0A:
		// IAS a
		IA = *ap;
		break;
	case 0x0B:
		// RFI a
		// TODO: Disable interrupt queueing, pop A from the stack,
		//       pop PC from the stack.
		break;
	case 0x0C:
		// IAQ a
		// TODO: If a is nonzero, interrupts will be added to the
		//       queue instead of triggered. Else, interrupts will
		//       be triggered as normal again.
		break;
	case 0x10:
		// HWN a
		*ap = getNumConnectedHardware();
		break;
	case 0x11:
		// HWQ a
		//       A+(B<<16) is a 32 bit word identifying the hardware id
		//       C is the hardware version
		//       X+(Y<<16) is a 32 bit word identifying the manufacturer
		A = hardware[*ap]->id & 0xffff;
		B = (hardware[*ap]->id >> 16) & 0xffff;
		C = hardware[*ap]->version;
		X = hardware[*ap]->manufacturer & 0xffff;
		Y = (hardware[*ap]->manufacturer >> 16) & 0xffff;
		break;
	case 0x12:
		// HWI a
		// TODO: Sends an interrupt to hardware a.
		break;
	default:
		// Opcode not recognized. Just NOP
		break;
	}
}

DCPU_WORD * DCPU::getValue(const DCPU_WORD val, const bool a)
{
	switch(val)
	{
		// 0x00-0x07: register (A, B, C, X, Y, Z, I or J, in that order)
	case 0x00:	return &A;
	case 0x01:	return &B;
	case 0x02:	return &C;
	case 0x03:	return &X;
	case 0x04:	return &Y;
	case 0x05:	return &Z;
	case 0x06:	return &I;
	case 0x07:	return &J;
		// 0x08-0x0F: [register]
	case 0x08:	return &RAM[A];
	case 0x09:	return &RAM[B];
	case 0x0A:	return &RAM[C];
	case 0x0B:	return &RAM[X];
	case 0x0C:	return &RAM[Y];
	case 0x0D:	return &RAM[Z];
	case 0x0E:	return &RAM[I];
	case 0x0F:	return &RAM[J];
		// 0x10-0x17: [register + next word]
	case 0x10:	return &RAM[A + NEXT_WORD];
	case 0x11:	return &RAM[B + NEXT_WORD];
	case 0x12:	return &RAM[C + NEXT_WORD];
	case 0x13:	return &RAM[X + NEXT_WORD];
	case 0x14:	return &RAM[Y + NEXT_WORD];
	case 0x15:	return &RAM[Z + NEXT_WORD];
	case 0x16:	return &RAM[I + NEXT_WORD];
	case 0x17:	return &RAM[J + NEXT_WORD];
		// 0x18: (PUSH / [--SP]) if in b, or (POP / [SP++]) if in a
	case 0x18:	return a ? &POP : &PUSH;
		// 0x19: [SP] / PEEK
	case 0x19:	return &PEEK;
		// 0x1A: [SP + next word] / PICK n
	case 0x1A:	return &PICK;
		// 0x1B: SP
	case 0x1B:	return &SP;
		// 0x1C: PC
	case 0x1C:	return &PC;
		// 0x1D: EX
	case 0x1D:	return &EX;
		// 0x1E: [next word]
	case 0x1E:	return &RAM[NEXT_WORD];
		// 0x1F: next word (literal)
	case 0x1F:	return &NEXT_WORD;
		// To make the compiler happy that
		// all paths return a value:
	default:	return nullptr;
	}
}

void DCPU::resetRAM()
{
	for(unsigned int i = 0; i < RAM_SIZE; ++i)
	{
		RAM[i] = 0;
	}
}

void DCPU::clearHardwareConnections()
{
	for(unsigned int i = 0; i < MAX_CONNECTED_HARDWARE; ++i)
	{
		hardware[i] = nullptr;
	}
}

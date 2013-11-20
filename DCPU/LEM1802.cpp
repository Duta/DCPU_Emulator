#include "LEM1802.h"

LEM1802::LEM1802()
	: Hardware(0x7349f615, 0x1802, 0x1c6c8b36), dcpu(nullptr),
	  videoRAMStart(nullptr), fontRAMStart(0), paletteRAMStart(0), borderColor(0) {}

void LEM1802::interrupt(DCPU *dcpu)
{
	this->dcpu = dcpu;
	switch(dcpu->A)
	{
	case 0: // MEM_MAP_SCREEN
		videoRAMStart = &dcpu->RAM[dcpu->B];
		if(!videoRAMStart)
		{
			this->dcpu = nullptr;
		}
		break;
	case 1: // MEM_MAP_FONT
		fontRAMStart = dcpu->B ? &dcpu->RAM[dcpu->B] : DEFAULT_FONT;
		break;
	case 2: // MEM_MAP_PALETTE
		paletteRAMStart = dcpu->B ? &dcpu->RAM[dcpu->B] : DEFAULT_PALETTE;
		break;
	case 3: // SET_BORDER_COLOR
		borderColor = &paletteRAMStart[dcpu->B & 0xF];
		break;
	}
}

bool LEM1802::isConnected() const
{
	return dcpu != nullptr;
}

const DCPU_WORD LEM1802::DEFAULT_FONT[256] =
{
	// ASCII format
	0xFF81, 0x81FF, // (\0) Null character
	0xFF81, 0x81FF, // Start of Header
	0xFF81, 0x81FF, // Start of Text
	0xFF81, 0x81FF, // End of Text
	0xFF81, 0x81FF, // End of Transmission
	0xFF81, 0x81FF, // Enquiry
	0xFF81, 0x81FF, // Acknowledgement
	0xFF81, 0x81FF, // (\a) Bell
	0xFF81, 0x81FF, // (\b) Backspace
	0xFF81, 0x81FF, // (\t) Horizontal Tab
	0xFF81, 0x81FF, // (\n) Line feed
	0xFF81, 0x81FF, // (\v) Vertical Tab
	0xFF81, 0x81FF, // (\f) Form feed
	0xFF81, 0x81FF, // (\r) Carriage return
	0xFF81, 0x81FF, // Shift Out
	0xFF81, 0x81FF, // Shift In
	0xFF81, 0x81FF, // Data Link Escape
	0xFF81, 0x81FF, // Device Control 1
	0xFF81, 0x81FF, // Device Control 2
	0xFF81, 0x81FF, // Device Control 3
	0xFF81, 0x81FF, // Device Control 4
	0xFF81, 0x81FF, // Negative Acknowledgement
	0xFF81, 0x81FF, // Synchronous idle
	0xFF81, 0x81FF, // End of Transmission Block
	0xFF81, 0x81FF, // Cancel
	0xFF81, 0x81FF, // End of Medium
	0xFF81, 0x81FF, // Substitute
	0xFF81, 0x81FF, // Escape
	0xFF81, 0x81FF, // File Separator
	0xFF81, 0x81FF, // Group Separator
	0xFF81, 0x81FF, // Record Separator
	0xFF81, 0x81FF, // Unit Separator
	0xFF81, 0x81FF, //  
	0xFF81, 0x81FF, // !
	0xFF81, 0x81FF, // "
	0xFF81, 0x81FF, // #
	0xFF81, 0x81FF, // $
	0xFF81, 0x81FF, // %
	0xFF81, 0x81FF, // &
	0xFF81, 0x81FF, // '
	0xFF81, 0x81FF, // (
	0xFF81, 0x81FF, // )
	0xFF81, 0x81FF, // *
	0xFF81, 0x81FF, // +
	0xFF81, 0x81FF, // ,
	0xFF81, 0x81FF, // -
	0xFF81, 0x81FF, // .
	0xFF81, 0x81FF, // /
	0x7E81, 0x7E00, // 0
	0x82FF, 0x8000, // 1
	0xE691, 0x8E00, // 2
	0x4289, 0x7600, // 3
	0x0F08, 0xFF00, // 4
	0x8F89, 0x7100, // 5
	0xFF91, 0xF100, // 6
	0x0101, 0xFF00, // 7
	0x7689, 0x7600, // 8
	0x4689, 0x7600, // 9
	0xFF81, 0x81FF, // :
	0xFF81, 0x81FF, // ;
	0xFF81, 0x81FF, // <
	0xFF81, 0x81FF, // =
	0xFF81, 0x81FF, // >
	0xFF81, 0x81FF, // ?
	0xFF81, 0x81FF, // @
	0xFE09, 0xFE00, // A
	0xFF89, 0x7600, // B
	0x7E81, 0x8100, // C
	0xFF81, 0x7E00, // D
	0xFF89, 0x8900, // E
	0xFF09, 0x0900, // F
	0x7E81, 0xF100, // G
	0xFF08, 0xFF00, // H
	0x81FF, 0x8100, // I
	0x4181, 0x7F00, // J
	0xFF08, 0xF700, // K
	0xFF80, 0x8000, // L
	0xFF02, 0xFF00, // M - Pretty terrible.
	0xFF81, 0x81FF, // N
	0xFF81, 0x81FF, // O
	0xFF81, 0x81FF, // P
	0xFF81, 0x81FF, // Q
	0xFF81, 0x81FF, // R
	0xFF81, 0x81FF, // S
	0xFF81, 0x81FF, // T
	0xFF81, 0x81FF, // U
	0xFF81, 0x81FF, // V
	0xFF81, 0x81FF, // W
	0xFF81, 0x81FF, // X
	0xFF81, 0x81FF, // Y
	0xFF81, 0x81FF, // Z
	0xFF81, 0x81FF, // [
	0xFF81, 0x81FF, // \ 
	0xFF81, 0x81FF, // ]
	0xFF81, 0x81FF, // ^
	0xFF81, 0x81FF, // _
	0xFF81, 0x81FF, // `
	0xFF81, 0x81FF, // a
	0xFF81, 0x81FF, // b
	0xFF81, 0x81FF, // c
	0xFF81, 0x81FF, // d
	0xFF81, 0x81FF, // e
	0xFF81, 0x81FF, // f
	0xFF81, 0x81FF, // g
	0xFF81, 0x81FF, // h
	0xFF81, 0x81FF, // i
	0xFF81, 0x81FF, // j
	0xFF81, 0x81FF, // k
	0xFF81, 0x81FF, // l
	0xFF81, 0x81FF, // m
	0xFF81, 0x81FF, // n
	0xFF81, 0x81FF, // o
	0xFF81, 0x81FF, // p
	0xFF81, 0x81FF, // q
	0xFF81, 0x81FF, // r
	0xFF81, 0x81FF, // s
	0xFF81, 0x81FF, // t
	0xFF81, 0x81FF, // u
	0xFF81, 0x81FF, // v
	0xFF81, 0x81FF, // w
	0xFF81, 0x81FF, // x
	0xFF81, 0x81FF, // y
	0xFF81, 0x81FF, // z
	0xFF81, 0x81FF, // {
	0xFF81, 0x81FF, // |
	0xFF81, 0x81FF, // }
	0xFF81, 0x81FF, // ~
	0xFF81, 0x81FF  // Delete
};

const DCPU_WORD LEM1802::DEFAULT_PALETTE[16] =
{
	0x101,
	0x423,
	0x336,
	0x444,
	0x843,
	0x362,
	0xD44,
	0x776,
	0x57C,
	0xD72,
	0x89A,
	0x6A2,
	0xDA9,
	0x6CC,
	0xDD5,
	0xDED
};
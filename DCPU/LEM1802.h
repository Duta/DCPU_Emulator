#pragma once

#include "DCPU.h"
#include "Hardware.h"

class LEM1802 : public Hardware
{
public:
	LEM1802();

	void interrupt(DCPU *);
private:
	bool isConnected() const;

	// The DCPU the LEM1802 is connected to
	const DCPU *dcpu;
	// Start of video RAM
	const DCPU_WORD *videoRAMStart;
	// Start of font RAM
	const DCPU_WORD *fontRAMStart;
	// Start of palette RAM
	const DCPU_WORD *paletteRAMStart;
	// Border colour
	const DCPU_WORD *borderColor;

	static const DCPU_WORD DEFAULT_FONT[256];   // These should be const but then can't
	static const DCPU_WORD DEFAULT_PALETTE[16]; // assign them to non-const pointers.
};

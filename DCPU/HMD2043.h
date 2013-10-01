#pragma once

#include "DCPU.h"
#include "Disk.h"
#include "Hardware.h"

class HMD2043 : public Hardware
{
public:
	HMD2043();

	void interrupt(DCPU *);
	void loadDisk(Disk *disk);
	void ejectDisk();
private:
	bool isSupported(const Disk *) const;
	bool isHIT(const Disk *) const;

	// The DCPU the HMD2043 is connected to
	DCPU *dcpu;
	// The disk in the drive
	Disk *disk;
	// Should slow operations be non-blocking
	bool nonBlocking;
	// Should an interrupt be raised on a media status change
	bool mediaStatusInterrupts;
	// Interrupt message
	DCPU_WORD interruptMessage;
	// The type of interrupt the HMD2043 last raised
	uint8_t lastInterruptType;
	uint8_t lastInterruptErrorStatus;
};

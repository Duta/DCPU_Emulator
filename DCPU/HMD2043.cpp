#include <iostream>
#include "HMD2043.h"
#include "HMU1440.h"

HMD2043::HMD2043()
	: Hardware(0x74FA4CAE, 0x07C2, 0x21544948), dcpu(nullptr),
	  disk(nullptr), nonBlocking(false), mediaStatusInterrupts(false),
	  interruptMessage(0xFFFF), lastInterruptType(0), lastInterruptErrorStatus(0) {}

void HMD2043::interrupt(DCPU *dcpu)
{
#define ERROR_NONE 0
#define ERROR_NO_MEDIA 1
#define ERROR_INVALID_SECTOR 2
#define ERROR_PENDING 3
	if(this->dcpu != dcpu)
	{
		lastInterruptType = 0; // NONE
		lastInterruptErrorStatus = ERROR_NONE;
	}
	this->dcpu = dcpu;
	switch(dcpu->A)
	{
	case 0x0000: // QUERY_MEDIA_PRESENT
		dcpu->B = isSupported(disk) ? 1 : 0;
		dcpu->A = ERROR_NONE;
		break;
	case 0x0001: // QUERY_MEDIA_PARAMETERS
		if(isSupported(disk))
		{
			dcpu->B = disk->wordsPerSector;
			dcpu->C = disk->numSectors;
			dcpu->X = disk->writeLocked ? 1 : 0;
			dcpu->A = ERROR_NONE;
		}
		else
		{
			dcpu->A = ERROR_NO_MEDIA;
		}
		break;
	case 0x0002: // QUERY_DEVICE_FLAGS
		dcpu->B = ((nonBlocking ? 1 : 0) << 15) +
			((mediaStatusInterrupts ? 1 : 0) << 14);
		dcpu->A = ERROR_NONE;
		break;
	case 0x0003: // UPDATE_DEVICE_FLAGS
		nonBlocking = dcpu->B >> 15 ? 1 : 0;
		mediaStatusInterrupts = dcpu->B >> 14 ? 1 : 0;
		dcpu->A = ERROR_NONE;
		break;
	case 0x0004: // QUERY_INTERRUPT_TYPE
		dcpu->B = lastInterruptType;
		dcpu->A = lastInterruptErrorStatus;
		break;
	case 0x0005: // SET_INTERRUPT_MESSAGE
		interruptMessage = dcpu->B;
		dcpu->A = ERROR_NONE;
		break;
	case 0x0010: // READ_SECTORS
		// TODO: Add non-blocking support
		std::cout << "HMD2043: READ_SECTORS<" << dcpu->B << "," << dcpu->C << "," << dcpu->X << ">...";
		if(isSupported(disk))
		{
			bool success = disk->read(dcpu->B, dcpu->C, &dcpu->RAM[dcpu->X]);
			dcpu->A = success ? ERROR_NONE : ERROR_INVALID_SECTOR;
			std::cout << (success ? "Success" : "Failed (invalid sector)") << std::endl;
		}
		else
		{
			dcpu->A = ERROR_NO_MEDIA;
			std::cout << "Failed (no media)" << std::endl;
		}
		break;
	case 0x0011: // WRITE_SECTORS
		// TODO: Add non-blocking support
		std::cout << "HMD2043: WRITE_SECTORS<" << dcpu->B << "," << dcpu->C << "," << dcpu->X << ">...";
		if(isSupported(disk))
		{
			bool success = disk->read(dcpu->B, dcpu->C, &dcpu->RAM[dcpu->X]);
			dcpu->A = success ? ERROR_NONE : ERROR_INVALID_SECTOR;
			std::cout << (success ? "Success" : "Failed (invalid sector)") << std::endl;
		}
		else
		{
			dcpu->A = ERROR_NO_MEDIA;
			std::cout << "Failed (no media)" << std::endl;
		}
		break;
	case 0xFFFF: // QUERY_MEDIA_QUALITY
		if(isSupported(disk))
		{
			dcpu->B = isHIT(disk) ? 0x7FFF : 0xFFFF;
		}
		else
		{
			dcpu->A = ERROR_NO_MEDIA;
		}
		break;
	}
#undef ERROR_NONE
#undef ERROR_NO_MEDIA
#undef ERROR_INVALID_SECTOR
#undef ERROR_PENDING
}

void HMD2043::loadDisk(Disk *disk)
{
	if(this->disk == disk)
	{
		return;
	}
	this->disk = disk;
	if(dcpu != nullptr && mediaStatusInterrupts)
	{
		dcpu->IA = interruptMessage;
		lastInterruptType = 1; // MEDIA_STATUS
		lastInterruptErrorStatus = 0; // ERROR_NONE
	}
}

void HMD2043::ejectDisk()
{
	if(disk == nullptr)
	{
		return;
	}
	disk = nullptr;
	if(dcpu != nullptr && mediaStatusInterrupts)
	{
		dcpu->IA = interruptMessage;
		lastInterruptType = 1; // MEDIA_STATUS
		lastInterruptErrorStatus = 0; // ERROR_NONE
	}
}

bool HMD2043::isSupported(const Disk *disk) const
{
	// This emulation of the HMD2043 supports all disks
	return disk != nullptr;
}

bool HMD2043::isHIT(const Disk *disk) const
{
	if(disk == nullptr)
	{
		return false;
	}
	return dynamic_cast<const HMU1440 *>(disk) != nullptr;
}

#pragma once

#include <cstdint>

class Disk
{
public:
	Disk(uint16_t, uint16_t, bool);
	
	virtual bool read(const uint16_t initialSector, const uint16_t numSectors, uint16_t *bufferStart) const = 0;
	virtual bool write(const uint16_t initialSector, const uint16_t numSectors, uint16_t *bufferStart) = 0;
	
	const uint16_t wordsPerSector;
	const uint16_t numSectors;
	const bool writeLocked;
};

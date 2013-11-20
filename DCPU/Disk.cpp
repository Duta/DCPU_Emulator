#include "Disk.h"

Disk::Disk(uint16_t wordsPerSector, uint16_t numSectors, bool writeLocked)
    : wordsPerSector(wordsPerSector), numSectors(numSectors), writeLocked(writeLocked) {}

#include "HMU1440.h"

HMU1440::HMU1440()
    : Disk(512, 1440, false)
{
    // Allocates sectors[512][1440] on the heap
    sectors = new uint16_t*[wordsPerSector];
    for(int i = 0; i < wordsPerSector; ++i)
    {
        sectors[i] = new uint16_t[numSectors];
    }
    zeroAllSectors();
}

HMU1440::~HMU1440()
{
    for(int i = 0; i < wordsPerSector; ++i)
    {
        delete[] sectors[i];
    }
    delete[] sectors;
}

bool HMU1440::read(const uint16_t initialSector, const uint16_t numSectors, uint16_t *bufferStart) const
{
    for(uint16_t sector = initialSector; sector < initialSector + numSectors; ++sector)
    {
        if(sector >= this->numSectors)
        {
            return false;
        }
        for(uint16_t word = 0; word < wordsPerSector; ++word)
        {
            *bufferStart++ = sectors[word][sector];
        }
    }
    return true;
}

bool HMU1440::write(const uint16_t initialSector, const uint16_t numSectors, uint16_t *bufferStart)
{
    for(uint16_t sector = initialSector; sector < initialSector + numSectors; ++sector)
    {
        if(sector >= this->numSectors)
        {
            return false;
        }
        for(uint16_t word = 0; word < wordsPerSector; ++word)
        {
            sectors[word][sector] = *bufferStart++;
        }
    }
    return true;
}

void HMU1440::zeroAllSectors()
{
    for(uint16_t sector = 0; sector < numSectors; ++sector)
    {
        for(uint16_t word = 0; word < wordsPerSector; ++word)
        {
            sectors[word][sector] = 0;
        }
    }
}

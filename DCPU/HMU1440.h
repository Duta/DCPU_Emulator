#pragma once

#include "Disk.h"

class HMU1440 : public Disk
{
public:
	HMU1440();
	~HMU1440();
	
	bool read(const uint16_t, const uint16_t, uint16_t *) const;
	bool write(const uint16_t, const uint16_t, uint16_t *);
private:
	void zeroAllSectors();

	uint16_t **sectors;
};

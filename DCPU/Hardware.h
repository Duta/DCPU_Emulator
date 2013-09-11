#pragma once

#include <string>
#include <cstdint>

class Hardware
{
public:
	Hardware(uint32_t, uint16_t, uint32_t);

	std::string toString() const;

	uint32_t id;
	uint16_t version;
	uint32_t manufacturer;
};
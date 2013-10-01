#include <sstream>
#include "Hardware.h"

Hardware::Hardware(uint32_t id, uint16_t version, uint32_t manufacturer)
	: id(id), version(version), manufacturer(manufacturer) {}

std::string Hardware::toString() const
{
	std::stringstream ss;
	ss << std::hex;
	ss << "{id=0x" << id;
	ss << ",version=0x" << version;
	ss << ",manufacturer=0x" << manufacturer;
	ss << "}";
	return ss.str();
}

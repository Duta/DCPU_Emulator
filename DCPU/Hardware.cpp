#include "Hardware.h"

Hardware::Hardware(uint32_t id, uint16_t version, uint32_t manufacturer)
	: id(id), version(version), manufacturer(manufacturer) {}

std::string Hardware::toString() const
{
	std::string s("Hardware{");
	s +=  "id=" + id;
	s += " version=" + version;
	s += " manufacturer=" + manufacturer;
	s += "}";
	return s;
}

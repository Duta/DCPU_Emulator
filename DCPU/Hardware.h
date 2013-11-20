#pragma once

#include <string>
#include <cstdint>
#include "DCPU.h"

class DCPU;

class Hardware
{
public:
    Hardware(uint32_t, uint16_t, uint32_t);

    virtual void interrupt(DCPU *) = 0;
    std::string toString() const;

    uint32_t id;
    uint16_t version;
    uint32_t manufacturer;
};

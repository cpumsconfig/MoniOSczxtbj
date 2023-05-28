typedef unsigned short u_short;
typedef unsigned int u_int;
typedef unsigned char u_char;
typedef unsigned long u_long;
typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned long uint32_t;
#ifndef FRTOS_GDT_H
#define FRTOS_GDT_H

#include "types.h"

class GlobalDescriptorTable {
public:
    class SegementDescriptor {
    private:
        uint16_t limit_lo;
        uint16_t base_io;
        uint16_t base_hi;
        uint16_t type;
        uint16_t flags_limit_hi;
        uint16_t base_vhi;
    public:
        SegementDescriptor(uint32_t base, uint32_t limit, uint8_t type);

        uint32_t Base();

        uint32_t Limit();

    };

    SegementDescriptor nullSegementDescriptor;
    SegementDescriptor unusedSegementDescriptor;
    SegementDescriptor codeSegementDescriptor;
    SegementDescriptor dataSegementDescriptor;


    GlobalDescriptorTable();
    ~GlobalDescriptorTable();

    uint16_t CodeSegementSelector();

    uint16_t DataSegementSelector();

};

#endif

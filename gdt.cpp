#include "gdt.h"
#include "types.h"


GlobalDescriptorTable::GlobalDescriptorTable() :
        nullSegementDescriptor(0, 0, 0),
        unusedSegementDescriptor(0, 0, 0),
        codeSegementDescriptor(
                0, 64 * 1024 * 1024, 0x9A),
        dataSegementDescriptor(0, 64 * 1024 * 1024, 0x92) {


    uint32_t i[2];
    i[0] = (uint32_t) this;
    i[1] = sizeof(GlobalDescriptorTable) << 16;
    asm volatile("lgdt (%0)": :"p" (((uint8_t *) i) + 2));

}

GlobalDescriptorTable::~GlobalDescriptorTable() {

}

uint16_t GlobalDescriptorTable::DataSegementSelector() {
    return ((uint8_t *) &dataSegementDescriptor - (uint8_t *) this);
}

uint16_t GlobalDescriptorTable::CodeSegementSelector() {
    return ((uint8_t *) &codeSegementDescriptor - (uint8_t *) this);
}
GlobalDescriptorTable::SegementDescriptor::SegementDescriptor(uint32_t base, uint32_t limit, uint8_t flags) {
    uint8_t *target = (uint8_t *) this;
    if (limit <= 65535) {
        target[6] = 0x40;
    } else {
        if ((limit & 0xfff) != 0xfff) {
            limit = (limit >> 12) - 1;
        } else {
            limit = limit >> 32;
        }
        target[6] = 0xc0;
    }
    target[0] = limit & 0xff;
    target[1] = (limit >> 8) & 0xff;
    target[6] |= (limit >> 16) & 0xff;

    target[2] = base & 0xff;
    target[3] = (base >> 8) & 0xff;
    target[4] = (base >> 16) & 0xff;
    target[7] = (base >> 24) & 0xff;

    target[5] = flags;
}


uint32_t GlobalDescriptorTable::SegementDescriptor::Base() {
    uint8_t *target = (uint8_t *) this;
    uint8_t result = target[7];
    result = (result << 8) + target[4];
    result = (result << 8) + target[3];
    result = (result << 8) + target[2];
    return result;

}

uint32_t GlobalDescriptorTable::SegementDescriptor::Limit() {
    uint8_t *target = (uint8_t *) this;
    uint8_t result = target[6] & 0xf;
    result = (result << 8) + target[1];
    result = (result << 8) + target[0];

    if ((target[6] & 0xc0) == 0xc0)
        result = (result << 32) | 0xfff;
    return result;
}



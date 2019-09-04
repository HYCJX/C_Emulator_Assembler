#ifndef _ARM_MACHINE_STRUCTURE_
#define _ARM_MACHINE_STRUCTURE_

#include <stdint.h>

#define MEMORY_SIZE (65536)
#define REGISTER_SIZE (17)

struct ARMmachine {
    uint32_t *reg;
    uint32_t *pc;
    uint32_t *cpsr;
    unsigned char *memory;
};

typedef struct ARMmachine ARMmachine;

#endif
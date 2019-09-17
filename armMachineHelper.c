#include "armMachineHelper.h"

//A helper function that is called if the given memory address in load/store data is out of bound.
//Print out the GPIO status and return true if the memAdd stands for a GPIO pin.
//Print error and return false otherwise.
//@param memAdd: Given memory address.
static bool checkGPIO(uint32_t memAdd)
{
    switch (memAdd) {
        case 0x20200000:
            printf("One GPIO pint from 0 to 9 has been accessed\n");
            break;
        case 0x20200004:
            printf("One GPIO pin from 10 to 19 has been accessed\n");
            break;
        case 0x20200008:
            printf("One GPIO pin from 20 to 29 has been accessed\n");
            break;
        case 0x2020001c:
            printf("PIN ON\n");
            break;
        case 0x20200028:
            printf("PIN OFF\n");
            break;
        default: {
            printf("Error: Out of bounds memory access at address0x%08x\n", memAdd);
            return false;
        }   
    }
    return true;
}

bool getCpsrBit(char flag, ARMmachine *machine)
{
    if (flag >= 'a') flag -= ('a'- 'A');
    int shift;
    switch (flag) {
    case 'N':
        shift = 31;
        break;
    case 'Z':
        shift = 30;
        break;
    case 'C':
        shift = 29;
        break;
    case 'V':
        shift = 28;
        break;
    default:
        perror("wrong flag!");
        break;
    }
    return getBit(*(machine -> cpsr), shift);
}

bool store32BitToMem(uint32_t content, uint32_t memAdd, ARMmachine * machine)
{
    if (memAdd + 3 >= MEMORY_SIZE) {
        //GPIO handling:
        checkGPIO(memAdd);
        return false;
    }
    for (int i = 0; i < 4; i++) {
        unsigned char byte = getBitSeq(content, 0, 7);
        machine -> memory[memAdd + i] = byte;
        content >>= 8;
    }
    return true;
}

uint32_t get32bitIns(uint32_t memAdd, ARMmachine *machine)
{
    uint32_t result = 0;
    if (memAdd + 3 >= MEMORY_SIZE) {
        //GPIO handling:
        return checkGPIO(memAdd) ? memAdd : 0;
    }
    for (int i = 3; i >= 0; i--) {
        result <<= 8;
        result += machine -> memory[memAdd + i];
    }
    return result;
}

uint32_t get32bitValue(uint32_t memAdd, ARMmachine *machine)
{
    uint32_t memValue = 0;
    for (int i = memAdd; i < memAdd + 4; i++) {
        memValue <<= 8;
        memValue += machine -> memory[i];
    }
    return memValue;
}

void setCpsrBit(char flag, bool givenBit, ARMmachine *machine)
{
    if (flag >= 'a') flag -= ('a'- 'A');
    int shift;
    switch (flag) {
    case 'N':
        shift = 31;
        break;
    case 'Z':
        shift = 30;
        break;
    case 'C':
        shift = 29;
        break;
    case 'V':
        shift = 28;
        break;
    default:
        perror("wrong flag!");
        return;
    }
    return setBit(machine -> cpsr, shift, givenBit);   
}
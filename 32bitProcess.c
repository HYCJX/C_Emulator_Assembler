#include "32bitProcess.h"

bool getBit(uint32_t instruction, int index)
{
    return (instruction & (1 << index)) != 0;
}

bool isBitsMatch(uint32_t instruction, int start, int end, char *bitSequence)
{
    int j = 0;
    for (int i = end; i >= start; --i, ++j) {
        if (bitSequence[j] - '0' != getBit(instruction, i)) {
            return false;
        }
    }
    return (bitSequence[j] == '\0');
}

unsigned char reverseByte(unsigned char instruction)
{
    unsigned char reversedNum = 0;
    int count = 0;
    while (instruction != 0) {
        reversedNum <<= 1;
        reversedNum |= (instruction & 1);
        instruction >>= 1;
        count--;
    }
    reversedNum <<= count;
    return reversedNum;
}

uint32_t arithShift32Bit(char direction, uint32_t x, unsigned int amount, bool *cPtr)
{
    if (amount == 0) {
        *cPtr = 0;
        return x;
    }
    if (direction == 'L') {
        *cPtr = getBit(x << (amount - 1), 31);
        return x << amount;
    } else if (direction == 'R') {
        *cPtr = getBit(x >> (amount - 1), 31);
        bool c = getBit(x, 31);
        uint32_t temp = x >> amount;
        if (c) {
            temp |= ((~0) >> (32 - amount)) << (32 - amount);
        }
        return temp;
    }
    return x;
}

uint32_t getBitSeq(uint32_t instruction, int start, int end)
{
    return (instruction << (31 - end)) >> (31 - end + start);
}

uint32_t logicalShift32Bit(char direction, uint32_t x, unsigned int amount, bool *cPtr)
{
    if (amount == 0) {
        *cPtr = 0;
        return x;
    }
    if (direction == 'L') {
        *cPtr = getBit(x << (amount -  1), 31);
        return x << amount;
    } else if (direction == 'R') {
        *cPtr = getBit(x >> (amount - 1), 0);
        return x >> amount;
    }
    return x;
}

uint32_t reverse32Bit(uint32_t instruction)
{
    uint32_t reversedNum = 0;
    int count = 32;
    while (instruction != 0) {
        reversedNum <<= 1;
        reversedNum |= (instruction & 1);
        instruction >>= 1;
        count--;
    }
    reversedNum <<= count;
    return reversedNum;
}

uint32_t rotate32Bit(char direction, uint32_t x, unsigned int amount, bool *cPtr)
{
    uint32_t rotate;
    if (direction == 'L') {
        rotate = getBitSeq(x, 32 - amount, 31);
        *cPtr = (rotate & 1);
        return (x << amount) | rotate;
    } else if (direction == 'R') {
        rotate = getBitSeq(x, 0, amount) << (32 - amount);
        *cPtr = (rotate & (1 << 31));
        return (x >> amount) | rotate;
    }
    return x;
}

uint32_t signExtend(uint32_t number, unsigned int length)
{
    bool x = getBit(number, length - 1);
    if (x == 1) {
        number |= (0xffffffff << (length - 1));
    }
    return number;
}

void printlnBinary(uint32_t instruction)
{
    int i;
    uint32_t mask = 1 << 31;
    for (i = 0; i < 32; ++i) {
        if ((instruction & mask) == 0) {
            printf("0");
        } else {
            printf("1");
        }
        instruction <<= 1;
    }
    printf("\n");
}

void setBit(uint32_t *dest, unsigned int index, bool givenBit)
{
    if (givenBit == 1) {
        *dest |= 1 << index;
    } else {
        *dest &= ~(1 << index);
    }
}
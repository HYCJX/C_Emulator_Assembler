#include "emulateIO.h"

//A helper function.
//Convert the given number into string.
//Align the result to the right if it is shorter than the given length.
//Fill the empty spaces with spaces.
//@param num:    Given number needs to be converted.
//@param length: If the length of the result string is shorter than the given length, align it to the given length.
//@param *dest:  Designated pointer to store the string.
static void rightAlignNum(uint32_t num, uint32_t length, char *dest)
{
    char str[12];
    sprintf(str, "%d", num);
    int difference = length - strlen(str);
    //size <= 10, align right:
    if (difference > 0) {
        char buffer[12];
        strcpy(&buffer[difference], str);
        for (int i = 0; i < difference; i++) {
            buffer[i] = ' ';
        }
        strcpy(str, buffer);
    }
    strcpy(dest, str);
}

void printState(ARMmachine *machine)
{
    //Print registers:
    printf("Registers:\n");
    for (int i = 0; i < 10; i++) {
        char numStr[12];
        rightAlignNum(machine -> reg[i], 10, numStr);
        printf("$%d  : %s (0x%08x)\n", i, numStr, machine -> reg[i]);
    }
    for (int i = 10; i < 13; i++) {
        char numStr[12];
        rightAlignNum(machine -> reg[i], 10, numStr);
        printf("$%d  :%s (0x%08x)\n", i, numStr, machine -> reg[i]);
    }
    char pcStr[12], cpsrStr[12];
    rightAlignNum(*(machine -> pc) + 8, 10, pcStr);
    rightAlignNum(*(machine -> cpsr), 10, cpsrStr);
    printf("PC  : %s (0x%08x)\n", pcStr, *(machine -> pc) + 8);
    printf("CPSR: %s (0x%08x)\n", cpsrStr, *(machine -> cpsr));
    printf("Non-zero memory:\n");
    for (int i = 0; i < MEMORY_SIZE; i += 4) {
        uint32_t memValue = get32bitValue(i, machine);
        if (memValue != 0) {
            printf("0x%08x: 0x%08x\n", i, memValue);
        }
    }
}

bool readBinaryFile(char *filename, ARMmachine *machine)
{
    FILE *fp = fopen(filename, "rb");
    if (fp == NULL) {
        perror("open failed!");
        return false;
    }
    fseek(fp, 0L, SEEK_END);
    long fileSize = ftell(fp);
    fseek(fp, 0L, SEEK_SET);
    for (long i = 0; i < fileSize; i += sizeof(char)) {
        fread(machine -> memory + i, sizeof(char), 1, fp);
    }
    free(fp);
    return true;
}
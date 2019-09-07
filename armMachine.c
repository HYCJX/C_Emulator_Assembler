#include "armMachine.h"

/*
PC EXPLAINATION:
Instead of using a pipeline, we used a 'pseudo-pipeline'. Once the instruction is fetched, is it translated and executed.
Thus the value of PC is always the address of the current instrcution being executed.
After executing the instruction, the PC is updated.
Therefore the value of PC in this emulator is not equal to the theoretical value of PC (as per the spec) (and in fact, 8-bit behind).
When the last instruction is executed, the theorectial value of pc is 8-bit ahead of the actual value of pc.
After executing the last instruction, the pc is not updated anymore.
Hence when the state is printed, the value of pc should be incremented by 8-bit.
*/

bool evalInstruction(uint32_t instruction, ARMmachine *machine)
{
    if (instruction == 0) return true;
    bool isCondValid = evalCond(instruction, machine);
    if (isCondValid) {
        if (isBitsMatch(instruction, 22, 27, "000000") && isBitsMatch(instruction, 4, 7, "1001")) {
            execMul(instruction, machine);
        } else if (isBitsMatch(instruction, 26, 27, "00")) {
            execDP(instruction, machine);
        } else if (isBitsMatch(instruction, 26, 27, "01") && isBitsMatch(instruction, 21, 22, "00")) {
            execSDT(instruction, machine);
        } else if (isBitsMatch(instruction, 24, 27, "1010")) {
            execBranch(instruction, machine);
        } else {
            perror("Unknown instruction type.");
        }
    }
    *(machine -> pc) += 4; //Update pc after executing each instruction.
    return false;
}

void freeMachine(ARMmachine *machine)
{
    free(machine -> reg);
    free(machine -> memory);
    free(machine);
}

void initializeAll(ARMmachine *machine)
{
    machine -> reg = (uint32_t *) malloc(REGISTER_SIZE * sizeof(uint32_t));
    machine -> memory = (unsigned char *) malloc(MEMORY_SIZE * sizeof(unsigned char));
    machine -> pc = &machine -> reg[15];
    machine -> cpsr = &machine -> reg[16];
    for (int i = 0; i < REGISTER_SIZE; i++) {
        machine -> reg[i] = 0;
    }
    for (int i = 0; i < MEMORY_SIZE; i++) {
        machine -> memory[i] = 0;
    }
}
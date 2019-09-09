#include <stdlib.h>
#include<stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <math.h>
#include <string.h>
#include "armMachine.h"
#include "emulateIO.h"

int main(int argc, char **argv) {
    ARMmachine *machine = (ARMmachine *) malloc(sizeof(ARMmachine));
    initializeAll(machine);
    if (argc != 2 || !readBinaryFile(argv[1], machine)) {
        freeMachine(machine);
        return EXIT_FAILURE;
    }
    bool terminate = false;
    while (!terminate) {
        uint32_t instruction = get32bitIns(*(machine -> pc), machine);
        terminate = evalInstruction(instruction, machine);
    }
    printState(machine);
    freeMachine(machine);
    return EXIT_SUCCESS;
}
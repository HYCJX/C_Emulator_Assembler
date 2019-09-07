#ifndef _ARM_MACHINE_
#define _ARM_MACHINE_

#include <string.h>
#include "32bitProcess.h"
#include "armMachineExec.h"

//Evaluate and execute the given instruction.
//Return true if the program terminates after executing this piece of instruction.
//Return false otherwise.
//@param instruction: Given 32-bit instruction.
//@param *machine:    Given ARM machine.
bool evalInstruction(uint32_t instruction, ARMmachine *machine);

//Free the ARM machine.
//@param *machine: Given ARM machine.
void freeMachine(ARMmachine *machine);

//Initialize ARM registers and memories.
//@param *machine: Given ARM machine.
void initializeAll(ARMmachine *machine);

#endif
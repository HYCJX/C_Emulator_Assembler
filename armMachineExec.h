#ifndef _ARM_MACHINE_EXEC_
#define _ARM_MACHINE_EXEC_

#include "32bitProcess.h"
#include "armMachineHelper.h"

/***** Instruction Execution Functions *****/

bool execDP(uint32_t instruction, ARMmachine *machine);

bool execMul(uint32_t instruction, ARMmachine *machine);

bool execSDT(uint32_t instruction, ARMmachine *machine);

bool execBranch(uint32_t instruction, ARMmachine *machine);

/***** Helper Evaluation Functions *****/

//Evaluate the boolean value of the Cond part of the given 32-bit instruction.
//@param instruction: Given 32-bit instruction.
//@param *machine:    Given ARM machine.
bool evalCond(uint32_t instruction, ARMmachine *machine);

//Evaluate 12-bit operand2.
//If the given boolean is set, then operand 2 is an immediate.
//If the given boolean is not set, then operand 2 is a shift register.
//Also updates the value stored in the given bool pointer of carry.
//@param operand:  Given 12-bit operand.
//@param I:        If I is set then immediate, else shift reg.
//@param *carry:   Bool pointer for storing carry of any shift.
//@param *machine: Given ARM machine.
uint32_t evalOperand(uint32_t operand, bool I, bool *carry, ARMmachine *machine);

#endif
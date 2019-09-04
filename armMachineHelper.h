#ifndef _ARM_MACHINE_HELPER_
#define _ARM_MACHINE_HELPER_

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include "32bitProcess.h"
#include "armMachineStructure.h"

//Return the corresponding flag bit in CPSR register.
//@param flag:     Required flag bit's name as character.
//@param *machine: Given Arm mechine.
//Pre: flag = ANY('N', 'Z', 'C', 'V').
bool getCpsrBit(char flag, ARMmachine *machine);

//Store the given 32-bit instruction/number to the given memory adress.
//Given error message and return false if the memory is out of bound.
//Return true if the execution is successful.
//@param content: Given content to be stored into the memory.
//@param memAdd:  Given memory address.
//@param *machie: Given ARM machine.
bool store32BitToMem(uint32_t content, uint32_t memAdd, ARMmachine * machine);

//Return the stored 32-bit instruction from the given memory address.
//@param memAdd:   Given memory address.
//@param *machine: Given ARM machine.
uint32_t get32bitIns(uint32_t memAdd, ARMmachine *machine);

//Return the stored 32-bit number from the given memory address.
//@param memAdd:   Given memory adress.
//@param *machine: Given ARM machine.
uint32_t get32bitValue(uint32_t memAdd, ARMmachine *machine);

//Set the corresponding flag bit in CPSR register as the given bit.
//@param flag:     Requried flag bit's name as character.
//@param givenBit: Bit replaces the target bit.
//@param *machine: Given ARM machine.
//Pre: flag = ANY('N', 'Z', 'C', 'V').
void setCpsrBit(char flag, bool givenBit, ARMmachine *machine);

#endif
#ifndef _EMULATE_IO_
#define _EMULATE_IO_

#include <string.h>
#include "32bitProcess.h"
#include "armMachineStructure.h"
#include "armMachineHelper.h"

//Print the current state of ARM machine to the command line.
//@param *machine: Given ARM machine.
void printState(ARMmachine *machine);

//Read binary file from a specific binary file with given filename and load its instructions into the memory.
//Return false if the file does not exist.
//@param *filename: String representing the file name.
bool readBinaryFile(char *filename, ARMmachine *machine);

#endif
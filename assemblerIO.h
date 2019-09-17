#ifndef _ASSEMBLER_IO_
#define _ASSEMBLER_IO_

#include <ctype.h>
#include <wctype.h>
#include "strIntList.h"

/***** Input & Output *****/

//First pass of the two-pass.
//Read the content from the file and store the code or labels.
//Return true if executed successfully.
//@param *filename: Given filename.
//@param *scList:   Given source code list.
//@param *lblist:   Given label list.
bool readFile(const char *filename, StrIntList *scList, StrIntList *lblist, uint32_t *finalAdd);

//Create a binary file of the given name and write the parsed instructions into the binary file.
//Return true if it succeeds to create a new file. Return false otherwise.
bool writeFile(const char *filename, const uint32_t instructions[], const int length);

#endif
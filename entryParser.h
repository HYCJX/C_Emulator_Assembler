#ifndef _ENTRY_PARSER_
#define _ENTRY_PARSER_

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "entryParserBody.h"

//Main entry of the code parser which returns the parsed binary code of the given tokens.
//If there are multiple outputs (other than the code itself), they will be stored directly into the output.
//@param tokens:   Given tokenized instruction.
//@param memAdd:   Given memory address of the instruction.
//@param constAdd: Index where the extra output should be stored inside the output.
//@param output:   Given pointer to the output array.
//@param lbList:   Given list which stores all the labels and their corresponding address.
//@return:         The given instruction code in binary form.
uint32_t evalTypeAndParse(char **tokens, uint32_t memAdd, uint32_t *constAdd, uint32_t output[], StrIntList *lbList);

//Tokenize the given string by space, comma or \n.
//Return the result string array.
//The given string itself will NOT be modified.
//@param *line: given string.
char **tokenize(char *line);

#endif
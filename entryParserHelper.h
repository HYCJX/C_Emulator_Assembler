#ifndef _ENTRY_PARSER_HELPER_
#define _Entry_PARSER_HELPER_

#include <stdbool.h>
#include <stdint.h>
#include <ctype.h>
#include <string.h>
#include "32bitProcess.h"
#include "strIntList.h"

#define COND_ALWAYS (0b1110)
#define ERROR_CODE (0xffffffff)
#define PC_VALUE (0xf)
#define DEFAULT_STRLEN (0xff)
#define DEFAULT_STRLEN_SHORT (16)

uint32_t evalEpression(char *exp, bool *U);

//Input a 32-bit operand2 and convert it to 12 bits. (op2 is an expression.)
uint32_t getOp2Numeric(uint32_t operand2);

//Get the shift code from the given shift type.
////@param *shiftType: Given shift type as a string.
uint32_t getShiftCode(char *shiftType);

//Evalute the operand2.
//@param *tokensFrag: Given fragment of tokens which represents the operand2.
//@param *I:          GIven boolean pointer which stores the updates Immediate value.
uint32_t parseOperand2(char **tokensFrag, bool *I);

//Evaluate the shift.
//@param *tokensFrag: Given fragment of tokens which represents the shift.
//@param *Rn:         Given boolean pointer which stores the updated Rn value.
//@param *offset:     Given boolean pointer which stores the updated offset value.
//@param *U:          Given boolean pointer which stores the updated U value.
void parseShift(char *tokensFrag, uint32_t *Rn, uint32_t *offset, bool *U);

#endif
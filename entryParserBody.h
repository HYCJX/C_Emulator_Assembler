#ifndef ENTRY_PARSER_BODY_
#define ENTRY_PARSER_BODY_

#include "entryParserHelper.h"

/***** Parse Different Instructions *****/

uint32_t parseBranch(char *operand, char **tokens, StrIntList *list, uint32_t pc);

uint32_t parseDP(char *opcode, char **tokens);

uint32_t parseMul(char *operand, char **tokens);

uint32_t parseSDT(char *opcode, char **tokens, uint32_t memAdd, uint32_t *target, uint32_t output[]);

uint32_t parseSpecial(char *opcode, char **tokens);

#endif
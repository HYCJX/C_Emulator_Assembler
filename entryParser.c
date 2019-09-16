#include "entryParser.h"

uint32_t evalTypeAndParse(char **tokens, uint32_t memAdd, uint32_t *constAdd, uint32_t output[], StrIntList *lbList) {
    char *opcode = tokens[0];
    if (opcode[0] == 'b') {
        return parseBranch(opcode, tokens, lbList, memAdd);
    }
    if (strcmp(opcode, "add") == 0
        || strcmp(opcode, "sub") == 0
        || strcmp(opcode, "rsb") == 0
        || strcmp(opcode, "and") == 0
        || strcmp(opcode, "eor") == 0
        || strcmp(opcode, "orr") == 0
        || strcmp(opcode, "mov") == 0
        || strcmp(opcode, "tst") == 0
        || strcmp(opcode, "teq") == 0
        || strcmp(opcode, "cmp") == 0) {
        return parseDP(opcode, tokens);
    }
    if (strcmp(opcode, "mul") == 0
        || strcmp(opcode, "mla") == 0) {
        return parseMul(opcode, tokens);
    }
    if (strcmp(opcode, "ldr") == 0
        || strcmp(opcode, "str") == 0) {
        return parseSDT(opcode, tokens, memAdd, constAdd, output);
    }
    
    if (strcmp(opcode, "lsl") == 0
        || strcmp(opcode, "andeq") == 0) {
        return parseSpecial(opcode, tokens);
    }
    return ERROR_CODE;
}

char** tokenize(char *line) {
  char *rest = (char *) malloc(strlen(line) + 1);
  char *anchor = rest;
  strcpy(rest, line);
  char **result = (char **) malloc(sizeof(char *));
  if (result) {
    size_t n = 1;
    char *token = strtok_r(rest, " ,\n", &rest);
    while (token) {
      char **tmp = realloc(result, (++n) * sizeof(char *));
      if (tmp == NULL) break;
      result = tmp;
      result[n - 2] = malloc(strlen(token) + 1);
      if (result[n - 2] != NULL) {
        strcpy(result[n - 2], token);
      }
      token = strtok_r(NULL, " ,\n", &rest);
    }
    result[n - 1] = NULL;
  }
  free(anchor);
  return result;
} 

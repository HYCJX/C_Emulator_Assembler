#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <ctype.h>
#include <math.h>
#include <string.h>
#include "assemblerIO.h"
#include "entryParser.h"
#include "strIntList.h"

#define MAX_MEMORY (65536)

int main(int argc, char **argv) {
    if (argc != 3) {
        return EXIT_FAILURE;
    }
    char *inputFile = argv[1];
    char *outputFile = argv[2];
    StrIntList *scList = newStrIntList();
    StrIntList *lbList = newStrIntList();
    uint32_t immAdd = 0;
    if (!readFile(inputFile, scList, lbList, &immAdd)) {
        printf("Error here.\n");
        freeAllEntries(scList);
        freeAllEntries(lbList);
        free(scList);
        free(lbList);
        return EXIT_FAILURE;
    }
    //Read from sclIst, tokenize the given instruction, then pass it to parser.
    uint32_t output[MAX_MEMORY / 4];
    uint32_t length = 0;
    for (ListEntry *node = scList -> head; node; node = node -> next, length += 4) {
        uint32_t memAdd = node -> address;
        //Tokenize given instruction:
        char **tokens = tokenize(node -> str);
        //Parse tokens:
        uint32_t instruction = evalTypeAndParse(tokens, memAdd, &immAdd, output, lbList);
        //Fetch instruction to output array.
        output[length / 4] = instruction;
        //Free tokens:
        for (int i = 0; tokens[i]; i++) {
            free(tokens[i]);
        }
        free(tokens);
    }
    writeFile(outputFile, output, immAdd / 4);
    freeAllEntries(scList);
    freeAllEntries(lbList);
    free(scList);
    free(lbList);
    return EXIT_SUCCESS;
}
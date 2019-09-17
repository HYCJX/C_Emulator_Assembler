#include "assemblerIO.h"

//Return true if the given line is a label, false otherwise.
//@param *line, Given line of code.
static bool isLabel(const char *line)
{
    int i;
    for (i = 0; line[i] != ':'; i++) {
        if (line[i] == '\0') return false;
    }
    for (i++; line[i] != '\0'; i++) {
        if (!iswspace(line[i])) return false;
    }
    return true;
}

//Return true if the given line is empty, false otherwise.
//@param: *line: Given line of code.
static bool isEmptyLine(const char *line)
{
    for (int i = 0; line[i]; i++) {
        if (!iswspace(line[i])) return false;
    }
    return true;
}

bool readFile(const char *filename, StrIntList *scList, StrIntList *lbList,uint32_t *finalAdd)
{
    FILE *fp = fopen(filename, "r");
    if (fp == NULL) {
        perror("Open failed!");
        return false;
    }
    char line[512];
    uint32_t memAdd = 0;
    while (fgets(line, 512, fp)) {
        if (!isEmptyLine(line)) {
            if (isLabel(line)) {
                addEntry(lbList, strtok(line, ":"), memAdd);
            } else {
                addEntry(scList, line, memAdd);
                memAdd += 4;
            }
        }
    }
    fclose(fp);
    *finalAdd = memAdd;
    return true;
}

bool writeFile(const char *filename, const uint32_t instructions[], const int length)
{
    FILE *fp = fopen(filename, "wb");
    if (fp == NULL) {
        perror("Fail to create a new file!");
        return false;
    }
    for (int i = 0; i < length; i++) {
        fwrite(&instructions[i], sizeof(uint32_t), 1, fp);
    }
    fclose(fp);
    return true;
}
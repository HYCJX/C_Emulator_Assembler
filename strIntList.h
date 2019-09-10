#ifndef _LABEL_LIST_
#define _LABEL_LIST_

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>

struct ListEntry {
    char *str;
    uint32_t address;
    struct ListEntry *next;
};

struct StrIntList {
    struct ListEntry *head;
    struct ListEntry *tail;
    uint32_t size;
};

typedef struct ListEntry ListEntry;
typedef struct StrIntList StrIntList;

//Create a new Label list and allocate memory for it.
//By default, the head and tail of the Label list is NULL, the size is 0.
StrIntList* newStrIntList(void);

//Build a new entry from the given label and address and add it to the end of the linked list.
//Return false if the addition fails, true if it succeeds (always returns true for now).
//@param *label:  given String of label.
//@param address: given address of the label.
bool addEntry(StrIntList *list, char *label, uint32_t address);

//Return the corresponding address of the given label.
//Gives error message and returns 0 if the label was not found.
//@param *label:given string of label.
uint32_t getAddress(StrIntList *list, char *label);

//Free all entries in the linked list.
void freeAllEntries(StrIntList *list);

#endif
#include "strIntList.h"

/*
This module provides a data structure for storing strings and their corresponding addresses into the memory.
The data structure is a 'pseudo-linkedlist, chich cannot remove any node (Except from freeing them).
Also, this impementation only allows adding entries to the end of the list.
Use function newStrIntList() to create a new strlist.
*/

StrIntList *newStrIntList(void)
{
    StrIntList *list = (StrIntList *) malloc(sizeof(StrIntList));
    list -> head = NULL;
    list -> tail = NULL;
    list -> size = 0;
    return list;
}

bool addEntry(StrIntList *list, char *label, uint32_t address)
{
    ListEntry *current = (ListEntry *) malloc(sizeof(current));
    current -> str = malloc(strlen(label) + 1);
    strcpy(current -> str, label);
    current -> address = address;
    current -> next = NULL;
    if (list -> size++ == 0) {
        list -> head = current;
        list -> tail = current;
    } else {
        list -> tail -> next = current;
        list -> tail = current;
    }
    return true;
}

uint32_t getAddress(StrIntList *list, char *label)
{
    ListEntry *current = list -> head;
    while (current != NULL) {
        if (strcmp(label, current -> str) == 0) {
            return current -> address;
        }
        current = current -> next;
    }
    perrpr("str not found!");
    return 0;
}

void freeAllEntries(StrIntList * list)
{
    ListEntry *current = list -> head;
    while (current != NULL) {
        ListEntry * temp = current;
        current = current -> next;
        free(temp -> str);
        free(temp);
        list -> size--;
    }
}
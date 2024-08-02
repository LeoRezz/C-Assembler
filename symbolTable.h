#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_LINE 82
#define MAX_LABEL_LENGTH 31

typedef enum {
    SYMBOL_DATA,
    SYMBOL_CODE,
    SYMBOL_ENTRY,
    SYMBOL_EXTERNAL
} SymbolType;

typedef struct {
    char name[MAX_LABEL_LENGTH];
    int value;
    SymbolType type;
} symbol;

void initSymbolTable();
int addSymbol(char *name, int *value, SymbolType type);
symbol *findSymbol(char *name);
void printSymbolTable();
void freeSymbolTable();
#endif
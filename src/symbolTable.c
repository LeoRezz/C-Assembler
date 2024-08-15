#include "symbolTable.h"
#define MAX_LABEL_LENGTH 31
#define INITIAL_SYMBOL_TABLE_SIZE 20

/* Global state, Instruction and Data count*/
extern int IC;
extern int DC;
extern int current_line;

symbol *symbolTable;
int symbolCount;
int symbolCapacity;

/*Initialize the symbol table */
void initSymbolTable() {
    symbolTable = calloc(INITIAL_SYMBOL_TABLE_SIZE , sizeof(symbol));
    if (symbolTable == NULL) { /* Handle allocation error */
        printf("Failed to allocate memory for SymbolTable\n");
        exit(1);
    }
    symbolCount = 0;
    symbolCapacity = INITIAL_SYMBOL_TABLE_SIZE;
}

/*Add a symbol to the table */
int addSymbol(char *name, int *value, SymbolType type) {
    int new_capacity;
    if (symbolCount >= symbolCapacity) {
        new_capacity = symbolCapacity * 2;
        symbol *new_table = realloc(symbolTable, new_capacity * sizeof(symbol));
        if (new_table == NULL) {
            /* Handle reallocation error*/
            printf("Failed to reallocate memory for symbolTable\n");
            return 0;
        }
        symbolTable = new_table;
        symbolCapacity = new_capacity;
    }

    printf("Adding symbol: %s, value: %d, type: %d\n", name, *value, type);
    strncpy(symbolTable[symbolCount].name, name, MAX_LABEL_LENGTH);
    symbolTable[symbolCount].name[MAX_LABEL_LENGTH] = '\0'; /* Ensure null-termination */
    printf("Symbol name copied: %s\n", symbolTable[symbolCount].name);
    symbolTable[symbolCount].value = *value;
    printf("Symbol value set: %d\n", symbolTable[symbolCount].value);
    symbolTable[symbolCount].type = type;
    printf("Symbol type set: %d\n", symbolTable[symbolCount].type);

    symbolCount++;
    return 1;
}

/* Look up a symbol in the table */
symbol *findSymbol(char *name) {
    int i;
    for (i = 0; i < symbolCount; i++) {
        if (strcmp(symbolTable[i].name, name) == 0) {
            return &symbolTable[i];
        }
    }
    return NULL;
}

void updateDataSymbols(int IC) {
    int i;
    for (i = 0; i < symbolCount; i++) {
        if (symbolTable[i].type == SYMBOL_DATA) {
            symbolTable[i].value += IC;
        }
    }
}

void printSymbolTable() {
    int i;
    printf("\nSymbol Table:\n");
    printf("%-32s %-10s %-10s\n", "Name", "Value", "Type");
    printf("-------------------------------- ---------- ----------\n");
    for (i = 0; i < symbolCount; i++) {
        printf("%-32s %-10d ", symbolTable[i].name, symbolTable[i].value);
        switch (symbolTable[i].type) {
        case SYMBOL_CODE:
            printf("CODE\n");
            break;
        case SYMBOL_DATA:
            printf("DATA\n");
            break;
        case SYMBOL_EXTERNAL:
            printf("EXTERNAL\n");
            break;
        default:
            printf("UNKNOWN\n");
        }
    }
    printf("\nTotal symbols: %d\n", symbolCount);
}

/* Clean up the symbol table */
void freeSymbolTable() {
    free(symbolTable);
    symbolTable = NULL;
    symbolCount = 0;
    symbolCapacity = 0;
}
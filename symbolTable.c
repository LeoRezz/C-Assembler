#include "symbolTable.h"
#define MAX_LABEL_LENGTH 31
#define INITIAL_SYMBOL_TABLE_SIZE 20

/* Global state, Instruction and Data count*/
extern int IC;
extern int DC;
extern int current_line;

symbol *symbolTable = NULL;
int symbolCount = 0;
int symbolCapacity = 0;

/*Initialize the symbol table */
void initSymbolTable() {
    symbolTable = malloc(INITIAL_SYMBOL_TABLE_SIZE * sizeof(symbol));
    if (symbolTable == NULL) { /* Handle allocation error */
        printf("Failed to allocate memory for SymbolTable\n");
        exit(1);
    }
    symbolCount = 0;
    symbolCapacity = INITIAL_SYMBOL_TABLE_SIZE;
}

/*Add a symbol to the table */
int addSymbol(char *name, int *value, SymbolType type) {
    if (symbolCount >= symbolCapacity) {
        int new_capacity = symbolCapacity * 2;
        symbol *new_table = realloc(symbolTable, new_capacity * sizeof(symbol));
        if (new_table == NULL) {
            /* Handle reallocation error*/
            printf("Failed to reallocate memory for symbolTable\n");
            return 0;
        }
        symbolTable = new_table;
        symbolCapacity = new_capacity;
    }

    strncpy(symbolTable[symbolCount].name, name, MAX_LABEL_LENGTH);
    symbolTable[symbolCount].name[MAX_LABEL_LENGTH] = '\0'; /* Ensure null-termination */
    symbolTable[symbolCount].value = *value;
    symbolTable[symbolCount].type = type;

    symbolCount++;
    return 1;
}

/* Look up a symbol in the table */
symbol *findSymbol(char *name) {
    for (int i = 0; i < symbolCount; i++) {
        if (strcmp(symbolTable[i].name, name) == 0) {
            return &symbolTable[i];
        }
    }
    return NULL;
}

void updateDataSymbols(int IC) {
    for (int i = 0; i < symbolCount; i++) {
        if (symbolTable[i].type == SYMBOL_DATA) {
            symbolTable[i].value += IC;
        }
    }
}

void printSymbolTable() {
    printf("\nSymbol Table:\n");
    printf("%-32s %-10s %-10s\n", "Name", "Value", "Type");
    printf("-------------------------------- ---------- ----------\n");
    for (int i = 0; i < symbolCount; i++) {
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
#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define INITIAL_ADDRESS 100
#define MAX_SYMBOL_LENGTH 31

typedef enum {
    SYMBOL_CODE,
    SYMBOL_DATA,
    SYMBOL_ENTRY,
    SYMBOL_EXTERNAL
} SymbolType;

typedef struct {
    char name[MAX_SYMBOL_LENGTH];
    int value; /* ADDRESS */
    SymbolType type;
} symbol;

void init_symbol_table(void);
int add_symbol(char *name, int value, SymbolType type);
symbol *find_symbol(char *name);
int resolve_entrys();
symbol *find_entry_refrence(char *name);
symbol* get_symbol_table();
int get_symbol_count();
void update_data_symbols(int IC);
void print_symbol_table(void);
void free_symbol_table(void);
#endif /* SYMBOL_TABLE_H */
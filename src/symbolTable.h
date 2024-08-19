#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

/* Symbol table to hold all the symbols declared in the source file */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_SYMBOL_LENGTH 31

/* Types of symbols of our assembly language */
typedef enum {
    SYMBOL_CODE,
    SYMBOL_DATA,
    SYMBOL_ENTRY,
    SYMBOL_EXTERNAL,
    SYMBOL_MACRO
} SymbolType;

/* Symbol struct to hold the symbols  */
typedef struct {
    char name[MAX_SYMBOL_LENGTH]; /* Symbol identifier name */
    int value; /* ADDRESS */
    SymbolType type; /* Symbol type */
} symbol;

/* Function prototypes */

/* Initilizes the symbol tabel, allocting space in memory */
void init_symbol_table(void);
/* Adds symbols to the table */
int add_symbol(char *name, int value, SymbolType type);
/* Finds a symbol inside the table */
symbol *find_symbol(char *name);
/* finds entry symbol for resolving address */
symbol *find_entry_refrence(char *name);
/* Resolves entry address refrences in the source code */
int resolve_entrys();
/* Getter for the symbol table */
symbol* get_symbol_table();
/* Getter for the total symbol count */
int get_symbol_count();
/* Updates the data symbols after the first pass */
void update_data_symbols(int IC);
/* Frees the symbol table after the file assembling*/
void free_symbol_table(void);


#endif 
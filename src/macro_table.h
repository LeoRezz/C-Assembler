#ifndef MACRO_TABLE_H
#define MACRO_TABLE_H

#include "util.h"

typedef struct Macro {
    char name[MAX_MACRO_NAME];
    char **body;
    int line_count;
    int capacity;
} Macro;

typedef struct MacroTable {
    Macro *macros;
    int count;
    int capacity;
} MacroTable;

MacroTable *init_macro_table(void);
void free_macro_table(MacroTable *table);
int add_macro(MacroTable *table, const char *name);
int add_macro_line(MacroTable *table, int macro_index, const char *line);
Macro *get_macro(MacroTable *table, const char *name);
void print_macro_table(MacroTable *table);

#endif 
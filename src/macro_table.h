#ifndef MACRO_TABLE_H
#define MACRO_TABLE_H

/* Stores macros in a table for later expansion */

#include "util.h"

/* Macro struct for storing the macro name and it's content */
typedef struct Macro {
    char name[MAX_MACRO_NAME]; /* Name of the macro */
    char **body; /* Content of the macro */
    int line_count; /* Number of lines in the macro */
    int capacity; /* Capacity of the macro */
} Macro;

/* Macro table for storing the macro names in an array */
typedef struct MacroTable {
    Macro *macros; /* Array of macro struct pointers */
    int count; /* Number of macros */
    int capacity; /* Capacity of the table */
} MacroTable;

/* Function prototypes */
MacroTable *init_macro_table(void);
void free_macro_table(MacroTable *table);
int add_macro(MacroTable *table, const char *name);
int add_macro_line(MacroTable *table, int macro_index, const char *line);
Macro *get_macro(MacroTable *table, const char *name);


#endif 
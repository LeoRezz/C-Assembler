#include "macro_table.h"

#define INITIAL_MACRO_CAPACITY 10
#define INITIAL_MACRO_LINES_CAPACITY 10

MacroTable *init_macro_table(void) {
    MacroTable *table = malloc(sizeof(MacroTable));
    if (!table)
        return NULL;

    table->macros = malloc(INITIAL_MACRO_CAPACITY * sizeof(Macro));
    if (!table->macros) {
        free(table);
        return NULL;
    }

    table->count = 0;
    table->capacity = INITIAL_MACRO_CAPACITY;
    return table;
}


void free_macro_table(MacroTable *table) {
    int i, j;
    if (!table)
        return;

    for (i = 0; i < table->count; i++) {
        for (j = 0; j < table->macros[i].line_count; j++) {
            free(table->macros[i].body[j]);
        }
        free(table->macros[i].body);
    }
    free(table->macros);
    free(table);
}

static int grow_macro_table(MacroTable *table) {
    int new_capacity = table->capacity * 2;
    Macro *new_macros = realloc(table->macros, new_capacity * sizeof(Macro));
    if (!new_macros)
        return 0;

    table->macros = new_macros;
    table->capacity = new_capacity;
    return 1;
}

int add_macro(MacroTable *table, const char *name) {
    if (table->count >= table->capacity) {
        if (!grow_macro_table(table))
            return -1;
    }

    strncpy(table->macros[table->count].name, name, MAX_MACRO_NAME - 1);
    table->macros[table->count].name[MAX_MACRO_NAME - 1] = '\0';

    table->macros[table->count].body = malloc(INITIAL_MACRO_LINES_CAPACITY * sizeof(char *));
    if (!table->macros[table->count].body)
        return -1;

    table->macros[table->count].line_count = 0;
    table->macros[table->count].capacity = INITIAL_MACRO_LINES_CAPACITY;

    return table->count++;
}

static int grow_macro_lines(Macro *m) {
    int new_capacity = m->capacity * 2;
    char **new_body = realloc(m->body, new_capacity * sizeof(char *));
    if (!new_body)
        return 0;

    m->body = new_body;
    m->capacity = new_capacity;
    return 1;
}

int add_macro_line(MacroTable *table, int macro_index, const char *line) {
    Macro *m = &table->macros[macro_index];

    if (m->line_count >= m->capacity) {
        if (!grow_macro_lines(m))
            return 0;
    }

    m->body[m->line_count] = my_strdup(line);
    if (!m->body[m->line_count])
        return 0;

    m->line_count++;
    return 1;
}

Macro *get_macro(MacroTable *table, const char *name) {
    int i;
    for (i = 0; i < table->count; i++) {
        if (strcmp(table->macros[i].name, name) == 0) {
            return &table->macros[i];
        }
    }
    return NULL;
}

void print_macro_table(MacroTable *table) {
    int i, j;
    for (i = 0; i < table->count; i++) {
        printf("Macro: %s\n", table->macros[i].name);
        for (j = 0; j < table->macros[i].line_count; j++) {
            printf("  Line %d: %s\n", j + 1, table->macros[i].body[j]);
        }
        printf("\n");
    }
}

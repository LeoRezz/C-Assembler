#include "preprocessor.h"
#include "symbolTable.h"
#include "util.h"
#include <stdlib.h>
#include <string.h>

#define INITIAL_MACRO_CAPACITY 10
#define INITIAL_MACRO_LINES_CAPACITY 10

/* Global State mode */
typedef enum { NORMAL,
               DEFINITION,
               EXPANSION } Mode;

typedef struct Macro {
    char name[MAX_MACRO_NAME + 1];
    char **lines;
    int line_count;
    int capacity;
} Macro;

typedef struct MacroTable {
    Macro *macros;
    int count;
    int capacity;
} MacroTable;

/* Global variables */
static MacroTable *macro_table;
static Mode current_mode = NORMAL;
static int current_macro_index = -1;

/* Function prototypes */
static MacroTable* init_macro_table(void);
static void free_macro_table(MacroTable *table);
static int add_macro(const char *name);
static int find_macro(const char *name);
static void add_line_to_macro(int macro_index, const char *line);
static int is_valid_macro_name(const char *name);
void trim_trailing_whitespace(char *line);
int is_valid_name(char *line);
int preprocess(const char *input_filename, const char *am_filename) {
    FILE *input_file, *output_file;
    char line[MAX_LINE];

    input_file = fopen(input_filename, "r");
    if (!input_file) {
        printf("Failed to open input file: %s\n", input_filename);
        return 0;
    }

    output_file = fopen(am_filename, "w");
    if (!output_file) {
        printf("Failed to create intermediate output file: %s\n", am_filename);
        fclose(input_file);
        return 0;
    }

    macro_table = init_macro_table();
    if (!macro_table) {
        printf("Failed to initialize macro table\n");
        fclose(input_file);
        fclose(output_file);
        return 0;
    }

    while (fgets(line, MAX_LINE, input_file) != NULL) {
        if (strlen(line) >= MAX_LINE - 1) {
            printf("Error: Input line too long, max length is %d\n", MAX_LINE - 1);
            fclose(input_file);
            fclose(output_file);
            free_macro_table(macro_table);
            return 0;
        }

        trim_trailing_whitespace(line);
        switch (current_mode) {
            case NORMAL:
                handle_normal_mode(line, output_file);
                break;
            case DEFINITION:
                handle_definition_mode(line, macro_table);
                break;
            case EXPANSION:
                handle_expansion_mode(line, output_file);
                break;
        }
    }

    fclose(input_file);
    fclose(output_file);
    free_macro_table(macro_table);

    return 1;
}


static int add_macro(const char *name) {
    if (macro_table->count >= macro_table->capacity) {
        int new_capacity = macro_table->capacity * 2;
        Macro *new_macros = realloc(macro_table->macros, new_capacity * sizeof(Macro));
        if (!new_macros) return -1;
        macro_table->macros = new_macros;
        macro_table->capacity = new_capacity;
    }

    strncpy(macro_table->macros[macro_table->count].name, name, MAX_MACRO_NAME);
    macro_table->macros[macro_table->count].name[MAX_MACRO_NAME] = '\0';
    macro_table->macros[macro_table->count].lines = NULL;
    macro_table->macros[macro_table->count].line_count = 0;
    macro_table->macros[macro_table->count].capacity = 0;

    return macro_table->count++;
}

void handle_normal_mode(char *line, FILE *output_file) {
    char *p = skipSpaces(line);
    
    if (*p == '\0') {
        return; /* Skip empty lines */
    }

    if (strncmp(p, MACRO_KEYWORD, MACRO_KEYWORD_LENGTH) == 0) {
        p += MACRO_KEYWORD_LENGTH;
        p = skipSpaces(p);
        if (is_valid_macro_name(p)) {
            current_macro_index = add_macro(p);
            current_mode = DEFINITION;
        } else {
            printf("Error: Invalid macro name '%s'\n", p);
        }
        return;
    }

    current_macro_index = find_macro(p);
    if (current_macro_index != -1) {
        current_mode = EXPANSION;
        return;
    }

    fputs(line, output_file);
    fputc('\n', output_file);
}

void handle_definition_mode(char *line, Macro *macro_table) {
    char *p = skipSpaces(line);

    if (strncmp(p, ENDMACRO_KEYWORD, ENDMACRO_KEYWORD_LENGTH) == 0) {
        current_mode = NORMAL;
        return;
    }

    add_line_to_macro(current_macro_index, line);
}

void handle_expansion_mode(char *line, FILE *output_file) {
    int i;
    Macro *macro = &macro_table->macros[current_macro_index];

    for (i = 0; i < macro->line_count; i++) {
        fputs(macro->lines[i], output_file);
        fputc('\n', output_file);
    }

    current_mode = NORMAL;
    handle_normal_mode(line, output_file);
}

static void add_line_to_macro(int macro_index, const char *line) {
    Macro *macro = &macro_table->macros[macro_index];
    
    if (macro->line_count >= macro->capacity) {
        int new_capacity = macro->capacity == 0 ? INITIAL_MACRO_LINES_CAPACITY : macro->capacity * 2;
        char **new_lines = realloc(macro->lines, new_capacity * sizeof(char*));
        if (!new_lines) {
            printf("Failed to allocate memory for macro line\n");
            return;
        }
        macro->lines = new_lines;
        macro->capacity = new_capacity;
    }

    macro->lines[macro->line_count] = strdup(line);
    if (!macro->lines[macro->line_count]) {
        printf("Failed to allocate memory for macro line\n");
        return;
    }
    macro->line_count++;
}


static int find_macro(const char *name) {
    int i;
    for (i = 0; i < macro_table->count; i++) {
        if (strcmp(macro_table->macros[i].name, name) == 0) {
            return i;
        }
    }
    return -1;
}

void trim_trailing_whitespace(char *line) {
    int i = strlen(line) - 1;
    while (i >= 0 && isspace(line[i])) {
        line[i] = '\0';
        i--;
    }
}


int has_extra_chars(char *line) {
    char *p = line;
    while (*p != '\0' && *p != '\n') {
        if (!isalnum(*p) && *p != '_') {
            return 0; /* Invalid character found */
        }
        p++;
    }

    if (*p != '\0' && *p != '\n') {
        return 0; /* Trailing characters found */
    }

    return 1; /* No extra characters found */
}


static MacroTable* init_macro_table(void) {
    MacroTable *table = malloc(sizeof(MacroTable));
    if (!table) return NULL;

    table->macros = malloc(INITIAL_MACRO_CAPACITY * sizeof(Macro));
    if (!table->macros) {
        free(table);
        return NULL;
    }

    table->count = 0;
    table->capacity = INITIAL_MACRO_CAPACITY;
    return table;
}

static void free_macro_table(MacroTable *table) {
    int i, j;
    if (!table) return;

    for (i = 0; i < table->count; i++) {
        for (j = 0; j < table->macros[i].line_count; j++) {
            free(table->macros[i].lines[j]);
        }
        free(table->macros[i].lines);
    }
    free(table->macros);
    free(table);
}





/* Reserved words */
static const char *reserved_words[] = {
    "r0", "r1", "r2", "r3", "r4", "r5", "r6", "r7",
    "mov", "cmp", "add", "sub", "lea", "clr", "not",
    "inc", "dec", "jmp", "bne", "red", "prn", "jsr", "rts", "stop",
    ".data", ".string", ".entry", ".extern",
    NULL};

/* Recives a char pointer of the name declration after "macr " */
/* Checks for extra characters on the macro definition line */
/* Checks for Reserved words */

int is_valid_name(char *line) {
    int i;
    /* Check for extra characters first */
    if (has_extra_chars(line) == 0) {
        printf("Invalid macro name: '%s' extra characters found\n", line);
        return 0;
    }

    /* Check against reserved words */
    for (i = 0; i < NUM_OF_RESERVED; i++) {
        if (strncmp(line, reserved_words[i], strlen(reserved_words[i])) == 0) {
            printf("Invalid macro name: '%s' is a reserved word\n", line);
            return 0;
        }
    }
    return 1;
}
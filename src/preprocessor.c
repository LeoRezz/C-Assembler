#include "util.h"
#include "preprocessor.h"
#include "macro_table.h"
#include "symbolTable.h"

/* Global State mode */
typedef enum { NORMAL, DEFINITION, EXPANSION } Mode;
static Mode current_mode = NORMAL;
static MacroTable *macro_table;
extern int error_flag;

/* Function prototypes */
static int handle_normal_mode(char *line, FILE *output_file, int current_line);
static void handle_definition_mode(char *line, int macro_index);
static void handle_expansion_mode(char *line, FILE *output_file, int macro_index);
static int is_valid_macro_name(const char *name, int current_line);

int preprocess(const char *input_filename, const char *am_filename) {
    FILE *input_file, *output_file;
    char line[MAX_LINE];
    int current_line = 0;
    int macro_index = -1;

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

    init_symbol_table();
    macro_table = init_macro_table();
    if (!macro_table) {
        printf("Failed to initialize macro table\n");
        fclose(input_file);
        fclose(output_file);
        return 0;
    }

    while (fgets(line, MAX_LINE, input_file) != NULL) {
        current_line++;
        if (strlen(line) == MAX_LINE - 1 && line[MAX_LINE - 2] != '\n') {
            printf("Error: Line number %d is too long\n", current_line);
            error_flag = 1;
            continue;
        }
        trim_trailing_whitespace(line);

        switch (current_mode) {
            case NORMAL:
                macro_index = handle_normal_mode(line, output_file, current_line);
                break;
            case DEFINITION:
                handle_definition_mode(line, macro_index);
                break;
            case EXPANSION:
                handle_expansion_mode(line, output_file, macro_index);
                break;
        }
    }

    fclose(input_file);
    fclose(output_file);
    free_macro_table(macro_table);

    return (error_flag == 0);
}

static int handle_normal_mode(char *line, FILE *output_file, int current_line) {
    Macro *m;
    int macro_index;
    char *line_ptr = skipSpaces(line);
    
    if (*line_ptr == '\0' || *line_ptr == '\n' || *line_ptr == ';') {
        return -1; /* Skip empty lines and comments */
    }

    if (strncmp(line_ptr, MACRO_KEYWORD, MACRO_KEYWORD_LENGTH) == 0) {
        current_mode = DEFINITION;
        line_ptr += MACRO_KEYWORD_LENGTH;
        line_ptr = skipSpaces(line_ptr);
        
        if (!is_valid_macro_name(line_ptr, current_line)) {
            current_mode = NORMAL;
            return -1;
        }
        
        macro_index = add_macro(macro_table, line_ptr);
        if (macro_index == -1) {
            printf("Error: Failed to add macro '%s'\n", line_ptr);
            error_flag = 1;
            return -1;
        }
        return macro_index;
    }

    m = get_macro(macro_table, line_ptr);
    if (m) {
        current_mode = EXPANSION;
        return m - macro_table->macros;  /* Return the index of the macro */
    }

    fputs(line, output_file);
    fputc('\n', output_file);
    return -1;
}

static void handle_definition_mode(char *line, int macro_index) {
    char *p = skipSpaces(line);

    if (strncmp(p, ENDMACRO_KEYWORD, ENDMACRO_KEYWORD_LENGTH) == 0) {
        current_mode = NORMAL;
        return;
    }

    if (!add_macro_line(macro_table, macro_index, line)) {
        printf("Error: Failed to add line to macro\n");
        error_flag = 1;
    }
}

static void handle_expansion_mode(char *line, FILE *output_file, int macro_index) {
    int i;
    Macro *m = &macro_table->macros[macro_index];
    
    for (i = 0; i < m->line_count; i++) {
        fputs(m->body[i], output_file);
        fputc('\n', output_file);
    }
    current_mode = NORMAL;
    handle_normal_mode(line, output_file, 0);
}

static int is_valid_macro_name(const char *name, int current_line) {
    if (is_reserved_word(name)) {
        printf("Error at line %d: Invalid macro name: '%s' is a reserved word\n", current_line, name);
        error_flag = 1;
        return 0;
    }
    
    if (!isalpha(*name)) {
        printf("Error at line %d: Invalid macro name: '%s' must start with a letter\n", current_line, name);
        error_flag = 1;
        return 0;
    }
    
    while (*name) {
        if (!isalnum(*name) && *name != '_') {
            printf("Error at line %d: Invalid macro name: '%s' contains invalid character\n", current_line, name);
            error_flag = 1;
            return 0;
        }
        name++;
    }
    
    return 1;
}
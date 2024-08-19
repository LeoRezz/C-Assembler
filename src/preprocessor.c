#include "util.h"
#include "preprocessor.h"
#include "macro_table.h"
#include "symbolTable.h"
#include "tokenizer.h"
#define BUFFER_SIZE 1024
/* Preprocessor State */
typedef enum { NORMAL = -1, DEFINITION, EXPANSION } Mode;
static Mode current_mode = NORMAL;
static MacroTable *macro_table;

/* Function prototypes */
static int handle_normal_mode(char *line, FILE *output_file, int current_line, int token_count, Token *tokens, int *error_flag);
static void handle_definition_mode(char *line, int macro_index ,int token_count ,Token *tokens , int *error_flag);
static void handle_expansion_mode(char *line, FILE *output_file,int current_line,int token_count ,Token *tokens ,int macro_index , int *error_flag);
static int is_valid_macro_name(const char *name, int current_line, int *error_flag);

int preprocess(const char *input_filename, const char *am_filename) {
    FILE *input_file, *output_file;
    Token *tokens;
    const char *line_ptr;
    char line[BUFFER_SIZE];
    int is_preprocess;
    int current_line;
    int token_count;
    int macro_index;
    int error_flag;
    int len;

    macro_index = -1;
    error_flag = 0;
    current_line = 0;
    is_preprocess = 1;

    /* Open input and output files */
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

    /* Initialize symbol table and macro table */
    init_symbol_table();
    macro_table = init_macro_table();
    if (!macro_table) {
        printf("Failed to initialize macro table\n");
        fclose(input_file);
        fclose(output_file);
        return 0;
    }
    printf("Preprocessing %s\n", input_filename);
    while (fgets(line, BUFFER_SIZE, input_file) != NULL) {
        current_line++;
        len = strlen(line);
        if (len >= MAX_LINE) {
            printf("Error: line %d is too long (max length is %d)\n", current_line, MAX_LINE - 1);
            error_flag = 1;
            continue;
        }
        line[strcspn(line, "\n")] = 0;
        line_ptr = line;
        /* remove empty lines */
        while (isspace(*line_ptr) && *line_ptr != '\0') {
            line_ptr++;
        }
        if (*line_ptr == '\0' || *line_ptr == '\n') {
            continue;
        }
        if (line[0] == ';')
            continue;
        tokens = tokenize_line(line, &token_count, current_line, is_preprocess);
        if (tokens == NULL) {
          /*  printf("Line %d: '%s'\n\n", current_line, line);
            error_flag = 1; */
            continue;
        }

        if (strlen(line) == MAX_LINE - 1 && line[MAX_LINE - 2] != '\n') {
            printf("Error: Line number %d is too long\n", current_line);
            error_flag = 1;
            continue;
        }
        trim_trailing_whitespace(line);

        switch (current_mode) {
        case NORMAL:
            macro_index = handle_normal_mode(line, output_file, current_line, token_count, tokens, &error_flag);
            break;
        case DEFINITION:
            handle_definition_mode(line, macro_index, token_count, tokens, &error_flag);
            break;
        case EXPANSION:
            handle_expansion_mode(line, output_file, current_line, token_count, tokens, macro_index, &error_flag);
            break;
        }
        free(tokens);
    }

    fclose(input_file);
    fclose(output_file);
    free_macro_table(macro_table);

    return (error_flag == 0);
}

static int handle_normal_mode(char *line, FILE *output_file, int current_line, int token_count, Token *tokens , int *error_flag) {
    Macro *m;
    int macro_index;
    char *line_ptr = skipSpaces(line);
    if (token_count == 0) {
        return NORMAL; /* Skip empty lines and comments */
    }

    /* Searching for macro usage */
    m = get_macro(macro_table, tokens[0].value);
    if (m != NULL) {
        if(token_count > 1) {
            printf("Error at line %d: macro usage with extra arguments: '%s' contains invalid character\n", current_line, tokens[1].value);
            *error_flag = 1;
            return NORMAL;
        }
        current_mode = EXPANSION;
        return m - macro_table->macros; /* Return the index of the macro */
    } else if (tokens[0].type == ENDMACRO_KEYWORD) {
        current_mode = NORMAL;
        printf("Error at line %d: endmacr without definition\n", current_line);
        *error_flag = 1;
        return NORMAL;
    }

    /* Searching for macro definition */
    if ((tokens[0].type == MACRO_KEYWORD)) { /* macr <identifier> */
        if (token_count > 2) {
            printf("Error at line %d: macro definition with extra arguments: '%s' contains invalid character\n", current_line, tokens[1].value);
            *error_flag = 1;
            return NORMAL;
        }

        if (!is_valid_macro_name(tokens[1].value, current_line , error_flag)) {
            current_mode = NORMAL;
            *error_flag = 1;
            return NORMAL;
        }
        current_mode = DEFINITION;
        line_ptr += MACRO_KEYWORD_LENGTH;
        line_ptr = skipSpaces(line_ptr);

        if (!is_valid_macro_name(line_ptr, current_line, error_flag)) {
            current_mode = NORMAL;
            return NORMAL;
        }

        macro_index = add_macro(macro_table, line_ptr);
        if (macro_index == -1) {
            printf("Error: Failed to add macro '%s'\n", line_ptr);
            *error_flag = 1;
            
        }
        return macro_index;
    }

    /* Write the line to the output file */
    fputs(line, output_file);
    fputs("\n", output_file);
    return NORMAL;
}

static void handle_definition_mode(char *line, int macro_index, int token_count , Token *tokens ,int *error_flag) {

    /* Assuming each macr has a valid endmacr as per the assignment assumptions for macros */
    if (tokens[0].type == ENDMACRO_KEYWORD) {
        if (token_count > 1) {
            printf("Error: endmacr definition with extra arguments: '%s' contains invalid character\n", tokens[1].value);
            *error_flag = 1;
        }
        current_mode = NORMAL;
        return;
    }

    if (!add_macro_line(macro_table, macro_index, line)) {
        printf("Error: Failed to add line to macro\n");
        *error_flag = 1;
    }
}

static void handle_expansion_mode(char *line, FILE *output_file,int current_line,int token_count ,Token *tokens ,int macro_index , int *error_flag) {
    int i;
    Macro *m = &macro_table->macros[macro_index];
    
    for (i = 0; i < m->line_count; i++) {
        fputs(m->body[i], output_file);
        fputc('\n', output_file);
    }
    current_mode = NORMAL;
    handle_normal_mode(line, output_file, current_line, token_count, tokens, error_flag);
}

static int is_valid_macro_name(const char *name, int current_line, int *error_flag) {
    if (is_reserved_word(name)) {
        printf("Error at line %d: Invalid macro name: '%s' is a reserved word\n", current_line, name);
        *error_flag = 1;
        return 0;
    }
    
    if (!isalpha(*name)) {
        printf("Error at line %d: Invalid macro name: '%s' must start with a letter\n", current_line, name);
        *error_flag = 1;
        return 0;
    }
    
    while (*name) {
        if (!isalnum(*name) && *name != '_') {
            printf("Error at line %d: Invalid macro name: '%s' contains invalid character\n", current_line, name);
            *error_flag = 1;
            return 0;
        }
        name++;
    }
    
    return 1;
}


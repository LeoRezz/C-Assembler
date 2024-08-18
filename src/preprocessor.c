#include "preprocessor.h"
#include "symbolTable.h"
#include "util.h"

/* Macro structure */
typedef struct macro {
    char name[MAX_MACRO_NAME];   /* Name of the macro */
    char *body[MAX_MACRO_LINES]; /* max lines to be defined in a single macro */
    int line_count;
} macro;

/* Global macro table */
macro macro_table[MAX_MACROS]; /* TODO: change to dynamic allocation */
int macro_count = 0;

/* Global State mode */
typedef enum { NORMAL, DEFINITION, EXPANSION } Mode;
Mode current_mode = NORMAL;
extern int error_flag;

/* Function prototypes */
int set_macro_name(char *macroName, macro *macro_table, int current_line);
int find_macro_usage(char *line);
int addMacroLines(char *line);
int is_valid_name(char *name , int current_line);
int has_extra_chars(char *line);
void cleanupMacros();
void trim_trailing_whitespace(char *str);

int preprocess(const char *input_filename, const char *am_filename) {
    int macro_index;
    int current_line;
    char line[MAX_LINE];
    FILE *input_file = fopen(input_filename, "r");
    if (!input_file) {
        printf("Failed to open input file: %s\n", input_filename);
        return 0;
    }

    FILE *output_file = fopen(am_filename, "w");
    if (!output_file) {
        printf("Failed to create intermediate output file: %s\n", am_filename);
        fclose(input_file);
        return 0;
    }
    init_symbol_table(); /* Initialize symbol table */
    macro_index = 0;
    current_line = 0;
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
            handle_definition_mode(line, macro_table);
            break;
        case EXPANSION:
            handle_expansion_mode(line, output_file, macro_index);
            break;
        }
    }

    fclose(input_file);
    fclose(output_file);

    cleanupMacros();

    return (error_flag == 0);
}

void trim_trailing_whitespace(char *str) {
    int len = strlen(str);
    while (len > 0 && isspace((unsigned char)str[len - 1])) {
        str[--len] = '\0';
    }
}

int handle_normal_mode(char *line, FILE *output_file,int current_line) {
    char *line_ptr;
    int index;
    line_ptr = skipSpaces(line);
    if (*line_ptr == '\0' || *line_ptr == '\n' || *line_ptr == ';') {
        return -1; /* Skip empty lines and comments */
    }

    if (strncmp(line_ptr, "macr", MACRO_KEYWORD_LENGTH) == 0) {
        current_mode = DEFINITION;
        line_ptr += MACRO_KEYWORD_LENGTH; /* Advance the pointer to the macro name */
        line_ptr = skipSpaces(line_ptr);
        set_macro_name(line_ptr, macro_table,current_line);
        return -1;
    }
    index = find_macro_usage(line);

    if (index != -1) {
        current_mode = EXPANSION;
        printf("EXPANSION mode at index %d activated for: %s\n", index, macro_table[index].name);
        return index;
    }

    printf("NORMAL fputs: %s\n", line);
    fputs(line, output_file);
    fputc('\n', output_file);  /* Add newline after each line */
}

/* Handle macro definition mode */
void handle_definition_mode(char *line, macro *macro_table) {
    char *p = skipSpaces(line);

    if (strncmp(p, ENDMACRO_KEYWORD, ENDMACRO_KEYWORD_LENGTH) == 0) {
        current_mode = NORMAL;
        macro_count++;
        return;
    }
    /* Assuming no Errors in endmacr directive */
    addMacroLines(line);
    printf("%s: line #%d %s\n", macro_table[macro_count].name, macro_table[macro_count].line_count, line);
}

void handle_expansion_mode(char *line, FILE *output_file , int macro_index) {
    int i;
    /* Handle macro expansion mode */
    for (i = 0; i < macro_table[macro_index].line_count; i++) {
        fputs(macro_table[macro_index].body[i], output_file);
        fputc('\n', output_file);  /* Add newline after each line */
    }
    current_mode = NORMAL;
    handle_normal_mode(line, output_file, 0);
}

int set_macro_name(char *macroName, macro *macro_table, int current_line) {
  if (!is_valid_name(macroName, current_line)) {
    error_flag = 1;
    return 0; /* Error code */
  }
  strncpy(macro_table[macro_count].name, macroName, MAX_MACRO_NAME - 1);
  macro_table[macro_count].name[MAX_MACRO_NAME - 1] = '\0'; /* Ensure null-termination */

  /* Add the macro name to the symbol table for cross refrence later */
  if (!add_symbol(macroName, 0, SYMBOL_MACRO)) {
    printf("Error at line %d: Failed to add macro '%s' to symbol table\n", current_line, macroName);
    error_flag = 1;
    return 0;
  }

  printf("Macro name is: %s\n", macro_table[macro_count].name);
  return 0; /* Success */
}

/* Receives a char array of the name declaration after "macr " */
/* Checks for extra characters on the macro definition line */
/* Checks for Reserved words */
int is_valid_name(char *name, int current_line) {
    int i;
    /* Check for extra characters first */
    if (has_extra_chars(name) == 0) {
        printf("Error at line %d: Invalid macro name: '%s' extra characters found\n", current_line, name);
        error_flag = 1;
        return 0;
    }
    if (is_reserved_word(name)) {
        printf("Error at line %d: Invalid macro name: '%s' is a reserved word\n", current_line, name);
        error_flag = 1;
        return 0;
    }
    return 1;
}


int addMacroLines(char *line) {
    int lineCount = macro_table[macro_count].line_count;
    char *temp;
    temp = (char *)malloc(strlen(line) + 1);
    if (temp) {
        strcpy(temp, line);
        macro_table[macro_count].body[lineCount] = temp;
        macro_table[macro_count].line_count += 1;
    } else
        return 0;
    return 1;
}


int has_extra_chars(char *name) {
    char *p;
    if (name == NULL) {
        return 0; /* Null pointer reference */
    }
    
    p = skipSpaces(name);
    while (*p != '\0' && *p != '\n') {
        if (!isalnum((unsigned char)*p) && *p != '_') {
            return 0; /* Invalid character found */
        }
        p++;
    }

    if (*p != '\0' && *p != '\n') {
        return 0; /* Trailing characters found */
    }

    return 1; /* No extra characters found */
}

/* If name is found in macro table, returns index */
int find_macro_usage(char *line) {
    int i;
    char *temp = line;
    while (isspace((unsigned char)*temp)) {
        temp++;
    }
    for (i = macro_count - 1; i >= 0; i--) {
        if (strncmp(temp, macro_table[i].name, strlen(macro_table[i].name)) == 0) {
            return i;
        }
    }
    return -1;
}

void cleanupMacros() {
    int i, j;
    for (i = 0; i < macro_count; i++) {
        for (j = 0; j < macro_table[i].line_count; j++) {
            if (macro_table[i].body[j] != NULL) {
                free(macro_table[i].body[j]);
                macro_table[i].body[j] = NULL;
            }
        }
        macro_table[i].line_count = 0;
    }
    macro_count = 0;
}

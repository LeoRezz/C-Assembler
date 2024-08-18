#include "preprocessor.h"
#include "symbolTable.h"
#include "util.h"

/* Macro structure */
typedef struct macro {
    char name[MAX_MACRO_NAME];   /* Name of the macro */
    char *body[MAX_MACRO_LINES]; /* max lines to be defined in a single macro */
    int line_count;
} macro;

/* Reserved words */
static const char *reserved_words[] = {
    /* Registers */
    "r0", "r1", "r2", "r3", "r4", "r5", "r6", "r7",

    /* Opcodes (Instructions) */
    "mov", "cmp", "add", "sub", "lea", "clr", "not",
    "inc", "dec", "jmp", "bne", "red", "prn", "jsr", "rts", "stop",

    /* Directives */
    ".data", ".string", ".entry", ".extern",

    /* Special symbols */
    "#", /* For immediate addressing */

    /* Null terminator to mark end of array */
    NULL};

/* Global macro table */
macro macro_table[MAX_MACROS]; /* TODO: change to dynamic allocation */
int macro_count = 0;
int macroIndex = -1;

/* Global State mode */
typedef enum { NORMAL,
               DEFINITION,
               EXPANSION } Mode;
Mode current_mode = NORMAL;

/* Function prototypes */
int set_macro_name(char *macroName, macro *macro_table);
int findMacroIndex(char *line);
int addMacroLines(char *line);
int is_valid_name(char *line);
int has_extra_chars(char *line);
void cleanupMacros();
void trim_trailing_whitespace(char *str);

int preprocess(const char *input_filename, const char *am_filename) {
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

    char line[MAX_LINE];

    while (fgets(line, MAX_LINE, input_file) != NULL) {
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

    cleanupMacros();

    return 1;
}

void trim_trailing_whitespace(char *str) {
    int len = strlen(str);
    while (len > 0 && isspace((unsigned char)str[len - 1])) {
        str[--len] = '\0';
    }
}

void handle_normal_mode(char *line, FILE *output_file) {
    char *p;
    p = skipSpaces(line);
    if (*p == '\0' || *p == '\n' || *p == ';') {
        return; /* Skip empty lines and comments */
    }

    if (strncmp(p, MACRO_KEYWORD, MACRO_KEYWORD_LENGTH) == 0) {
        current_mode = DEFINITION;
        p += MACRO_KEYWORD_LENGTH;
        set_macro_name(p, macro_table);
        return;
    }

    if (findMacroIndex(line) != 0) {
        current_mode = EXPANSION;
        printf("EXPANSION mode at index %d activated for: %s\n", macroIndex, macro_table[macroIndex].name);
        return;
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

    addMacroLines(line);
    printf("%s: line #%d %s\n", macro_table[macro_count].name, macro_table[macro_count].line_count, line);
}

void handle_expansion_mode(char *line, FILE *output_file) {
    int i;
    /* Handle macro expansion mode */
    for (i = 0; i < macro_table[macroIndex].line_count; i++) {
        fputs(macro_table[macroIndex].body[i], output_file);
        fputc('\n', output_file);  /* Add newline after each line */
    }
    current_mode = NORMAL;
    handle_normal_mode(line, output_file);
}

int set_macro_name(char *macroName, macro *macro_table) {
    macroName[strlen(macroName) - 1] = '\0';
    if (!is_valid_name(macroName)) {
        return -1; /* Error code */
    }
    strncpy(macro_table[macro_count].name, macroName, MAX_MACRO_NAME - 1);
    macro_table[macro_count].name[MAX_MACRO_NAME - 1] = '\0'; /* Ensure null-termination */
    
    /* Add the macro name to the symbol table for cross refrence later */
    if (!add_symbol(macroName, 0, SYMBOL_MACRO)) {
        printf("Error: Failed to add macro '%s' to symbol table\n", macroName);
        return -1;
    }
    
    printf("Macro name is: %s\n", macro_table[macro_count].name);
    return 0; /* Success */
}

/* Receives a char pointer of the name declaration after "macr " */
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

/* If name is found in macro table, returns index */
int findMacroIndex(char *line) {
    int i;
    char *temp = line;
    while (isspace((unsigned char)*temp)) {
        temp++;
    }
    for (i = macro_count - 1; i >= 0; i--) {
        if (strncmp(temp, macro_table[i].name, strlen(macro_table[i].name)) == 0) {
            macroIndex = i;
            return 1;
        }
    }
    return 0;
}

int has_extra_chars(char *line) {
    char *p = line;
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
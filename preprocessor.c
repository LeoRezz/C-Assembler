#include "preprocessor.h"
#include "util.h"
#define STRCPY_MALLOC(dst, src) ((dst) = (char *)malloc(strlen(src) + 1)) ? strcpy(dst, src) : NULL

/* Macro structure */
typedef struct macro {
    char name[MAX_MACRO_NAME];   // Name of the macro
    char *body[MAX_MACRO_LINES]; // max lines to be defined in a single macro
    int line_count;
} macro;

/* Reserved words */
const char *reserved_word[NUM_OF_RESERVED] = {
    ".data", ".string", ".entry", ".extern",
    "mov", "cmp", "add", "sub", "lea",
    "clr", "not", "inc", "dec",
    "jmp", "bne", "red", "prn",
    "jsr", "rts", "stop"};

/* Global macro table */
macro macro_table[MAX_MACROS]; // Array of macros with size 100.
int macro_count = 0;
int macroIndex = -1;

/* Global State mode */
typedef enum { NORMAL,
               DEFINITION,
               EXPANSION } Mode;
Mode current_mode = NORMAL;

int setMacroName(char *macroName, macro *macro_table);
int findMacroIndex(char *line);
void addMacroLines(char *line);
int isValidName(char *line);
int hasExtraChar(char *line);
void cleanupMacros();

/* Main function to be changed to: int preprocess(char *input_filename, char *am_filename) */
int main(int argc, char *argv[]) {
    FILE *input_file = NULL;
    FILE *output_file = NULL;
    char *input_name = NULL;
    char *output_name = NULL;

    char line[MAX_LINE];

    if (argc != 2) {
        printf("Usage: %s <Enter source code file name here>\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    /* Open files*/
    input_name = my_strdup(argv[1]);
    output_name = my_strdup(argv[1]);
    TRY(strcat(input_name, ".as"));
    TRY(strcat(output_name, ".am"));
    input_file = fopen(input_name, "r");
    output_file = fopen(output_name, "w");

    /* fopen() return NULL if unable to open file in given mode. */
    if (!input_file) {
        printf("Failed to open input file: %s\n", argv[1]);
        exit(EXIT_FAILURE);
    }
    if (!output_file) {
        printf("Failed to create temporary file: temp.ob\n");
        fclose(input_file);
        exit(EXIT_FAILURE);
    }

    /* Reads a single line each iteration of the loop */
    while (fgets(line, MAX_LINE, input_file) != NULL) {
        /* Based on the current mode, perform the appropriate action */
        switch (current_mode) {
        case NORMAL:
            /* If the line contains no macro commands, write the line into a new file */
            handle_normal_mode(line, output_file);
            break;
        case DEFINITION:
            /* If a macro declaration is encountered, store the body of the macro for later expansion */
            handle_definition_mode(line, macro_table);
            break;
        case EXPANSION:
            /* If a macro name is encountered, expand the corresponding macro body */
            handle_expansion_mode(line, output_file);
            break;
        }
    }

    fclose(input_file);
    fclose(output_file);
    cleanupMacros();
    free(input_name);
    free(output_name);

    return 0;
}
/*
1. Reading line by line
2. If a macro declaration is encountered, it switches to Definition mode (D).
3. If a macro name is encountered, it switches to Expantion mode (E)
*/
void handle_normal_mode(char *line, FILE *output_file) {
    char *p = skipSpaces(line);
    if (*p == '\0' || *p == '\n' || *p == ';') {
        printf("*p = %c\tDELETE LINE\n", *p);
        return;
    }

    if (strncmp(p, MACRO_KEYWORD, MACRO_KEYWORD_LENGTH) == 0) {
        current_mode = DEFINITION;
        p += MACRO_KEYWORD_LENGTH;
        setMacroName(p, macro_table);
        return;
    }

    if (findMacroIndex(line) != 0) {
        current_mode = EXPANSION;
        printf("EXPANSION mode at index %d activated for: %s\n", macroIndex, macro_table[macroIndex].name);
        return;
    }

    printf("NORMAL fputs: %s", line);
    fputs(line, output_file);
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
    printf("%s: line #%d %s", macro_table[macro_count].name, macro_table[macro_count].line_count, line);
}

void handle_expansion_mode(char *line, FILE *output_file) {
    int i;
    printf("Reached EXPANSION with macroIndex %d\n", macroIndex);
    // Handle macro expansion mode
    for (i = 0; i < macro_table[macroIndex].line_count; i++) {
        printf("fputs: %s", macro_table[macroIndex].body[i]);
        fputs(macro_table[macroIndex].body[i], output_file);
    }
    printf("\nExiting EXPANSION\n");
    current_mode = NORMAL;
    handle_normal_mode(line, output_file);
}

int setMacroName(char *macroName, macro *macro_table) {
    macroName[strlen(macroName) - 1] = '\0';
    if (!isValidName(macroName)) {
        return -1; // Error code
    }
    strncpy(macro_table[macro_count].name, macroName, MAX_MACRO_NAME - 1);
    printf("Macro name is: %s\n", macro_table[macro_count].name);
    return 0; // Success
}


/* Recives a char pointer of the name declration after "macr " */
/* Checks for extra characters on the macro definition line */
/* Checks for Reserved words */
int isValidName(char *line) {
    int i;
    /* Check for extra characters first */
    if (hasExtraChar(line) == 0) {
        printf("Invalid macro name: '%s' extra characters found\n", line);
        exit(EXIT_FAILURE);
    }

    // Check against reserved words
    for (i = 0; i < NUM_OF_RESERVED; i++) {
        if (strncmp(line, reserved_word[i], strlen(reserved_word[i])) == 0) {
            printf("Invalid macro name: '%s' is a reserved word\n", line);
            exit(EXIT_FAILURE);
        }
    }
    return 1;
}

void addMacroLines(char *line) {
    int lineCount = macro_table[macro_count].line_count;
    char *temp;
    temp = (char *)malloc(strlen(line) + 1);
    if (temp) {
        strcpy(temp, line);
        macro_table[macro_count].body[lineCount] = temp;
        macro_table[macro_count].line_count += 1;
    } else
        exit(EXIT_FAILURE);
}

/* If name is found in macro table, returns index*/
int findMacroIndex(char *line) {
    int i;
    char *temp = line;
    while (isspace(*(temp))) {
        temp++;
    }
    for (i = macro_count - 1; i >= 0; i--) {
        /*printf("\nComparing: %s With: %s\n", temp, macro_table[i].name);*/
        if (strncmp(temp, macro_table[i].name, strlen(macro_table[i].name)) == 0) {
            macroIndex = i;
            printf("\nFound %s at index %d\n", macro_table[macroIndex].name, macroIndex);
            return 1;
        }
    }
    return 0;
}

int hasExtraChar(char *line) {
    char *p = line;
    while (*p != '\0' && *p != '\n') {
        if (!isalnum(*p) && *p != '_') {
            return 0; // Invalid character found
        }
        p++;
    }

    return 1; // No extra characters found
}

void cleanupMacros() {
    for (int i = 0; i < macro_count; i++) {
        for (int j = 0; j < macro_table[i].line_count; j++) {
            free(macro_table[i].body[j]);
        }
    }
}
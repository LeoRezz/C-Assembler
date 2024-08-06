#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LABEL 33
#define MAX_LINE 82
#define MAX_MACRO_NAME 77
#define MAX_MACRO_LINES 50
#define MAX_MACROS 50
#define NUM_OF_RESERVED 20
#define MACRO_KEYWORD "macr "
#define MACRO_KEYWORD_LENGTH 5
#define ENDMACRO_KEYWORD "endmacr"
#define ENDMACRO_KEYWORD_LENGTH 7

/* Macro structure */
typedef struct macro macro;

void handle_normal_mode(char *line, FILE *output_file);

void handle_definition_mode(char *line, macro *macro_table);

void handle_expansion_mode(char *line, FILE *output_file);

int setMacroName(char *line, macro *macro_table);
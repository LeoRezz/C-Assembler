#ifndef PREPROCESSOR_H
#define PREPROCESSOR_H

#include "util.h"

#define MAX_LABEL 33
#define MAX_LINE 82
#define MAX_MACRO_NAME 77
#define MAX_MACRO_LINES 50
#define MAX_MACROS 50
#define MACRO_KEYWORD "macr"
#define MACRO_KEYWORD_LENGTH 4
#define ENDMACRO_KEYWORD "endmacr"
#define ENDMACRO_KEYWORD_LENGTH 7

/* Macro structure */
typedef struct macro macro;

int preprocess(const char *input_filename, const char *am_filename);
int handle_normal_mode(char *line, FILE *output_file, int current_line);
void handle_definition_mode(char *line, macro *macro_table);

void handle_expansion_mode(char *line, FILE *output_file, int macro_index);

int setMacroName(char *line, macro *macro_table);

void cleanupMacros();

#endif /* PREPROCESSOR_H */
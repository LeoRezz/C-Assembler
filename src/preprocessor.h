#ifndef PREPROCESSOR_H
#define PREPROCESSOR_H

/* Responsible for preprocessing the input file, expanding macros and removes comments */
/* Creates an intermediate .am output file, for later parsing in the assembler flow */

#include "util.h"

/* Constants */
#define MACRO_KEYWORD_LENGTH 4
#define ENDMACRO_KEYWORD_LENGTH 7

/* Function prototypes */

/* Main function that calls the preprocess state machine functions */
int preprocess(const char *input_filename, const char *am_filename);


#endif 
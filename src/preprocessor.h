#ifndef PREPROCESSOR_H
#define PREPROCESSOR_H

#include "util.h"

/* Constants */
#define MAX_LINE 82
#define MACRO_KEYWORD "macr"
#define MACRO_KEYWORD_LENGTH 4
#define ENDMACRO_KEYWORD "endmacr"
#define ENDMACRO_KEYWORD_LENGTH 7

/* Function prototypes */
/* TODO: Document the purpose, parameters, and return value of this function */
int preprocess(const char *input_filename, const char *am_filename);

#endif /* PREPROCESSOR_H */
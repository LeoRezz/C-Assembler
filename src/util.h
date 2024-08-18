#ifndef UTIL_H
#define UTIL_H

/* Standard library includes */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/* Constants */
#define MAX_FILENAME 50
#define MAX_LINE 82
#define MAX_LABEL_LENGTH 31
#define MAX_SYMBOL_LENGTH 31
#define MAX_MACRO_NAME 31
#define MAX_IMMEDIATE_VALUE 2047
#define MIN_IMMEDIATE_VALUE -2048
#define INITIAL_ADDRESS 100


/* Macro for error checking */
#define TRY(a)       \
    if (!(a)) {      \
        perror(#a);  \
        exit(1);     \
    }

/* Function prototypes */
/* TODO: Document each of these functions */
void trim_trailing_whitespace(char *str);
int is_reserved_word(const char *word);
char *my_strdup(const char *s);
char *skipSpaces(char *s);
void skip_spaces(char *s);
void append_extension(char *destination, const char *source, const char *extension);

/* Add any other utility functions you've defined */

#endif 
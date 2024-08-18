#ifndef UTIL_H
#define UTIL_H
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define EXTESION_LENGTH 5
#define MAX_FILENAME 50
#define MAX_IMMEDIATE_VALUE 2047
#define MIN_IMMEDIATE_VALUE -2048
#define STRING(s) #s
#define CONCAT(x, y) STRING(x##y)

#define TRY(a)       \
    if (!(a)) {      \
        perror(#a);  \
        exit(1); \
    }


/* trim: remove trailing blanks, tabs, newlines */
int trim_trailing_spaces(char s[]);


int is_reserved_word(const char *word);

/* make a duplicate of s */
char *my_strdup(char *s);

char *skipSpaces(char *s);

void skip_spaces(char *s);

void append_extension(char *destination, const char *source, const char *extension);
#endif /* UTIL_H */
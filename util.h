#ifndef UTIL_H
#define UTIL_H
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define STRING(s) #s
#define CONCAT(x, y) STRING(x##y)
#define TOKEN_OK 0
#define ERROR_INVALID_IMMEDIATE -1
#define ERROR_INVALID_REGISTER -2

#define TRY(a)       \
    if (!(a)) {      \
        perror(#a);  \
        return NULL; \
    }

/* Add any common utility function declarations here */

/* trim: remove trailing blanks, tabs, newlines */
int trim_trailing_spaces(char s[]);

/* make a duplicate of s */
char *my_strdup(char *s);

char *skipSpaces(char *s);

void skip_spaces(char *s);

/* getword: get next word or character from input */
int my_getword_file(char *word, int lim, FILE *fp);

int my_getword_simple(char *word, int lim, char **line);

/* getword: get next word or character from line */
int my_getword(char *word, int lim, const char **line, int *error);

#endif /* UTIL_H */
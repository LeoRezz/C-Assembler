#include "util.h"

/* trim: remove trailing blanks, tabs, newlines */
int trim(char s[]) {
    int n;
    for (n = strlen(s) - 1; n >= 0; n--) {
        if (s[n] != ' ' && s[n] != '\t' && s[n] != '\n') {
            break;
        }
    }
    s[n + 1] = '\0';
    return n;
}

/* make a duplicate of s */
char *my_strdup(char *s) 
{
    char *p;
    p = (char *)malloc(strlen(s) + 1); /* +1 for '\0' */
    if (p != NULL)
        strcpy(p, s);
    return p;
}

char *skipSpaces(char *s) {
    char *ptr = s;
    /* Skip whitespace and tabs characters */
    while (isspace(*ptr)) {
        ptr++;
    }
    return ptr;
}

void skip_spaces(char *s) {
    /* Skip whitespace and tabs characters */
    while (isspace(*s)) {
        s++;
    }
    return;
}
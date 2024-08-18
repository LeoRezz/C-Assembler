#include "util.h"

void append_extension(char *destination, const char *source, const char *extension) {
    int source_len;
    int ext_len;

    source_len = strlen(source);
    ext_len = strlen(extension);

    strncpy(destination, source, MAX_FILENAME - ext_len - 1);
    destination[MAX_FILENAME - ext_len - 1] = '\0';
    strcat(destination, extension);
}

/* trim: remove trailing blanks, tabs, newlines */
int trim_trailing_spaces(char s[]) {
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
char *my_strdup(char *s) {
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


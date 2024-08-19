#include "util.h"

void append_extension(char *destination, const char *source, const char *extension) {

    strncpy(destination, source, MAX_FILENAME);
    destination[MAX_FILENAME] = '\0';
    strcat(destination, extension);
}

/* trim: remove trailing blanks, tabs, newlines */
void trim_trailing_whitespace(char *line) {
    int i = strlen(line) - 1;
    while (i >= 0 && isspace(line[i])) {
        line[i] = '\0';
        i--;
    }
}


/* make a duplicate of s */
char *my_strdup(const char *s) {
    char *p;
    p = (char *)malloc(strlen(s) + 1); /* +1 for '\0' */
    if (p != NULL)
        strcpy(p, s);
    return p;
}

char *skipSpaces(char *s) {
    char *ptr = s;
    /* Skip whitespace and tabs characters */
    while (isspace(*ptr) && *ptr != '\0') {
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

/* Reserved words */
static const char *reserved_words[] = {
    /* Registers */
    "r0", "r1", "r2", "r3", "r4", "r5", "r6", "r7",

    /* Opcodes (Instructions) */
    "mov", "cmp", "add", "sub", "lea", "clr", "not",
    "inc", "dec", "jmp", "bne", "red", "prn", "jsr", "rts", "stop",

    /* Directives */
    ".data", ".string", ".entry", ".extern",

    /* Preprocessor */
    "macr", "endmacr",


    /* Null terminator to mark end of array */
    NULL};


int is_reserved_word(const char *word) {
    int i;
    for (i = 0; reserved_words[i] != NULL; i++) {
        if (strcmp(word, reserved_words[i]) == 0) {
            return 1; /* TRUE, found a reserved word */
        }
    }
    return 0; /* FALSE, not a reserved word */
}
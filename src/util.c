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

/* Reserved words */
static const char *reserved_words[] = {
    /* Registers */
    "r0", "r1", "r2", "r3", "r4", "r5", "r6", "r7",

    /* Opcodes (Instructions) */
    "mov", "cmp", "add", "sub", "lea", "clr", "not",
    "inc", "dec", "jmp", "bne", "red", "prn", "jsr", "rts", "stop",

    /* Directives */
    ".data", ".string", ".entry", ".extern",


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
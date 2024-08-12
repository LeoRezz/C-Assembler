#include "util.h"

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

/* getword: get next word or character from input */
int my_getword_file(char *word, int lim, FILE *fp) {
    int c;
    char *w = word;
    /* Skip leading whitespace */
    while (isspace(c = getc(fp)))
        ;
    if (c != EOF)
        *w++ = c;
    /* If not alphanumeric, return the character */
    if (!isalnum(c)) {
        *w = '\0';
        return c;
    }
    /* Read alphanumeric characters until limit is reached or non-alphanumeric is found */
    for (; --lim > 0; w++)
        if (!isalnum(*w = getc(fp))) {
            ungetc(*w, fp);
            break;
        }
    /* Null-terminate the word */
    *w = '\0';
    /* Return the first character of the word */
    return word[0];
}

int my_getword_simple(char *word, int lim, char **line) {
    int c;
    char *w = word;

    /* Skip leading whitespace */
    while (isspace(**line))
        (*line)++;

    if (**line == '\0')
        return EOF;

    c = **line;
    (*line)++;

    /* If not alphanumeric, return the character */
    if (!isalnum(c)) {
        *w++ = c;
        *w = '\0';
        return c;
    }

    *w++ = c;
    /* Read alphanumeric characters until limit is reached or non-alphanumeric is
     * found */
    for (; --lim > 1; w++) {
        if (!isalnum(**line))
            break;
        *w = **line;
        (*line)++;
    }

    *w = '\0';
    return *word;
}

int my_getword(char *word, int lim, const char **line, int *error) {
    int c;
    char *w = word;
    const char *l = *line;

    *error = TOKEN_OK;  // Initialize error to OK

    /* Skip leading whitespace */
    while (isspace(*l))
        l++;

    if (*l == '\0')
        return EOF;

    c = *l++;

    switch (c) {
    case '"': /* Handle string literals */
        *w++ = c;
        while (--lim > 1 && *l != '\0' && *l != '"') {
            *w++ = *l++;
        }
        if (*l == '"') {
            *w++ = *l++;
        }
        break;

    case '#': /* Handle immediate values */
        *w++ = c;
        if (*l == '+' || *l == '-') {
            *w++ = *l++;
            lim--;
        }
        if (!isdigit(*l)) {
            *error = ERROR_INVALID_IMMEDIATE;
            break;
        }
        while (--lim > 1 && isdigit(*l)) {
            *w++ = *l++;
        }
        break;

    case '*': /* Handle indirect addressing */
        *w++ = c;
        if (*l != 'r') {
            *error = ERROR_INVALID_REGISTER;
            break;
        }
        *w++ = *l++;
        if (*l < '0' || *l > '7') {
            *error = ERROR_INVALID_REGISTER;
            break;
        }
        *w++ = *l++;
        break;

    case '+': /* Handle positive numbers or standalone + */
    case '-': /* Handle negative numbers or standalone - */
        *w++ = c;
        if (isdigit(*l)) {
            while (--lim > 1 && isdigit(*l)) {
                *w++ = *l++;
            }
        }
        break;

    case ',': /* Handle comma separator */
    case ':': /* Handle label terminator */
        *w++ = c;
        break;

    case '.': /* Handle directives */
        *w++ = c;
        while (--lim > 1 && isalpha(*l)) {
            *w++ = *l++;
        }
        break;

    default:
        if (c == 'r') { /* Handle registers */
            *w++ = c;
            if (*l >= '0' && *l <= '7') {
                *w++ = *l++;
            } else {
                *error = ERROR_INVALID_REGISTER;
                break;
            }
        } else if (isalpha(c)) { /* Handle labels and other identifiers */
            *w++ = c;
            while (--lim > 1 && (isalnum(*l))) {
                *w++ = *l++;
            }
        } else if (isdigit(c)) { /* Handle numeric values */
            *w++ = c;
            while (--lim > 1 && isdigit(*l)) {
                *w++ = *l++;
            }
        } else { /* Single non-alphanumeric character */
            *w++ = c;
        }
        break;
    }

    *w = '\0';
    *line = l;
    return c;
}
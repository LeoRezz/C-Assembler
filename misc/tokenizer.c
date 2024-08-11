#include "tokenizer.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_LINE 82

int my_getword(char *word, int lim, const char **line) {
    int c;
    char *w = word;
    const char *l = *line;

    /* Skip leading whitespace */
    while (isspace(*l))
        l++;

    if (*l == '\0')
        return EOF;

    c = *l++;

    switch (c) {
    case ';': /* Skip comments */
        return EOF;
        break;

    case '"': /* Handle string literals */
        *w++ = c;
        while (--lim > 1 && *l != '\0' && isprint(*l)) {
            *w++ = *l++;
        }
        *w = '\0';
        *line = l;
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

    case '.': /* Handle directives */
        *w++ = c;
        while (--lim > 1 && isalpha(*l)) {
            *w++ = *l++;
        }
        break;

    default:
        /* If not alphanumeric, return the character */
        if (!isalnum(c)) {
            *w++ = c;
            *w = '\0';
            *line = l;
            return c;
        }
        if (isalpha(c)) { /* Handle labels and other identifiers */
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

int main(int argc, char *argv[]) {
    FILE *input_file;
    Token tokens[100];
    int token_count;
    char *file_name;
    char line[MAX_LINE];
    token_count = 0;
    const char *line_ptr; // Use a pointer to traverse the line

    input_file = fopen("error_asm1.am", "r"); /* TODO: change later to argv[1] */
    /* fopen() return NULL if unable to open file in given mode. */
    if (!input_file) {
        printf("Failed to open input file: %s\n", argv[1]);
        free(file_name);
        exit(EXIT_FAILURE);
    }

    while (fgets(line, MAX_LINE, input_file) != NULL) {
        int counter = 1;
        line_ptr = line;
        while (my_getword(tokens[token_count].value, MAX_LINE, &line_ptr) != EOF) {
            printf("Token[%d] = %s\n", counter, tokens[token_count].value);
            token_count++;
            counter++;
        }
        printf("\n");
        /* code */
    }

    fclose(input_file);
    return 0;
}
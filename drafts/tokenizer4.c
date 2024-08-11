#include <stdio.h>
#include <ctype.h>
#include <string.h>

#define MAX_TOKEN_LENGTH 100
#define TOKEN_OK 0
#define ERROR_INVALID_IMMEDIATE -1
#define ERROR_INVALID_REGISTER -2

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

void process_line(const char *line) {
    char token[MAX_TOKEN_LENGTH];
    int error;
    int result;

    printf("Processing line: %s\n", line);

    while ((result = my_getword(token, MAX_TOKEN_LENGTH, &line, &error)) != EOF) {
        if (error != TOKEN_OK) {
            switch(error) {
                case ERROR_INVALID_IMMEDIATE:
                    printf("Error: Invalid immediate value in token '%s'\n", token);
                    break;
                case ERROR_INVALID_REGISTER:
                    printf("Error: Invalid register in token '%s'\n", token);
                    break;
                default:
                    printf("Unknown error in token '%s'\n", token);
            }
        } else {
            printf("Token: '%s'\n", token);
        }
    }
    printf("\n");
}

int main() {
    const char *test_lines[] = {
        "mov r1, #42",
        "add *r3, r5",
        "sub r0, #-10",
        "LABEL: jmp *r7",
        "mov r8, #5",  // Invalid register
        "add *x3, r2", // Invalid register
        "sub r1, #abc", // Invalid immediate
        ".data 1, 2, 3",
        ".string \"Hello, World!\"",
        "mov r1, r2,"  // Extra comma
    };

    int num_lines = sizeof(test_lines) / sizeof(test_lines[0]);

    for (int i = 0; i < num_lines; i++) {
        process_line(test_lines[i]);
    }

    return 0;
}
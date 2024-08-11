#include "tokenizer.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_LINE 82

static const char *reserved_words[] = {
    /* Registers */
    "r0", "r1", "r2", "r3", "r4", "r5", "r6", "r7",

    /* Opcodes (Instructions) */
    "mov", "cmp", "add", "sub", "not", "clr", "lea",
    "inc", "dec", "jmp", "bne", "red", "prn", "jsr", "rts", "stop",

    /* Directives */
    ".data", ".string", ".entry", ".extern",

    /* Special symbols */
    "#", /* For immediate addressing */

    /* Null terminator to mark end of array */
    NULL};

TokenType get_token_type(const char *token) {
    int i;
    int len;
    len = strlen(token);

    for (i = 0; reserved_words[i] != NULL; i++) {
        if (strcmp(token, reserved_words[i]) == 0) {
            /* Found a reserved word, reutnrs its TokenType enum */
            return (TokenType)i;
        }
    }

    /* If not a reserved word, determine other token types */
    if (token[0] == '#') {
        return HASH;
    }
    if (token[0] == ',') {
        return COMMA;
    }
    if (token[0] == '*') {
        return ASTERISK;
    }

    if (isdigit((unsigned char)token[0]) || ((token[0] == '-') || (token[0] == '+')) && isdigit((unsigned char)token[1])) {
        return INTEGER;
    }
    if (token[0] == '"' && token[len - 1] == '"') {
        return STRING_LITERAL;
    }

    if (isalpha((unsigned char)token[0])) {
        return LABEL_USE;
    }
        /* If none of the above, it's a potential forward refrence */
        return UNKNOWN;
    }

const char *token_type_to_string(TokenType type) {
    switch (type) {
        case R0:
            return "R0";
        case R1:
            return "R1";
        case R2:
            return "R2";
        case R3:
            return "R3";
        case R4:
            return "R4";
        case R5:
            return "R5";
        case R6:
            return "R6";
        case R7:
            return "R7";
        case MOV:
            return "MOV";
        case CMP:
            return "CMP";
        case ADD:
            return "ADD";
        case SUB:
            return "SUB";
        case NOT:
            return "NOT";
        case CLR:
            return "CLR";
        case LEA:
            return "LEA";
        case INC:
            return "INC";
        case DEC:
            return "DEC";
        case JMP:
            return "JMP";
        case BNE:
            return "BNE";
        case RED:
            return "RED";
        case PRN:
            return "PRN";
        case JSR:
            return "JSR";
        case RTS:
            return "RTS";
        case STOP:
            return "STOP";
        case DATA:
            return "DATA";
        case STRING:
            return "STRING";
        case ENTRY:
            return "ENTRY";
        case EXTERN:
            return "EXTERN";
        case HASH:
            return "HASH";
        case INTEGER:
            return "INTEGER";
        case STRING_LITERAL:
            return "STRING_LITERAL";
        case EOL:
            return "EOL";
        case EOF:
            return "EOF";
        case UNKNOWN:
            return "UNKNOWN";
        case LABEL_DEF:
            return "LABEL_DEF";
        case LABEL_USE:
            return "LABEL_USE";
        case COMMA:
            return "COMMA";
        case ASTERISK:
            return "ASTERISK";
        default:
            return "UNDEFINED";
    }
}
int my_getword(char *word, int lim, const char **line) {
    int c;
    char *w = word;
    const char *l = *line;
    int is_label;
    is_label = 0;

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
        /* Handle potential label */
        if (isalpha(c)) {
            *w++ = c;
            while (--lim > 1 && (isalnum(*l))) {
                *w++ = *l++;
            }
            if (*l == ':') {
                is_label = 1;
                l++; /* Skip the colon */
                *w = '\0';
                *line = l;
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
    return is_label ? ':' : c; /* Return ':' if it's a label */
}

int main(int argc, char *argv[]) {
    char *p;
    FILE *input_file;
    Token tokens[100];
    char token_type[50];
    int token_count;
    char *file_name;
    char line[MAX_LINE];
    int label_def_flag;
    label_def_flag = 0;
    token_count = 0;
    const char *line_ptr; // Use a pointer to traverse the line

    input_file = fopen("asm1.am", "r"); /* TODO: change later to argv[1] */
    /* fopen() return NULL if unable to open file in given mode. */
    if (!input_file) {
        printf("Failed to open input file: %s\n", argv[1]);
        free(file_name);
        exit(EXIT_FAILURE);
    }

    while (fgets(line, MAX_LINE, input_file) != NULL) {
        int counter = 1;
        p = strrchr(line, '\n');
        if (p)
            *p = '\0'; /* Remove newline character */
        line_ptr = line;
        printf("source [%d]:'%s'\n", counter, line);
        printf("---------tokenized---------\n");
        while ((label_def_flag = my_getword(tokens[token_count].value, MAX_LINE, &line_ptr)) != EOF) {
            
            if ((char)label_def_flag == ':') {
                printf("LABEL_DEF[%d] = %s\n", counter, tokens[token_count].value);
                tokens[token_count].type = LABEL_DEF;
                token_count++;
                counter++;
            } else {
                tokens[token_count].type = get_token_type(tokens[token_count].value);
                const char *type_str = token_type_to_string(tokens[token_count].type);
                printf("Token[%d] = %s\tType = %s\n", counter, tokens[token_count].value, type_str);
                token_count++;
                counter++;
            }
        }
        printf("\n");
        printf("\n");
        /* code */
    }

    fclose(input_file);
    return 0;
}
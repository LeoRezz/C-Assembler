#include "tokenizer.h"

/* my_getword seprates a given line to meaningful tokens,
   returns ':' when encounterd in label definition. */
static int my_getword(char *word, int lim, const char **line);


/**
 * @brief Tokenizes a given line of assembly code into an array of tokens with assigned type.
 *
 * This function processes a line of assembly code, breaking it down into individual tokens
 * and categorizing each token by its type. It handles labels, instructions, directives, 
 * and other symbols.
 *
 * @param line The input line of assembly code to be tokenized.
 * @param token_count A pointer to an integer where the function will store the number of tokens found.
 * @return A dynamically allocated array of tokens. Each token contains its value and type.
 *         The caller is responsible for freeing this array.
 */
Token *tokenize_line(const char *line, int *token_count) {
    Token *tokens;
    int label_def_flag;
    const char *line_ptr; 
    
    TRY(tokens = (Token *)calloc(MAX_TOKENS, sizeof(Token)));
    line_ptr = line; /* Use a pointer to traverse the line */
    *token_count = 0;
    label_def_flag = 0;
    
    while ((label_def_flag = my_getword(tokens[*token_count].value, MAX_LINE, &line_ptr)) != EOF) {
        if ((char)label_def_flag == ':') {
            tokens[*token_count].type = LABEL_DEF;
            (*token_count)++;
        } else {
            tokens[*token_count].type = get_token_type(tokens[*token_count].value);
            (*token_count)++;
        }
    }

    return tokens;
}

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
            /* Found a reserved word, return its TokenType enum */
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
    return ERROR;
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
    case ERROR:
        return "ERROR";
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
static int my_getword(char *word, int lim, const char **line) {
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
        ;

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


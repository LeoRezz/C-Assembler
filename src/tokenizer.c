#include "tokenizer.h"

/* my_getword seprates a given line to meaningful tokens,
   returns ':' when encounterd in label definition. */
static int my_getword(char *word, int lim, const char **line);
int is_valid_immediate(const char *token);
int is_valid_indirect_register(const char *token);
int is_reserved_word(const char *word);
int is_valid_register(const char *token);
void init_tokens(Token *tokens);
/* Tokenizes a given line of assembly code into tokens with assigned type. */
Token *tokenize_line(const char *line, int *tokens_count , int current_line) {
    Token *tokens;
    int label_def_flag;
    const char *line_ptr;
    
    TRY(tokens = (Token *)calloc(MAX_TOKENS, sizeof(Token)));
    init_tokens(tokens);
    line_ptr = line; /* Use a pointer to traverse the line */
    *tokens_count = 0;
    label_def_flag = 0;

    label_def_flag = my_getword(tokens[*tokens_count].value, MAX_LINE, &line_ptr);
    if ((char)label_def_flag == ':') {
        if (is_reserved_word(tokens[*tokens_count].value)) {
            printf("Error in line %d: Label name '%s:' is a reserved word\n", current_line, tokens[*tokens_count].value);
            free(tokens);
            return NULL;
        }
        tokens[*tokens_count].type = LABEL_DEF;
        (*tokens_count)++;
    }
    do {
        if ((tokens[*tokens_count].type = get_token_type(tokens[*tokens_count].value)) == ERROR) {
            printf("Error: Invalid token '%s'\n", tokens[*tokens_count].value);
            free(tokens);
            return NULL;
        }

        (*tokens_count)++;

    } while (my_getword(tokens[*tokens_count].value, MAX_LINE, &line_ptr) != EOF);

    return tokens;
}

void init_tokens(Token *tokens) {
    int i;
    for (i = 0; i < MAX_TOKENS; i++) {
        tokens[i].type = UNKNOWN;
        tokens[i].value[0] = '\0';
    }
}


static const char *reserved_words[] = {
    /* Registers */
    "r0", "r1", "r2", "r3", "r4", "r5", "r6", "r7",

    /* Opcodes (Instructions) */
    "mov", "cmp", "add", "sub", "lea", "clr", "not",
    "inc", "dec", "jmp", "bne", "red", "prn", "jsr", "rts", "stop",

    /* Directives */
    ".data", ".string", ".entry", ".extern",

    /* Special symbols */
    "#", /* For immediate addressing */

    /* Null terminator to mark end of array */
    NULL};

static const char *token_type[] = {
    
    /* Opcodes (Instructions) */
    "mov", "cmp", "add", "sub", "lea", "clr", "not",
    "inc", "dec", "jmp", "bne", "red", "prn", "jsr", "rts", "stop",

    /* Directives */
    ".data", ".string", ".entry", ".extern",

    /* Null terminator to mark end of array */
    NULL};

TokenType get_token_type(const char *token) {
    int i;
    int len;
    len = strlen(token);

    for (i = 0; token_type[i] != NULL; i++) {
        if (strcmp(token, token_type[i]) == 0) {
            /* Found a reserved word, return its TokenType enum */
            return (TokenType)i;
        }
    }

    /* If not a reserved word, determine other token types */
    if ((token[0] == '#') && is_valid_immediate(token)) {
        return IMMEDIATE;
    }

    if ((token[0] == '*') && is_valid_indirect_register(token)) {
        return INDIRECT_REGISTER;
    }

    if ((token[0] == 'r') && is_valid_register(token)) {
        return REGISTER;
    }

    if (token[0] == ',') {
        return COMMA;
    }


   if (isdigit((unsigned char)token[0]) || (((token[0] == '-') || (token[0] == '+')) && isdigit((unsigned char)token[1]))) {
       return INTEGER;
   }
   
    if (token[0] == '"' && token[len - 1] == '"') {
        return STRING_LITERAL;
    }

    /* If none of the above, it's a potential label refrence */
    if (isalpha((unsigned char)token[0])) {
        return DIRECET;
    }
    /* else it's an error */
    return ERROR;
}

int is_reserved_word(const char *word) {
    int i;
    for (i = 0; reserved_words[i] != NULL; i++) {
        if (strcmp(word, reserved_words[i]) == 0) {
            return 1;
        }
    }
    return 0;
}

int is_valid_immediate(const char *token) {
    const char *num_part;

    if (token[0] != '#')
        return 0;

    num_part = token + 1;

    /* Check if it's empty after '#' */
    if (*num_part == '\0')
        return 0;

    /* Check for optional sign */
    if (*num_part == '+' || *num_part == '-') {
        num_part++;
    }

    /* Must have at least one digit after sign */
    if (*num_part == '\0')
        return 0;

    /* Check if all remaining characters are digits */
    while (*num_part != '\0') {
        if (!isdigit((unsigned char)*num_part))
            return 0;
        num_part++;
    }

    return 1;
}

int is_valid_indirect_register(const char *token) {
    if (token[0] != '*')
        return 0;

    if (token[1] != 'r')
        return 0;

    if (token[2] < '0' || token[2] > '7')
        return 0;

    if (token[3] != '\0')
        return 0;

    return 1;
}

int is_valid_register(const char *token) {
    if (token[0] != 'r')
        return 0;
    if (token[1] < '0' || token[1] > '7')
        return 0;

    return 1;
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

    case '*':
        *w++ = c; /* Store the '*' */
        while ((--lim > 1 && *l != '\0') && (*l != ',' && !isspace(*l))) {
            *w++ = *l++;
        }
        break;

    case '#':
        *w++ = c; /* Store the '#' */
        while (--lim > 1 && *l != '\0' && !isspace(*l)) {
            *w++ = *l++;
        }
        break;

    case ';': /* Skip comments */
        return EOF;
        break;

    case '"': /* Handle string literals */
        *w++ = c;
        while (--lim > 1 && *l != '\0' && isprint(*l)) {
            *w++ = *l++;
        }
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
        /* Handle potential register or label */
        if (isalpha(c)) {
            *w++ = c;
            while (--lim > 1 && (*l != ':' && *l != ',' && !isspace(*l))) {
                *w++ = *l++;
            }
            if (*l == ':') {
                is_label = 1;
                l++; /* Skip the colon */
            }
            *w = '\0';
            *line = l;
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
    return is_label ? ':' : 0; /* Return ':' if it's a label */
}

const char *token_type_to_string(TokenType type) {
    switch (type) {
    case REGISTER:
        return "REGISTER";
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
    case IMMEDIATE:
        return "IMMEDIATE";
    case INTEGER:
        return "INTEGER";
    case STRING_LITERAL:
        return "STRING_LITERAL";
    case ERROR:
        return "ERROR";
    case LABEL_DEF:
        return "LABEL_DEF";
    case DIRECET:
        return "DIRECET";
    case COMMA:
        return "COMMA";
    case INDIRECT_REGISTER:
        return "INDIRECT_REGISTER";
    case UNKNOWN:
        return "UNKNOWN";
    default:
        return "UNDEFINED";
    }
}

#ifndef TOKENIZER_H
#define TOKENIZER_H
#define MAX_TOKENS 40
#define MAX_TOKEN_LENGTH 70
#define MAX_LABEL_LENGTH 31


typedef enum {
    /* Structural elements */
    EOL,            /* End of Line */
    LABEL,
    COLON,          /* : */
    COMMA,          /* , */

    /* Instructions */
    OPCODE,         /* mov, add, sub, etc. */

    /* Registers */
    REGISTER,       /* r0, r1, r2, etc. */

    /* Memory access */
    ASTERISK,       /* * (for indirect addressing) */

    /* Literals */
    INTEGER,        /* Immediate values */
    STRING,         /* String literals */

    /* Directives */
    DIRECTIVE,      /* .data, .string, .entry, .extern */

    /* Symbols */
    SYMBOL,         /* User-defined symbols */

    /* Special */
    HASH,           /* # (for immediate addressing) */
    PLUS,           /* + */
    MINUS,          /* - */

    /* Miscellaneous */
    COMMENT,        /* ; (start of a comment) */
    UNKNOWN,        /* For unrecognized tokens */
} TokenType;


typedef struct {
    TokenType type;
    char value[MAX_TOKEN_LENGTH];
} Token;


#endif
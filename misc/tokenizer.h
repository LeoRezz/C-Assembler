#ifndef TOKENIZER_H
#define TOKENIZER_H
#define MAX_TOKENS 40
#define MAX_TOKEN_LENGTH 70
#define MAX_LABEL_LENGTH 31


/* In tokenizer.h */
typedef enum {
    /* Registers */
    R0, R1, R2, R3, R4, R5, R6, R7,

    /* Opcodes (Instructions) */
    MOV, CMP, ADD, SUB, NOT, CLR, LEA,
    INC, DEC, JMP, BNE, RED, PRN, JSR, RTS, STOP,

    /* Directives */
    DATA, STRING, ENTRY, EXTERN,

    /* Special symbols */
    HASH,  /* For immediate addressing */
    ASTERISK,   /* For indirect addressing */

    /* Additional token types */
    LABEL_DEF,      /* Label definition (ends with ':') */
    LABEL_USE,      /* Label use (without ':') */
    COMMA,          /* ',' */
    INTEGER,
    STRING_LITERAL,

    EOL,  /* End of line */
    EOF,  /* End of file */

    UNKNOWN  /* For unrecognized tokens */
} TokenType;

/* Function prototype */
TokenType get_token_type(const char* token);


typedef struct {
    TokenType type;
    char value[MAX_TOKEN_LENGTH];
} Token;


#endif 
#ifndef TOKENIZER_H
#define TOKENIZER_H
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_LINE 82
#define MAX_TOKENS 40
#define MAX_TOKEN_LENGTH 70
#define MAX_LABEL_LENGTH 31
#define TRY(a)       \
    if (!(a)) {      \
        perror(#a);  \
        exit(1); \
    }



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
    ERROR, /* for blatant errors */
    UNKNOWN  /* For unrecognized tokens */
} TokenType;



typedef struct {
    TokenType type;
    char value[MAX_TOKEN_LENGTH];
} Token;

/* Function prototype */
TokenType get_token_type(const char* token);
Token *tokenize_line(const char *line, int *token_count);
TokenType get_token_type(const char *token);
const char *token_type_to_string(TokenType type);

#endif 
#ifndef TOKENIZER_H
#define TOKENIZER_H

/* TODO: Add brief description of the tokenizer's purpose and functionality */

#include "line.h"
#include "util.h"

/* Constants */
#define MAX_TOKENS 60
#define MAX_TOKEN_LENGTH 70
/* TODO: Document the purpose of each constant */

/* Enums and Structures */
typedef enum {

    /* Opcodes (Instructions) */
    MOV, CMP, ADD, SUB, LEA, CLR, NOT,
    INC, DEC, JMP, BNE, RED, PRN, JSR, RTS, STOP,

    /* Directives */
    DATA, STRING, ENTRY, EXTERN,
    MACRO_KEYWORD, ENDMACRO_KEYWORD,

    /* Special symbols */
    IMMEDIATE,  /* Label use (without ':') */
    DIRECET,  /* For immediate addressing */
    INDIRECT_REGISTER,   /* For indirect addressing */
    REGISTER,     /* For register operands, like: R0, R1 */

    /* Additional token types */
    LABEL_DEF,      /* Label definition (ends with ':') */      
    COMMA,          /* ',' */
    INTEGER,
    STRING_LITERAL,

    ERROR = - 2, /* for blatant errors */
    UNKNOWN = -1  /* For unrecognized tokens */
} TokenType;




typedef struct {
    TokenType type;
    char value[MAX_TOKEN_LENGTH];
} Token;


/* Function prototype */
Token *tokenize_line(const char *line, int *tokens_count, int current_line, int is_preprocess);
int is_reserved_word(const char *word);
#endif
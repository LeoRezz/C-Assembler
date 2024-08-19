#ifndef TOKENIZER_H
#define TOKENIZER_H

/* Tokenizes a given line of assembly code into tokens, and assigns their type. */


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

/* my_getword seprates a given line to meaningful tokens */
int my_getword(char *word, int lim, const char **line, int is_preprocess);

/* Checks if the given token is a valid immediate operand */
int is_valid_immediate(const char *token);

/* Checks if a given token is a valid indirect register */
int is_valid_indirect_register(const char *token);

/* Checks if a given token is a valid register */
int is_valid_register(const char *token);

/* Checks if a given token is a valid label */
int is_valid_label(char *label);

/* Tokenizes a given line of assembly code into tokens, and assigns their type. */
Token *tokenize_line(const char *line, int *tokens_count, int current_line, int is_preprocess);

/* Gets the type of a given token */
TokenType get_token_type(const char *token, int is_preprocess);

/* Setts all the token values to UNKNOWN for better error checking and parsing */
void init_tokens(Token *tokens);

/* Checks for reserved assembly language words */
int is_reserved_word(const char *word);
#endif
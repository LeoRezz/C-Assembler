#ifndef TOKENIZER_H
#define TOKENIZER_H

#include "util.h"
#define MAX_LINE 82
#define MAX_TOKENS 40
#define MAX_TOKEN_LENGTH 70
#define MAX_LABEL_LENGTH 31



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

typedef enum {
    LINE_INSTRUCTION,
    LINE_DATA,
    LINE_DIRECTIVE
} LineType;

typedef struct {
    TokenType type;
    char value[MAX_TOKEN_LENGTH];
} Token;

typedef struct {
    Token tokens[MAX_TOKENS];
    int num_of_tokens;
    int line_number;
    LineType type;
} TokenizedLine;

#define MAX_DATA_VALUES 70
#define MAX_SYMBOL_LENGTH 31

typedef struct Instruction {
    int address;
    int opcode;
    int operand_types[2];
    union {
        int immediate;
        char symbol[MAX_SYMBOL_LENGTH];
        int reg;
    } operands[2];
} Instruction;

typedef struct Data {
    int address;
    enum DataType {
        DATA_INT,
        DATA_STRING
    } type;
    int value_count;
    union {
        int int_values[MAX_DATA_VALUES];
        char char_values[MAX_DATA_VALUES];
    } content;
} Data;

typedef struct Line {
    LineType type;
    char label[MAX_LABEL_LENGTH];
    union {
        Instruction inst;
        Data data;
    } content;
} Line;

/* Function prototype */
TokenType get_token_type(const char* token);
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
Token *tokenize_line(const char *line, int *token_count);
TokenType get_token_type(const char *token);
const char *token_type_to_string(TokenType type);

#endif 
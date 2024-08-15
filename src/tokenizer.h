#ifndef TOKENIZER_H
#define TOKENIZER_H

#include "util.h"
#define MAX_LINE 82
#define MAX_TOKENS 40
#define MAX_TOKEN_LENGTH 70
#define MAX_LABEL_LENGTH 31
#define MAX_DATA_VALUES 70
#define MAX_SYMBOL_LENGTH 31


typedef enum {
    /* Registers */
    R0, R1, R2, R3, R4, R5, R6, R7,
    
    /* Opcodes (Instructions) */
    MOV, CMP, ADD, SUB, LEA, CLR, NOT,
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

    ERROR = - 2, /* for blatant errors */
    UNKNOWN = -1  /* For unrecognized tokens */
} TokenType;



typedef enum {
    LINE_INSTRUCTION,
    LINE_DATA,
    LINE_STRING,
    LINE_DIRECTIVE
} LineType;

typedef struct {
    TokenType type;
    char value[MAX_TOKEN_LENGTH];
} Token;

typedef struct {
    char symbol[MAX_SYMBOL_LENGTH]; /* Storing label opreand name, like: LOOP */
    int address;
    /* Label operands are assumed undefined and set to -1, To be updated in the second pass, using the ~LabelTable~ */
} LabelOperand;

typedef struct Instruction {
    int opcode;
    int operand_types[2]; /* Types of Addressing mode, should be intialized to -1 ? */
    int operands_count; /* For determining operand count */
    union {
        int immediate; /* Storing opreand's integer value  */
        LabelOperand label; /* Storing opreand's label name, and address field for resolution */
        int reg; /* Storing register's value, 0 to 7 */
    } operands[2];
} Instruction;

typedef struct Data {
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

/* Structure to represent a line of assembly code */
typedef struct Line {
    int line_number; /* For error reporting */
    int address; /* Current IC for output */
    LineType type;
    char label[MAX_LABEL_LENGTH]; /* Stores labels, currently used for debugging */
    union {
        Instruction inst; /* Instruction fields */
        Data data; /* Data fields */
    } content;
} Line;

/* Function prototype */

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
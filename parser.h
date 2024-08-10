#ifndef PARSER_H
#define PARSER_H
/* Gets a TokenizedLine object and goes through each token */
#include "opcodeTable.h"
#include "symbolTable.h"
#include "tokenizer.h" /* For TokenizedLine definition */
#include "util.h"

#define MAX_DATA_VALUES 70
#define MAX_LABEL_LENGTH 31

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
    enum LineType { LINE_INSTRUCTION,
                    LINE_DATA,
                    LINE_DIRECTIVE } type;
    char label[MAX_LABEL_LENGTH];
    union {
        Instruction inst;
        Data data;
    } content;
} Line;

/* Function prototypes */

Line *parse_line(TokenizedLine *tok_line, int *IC, int *DC);
Data create_data(TokenizedLine *tok_line, int *DC, int *i);
Instruction create_instruction(TokenizedLine *tok_line, int *IC);

/* Add other parsing-related function prototypes here */
#endif
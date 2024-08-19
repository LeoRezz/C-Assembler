#ifndef LINE_H
#define LINE_H

/* TODO: Add brief description of the Line structure and its purpose */
#include "util.h"
#include "tokenizer.h"

#define MAX_DATA_VALUES 70


typedef enum {
    LINE_INSTRUCTION,
    LINE_DATA,
    LINE_STRING,
    LINE_ENTRY,
    LINE_EXTERN
} LineType;

typedef struct Instruction {
    int opcode;
    int operand_types[2]; /* Types of Addressing mode, should be intialized to -1 ? */
    int operands_count; /* For determining operand count */
    union {
        int immediate; /* Storing opreand's integer value  */
        char symbol[MAX_SYMBOL_LENGTH]; /* Storing label opreand name, like: LOOP */
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


#endif 
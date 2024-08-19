#ifndef OPCODE_TABLE_H
#define OPCODE_TABLE_H

/* Addresing mode enum for each operand */
typedef enum {
    ADD_NONE = 0, /* 0000 */
    ADD_IMMEDIATE = 1, /* 0001 */
    ADD_DIRECT = 2, /* 0010 */
    ADD_INDIRECT_REGISTER = 4, /* 0100 */
    ADD_REGISTER = 8  /* 1000 */
} AddressingMode;
 

typedef struct {
    const char *mnemonic; /* Textual representation of the opcode */
    int opcode; /* The opcode's numeric value */
    int operands; /* Number of operands allowed */
    int src_modes; /* Addersing modes allowed for source operand, if exists */
    int dest_modes; /* Addersing modes allowed for destination operand, if exists */
} Opcode;

/* Function prototypes */

/* Finds an opcode by its name given as a parameter */
const Opcode *find_opcode(const char *mnemonic);
/* Calculates the instruction count of a given opcode and addressing modes */
int calculate_word_count(const Opcode *op, AddressingMode src_mode, AddressingMode dest_mode);
/* Checks if given addressing mode combinations are allowed */
int is_addressing_mode_allowed(int allowed_modes, AddressingMode mode);
#endif
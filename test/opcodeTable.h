#ifndef OPCODE_TABLE_H
#define OPCODE_TABLE_H


typedef enum {
    ADD_NONE = 0b0000, /* 0000 */
    ADD_IMMEDIATE = 0b0001, /* 0001 */
    ADD_DIRECT = 0b0010, /* 0010 */
    ADD_INDIRECT_REGISTER = 0b0100, /* 0100 */
    ADD_REGISTER = 0b1000  /* 1000 */
} AddressingMode;
 

typedef struct {
    const char *mnemonic; /* Textual representation of the opcode */
    int opcode; /* The opcode's numeric value */
    int operands; /* Number of operands allowed */
    int src_modes; /* Addersing modes allowed for source operand, if exists */
    int dest_modes; /* Addersing modes allowed for destination operand, if exists */
} Opcode;

const Opcode *find_opcode(const char *mnemonic);
int calculate_word_count(const Opcode *op, AddressingMode src_mode, AddressingMode dest_mode);
int is_addressing_mode_allowed(int allowed_modes, AddressingMode mode);
const char *addressing_mode_to_string(AddressingMode mode);
void print_allowed_modes(int modes);
void print_opcode_info(const Opcode *op);
void print_opcode_table();
const char *opcode_to_string(int opcode);

#endif
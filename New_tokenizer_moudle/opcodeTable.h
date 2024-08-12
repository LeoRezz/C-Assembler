#ifndef OPCODE_TABLE_H
#define OPCODE_TABLE_H

typedef enum {
    ADD_IMMEDIATE,
    ADD_DIRECT,
    ADD_INDIRECT_REGISTER,
    ADD_REGISTER
} AddressingMode;

typedef struct {
    const char *mnemonic;
    int opcode;
    int operands;
    int src_modes;
    int dest_modes;
} Opcode;

const Opcode *find_opcode(const char *mnemonic);
int is_addressing_mode_allowed(int allowed_modes, AddressingMode mode);
const char *addressing_mode_to_string(AddressingMode mode);
void print_allowed_modes(int modes);
void print_opcode_info(const Opcode *op);
void print_opcode_table();

#endif // OPCODE_TABLE_H
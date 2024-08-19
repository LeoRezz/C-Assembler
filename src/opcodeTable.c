#include "opcodeTable.h"
#include "util.h"


#define NUM_OPCODES 16

/*
 * OPCODE_TABLE: A static constant array containing information about opcodes.
 *
 * Each entry in the array represents an opcode that consists of:
 * 1. Mnemonic (string): The name of the opcode.
 * 2. Opcode value (int): Numeric representation of the opcode.
 * 3. Number of operands (int): How many operands the instruction takes.
 * 4. Source addressing modes (int): Allowed addressing modes for the source operand.
 * 5. Destination addressing modes (int): Allowed addressing modes for the destination operand.
 *
 * Addressing modes are represented as bitwise combinations of:
 * ADD_IMMEDIATE, ADD_DIRECT, ADD_INDIRECT_REGISTER, and ADD_REGISTER.
 *
 *
 * This table is used to validate instructions, determine operand counts,
 * and check addressing mode validity during the assembly process.
 */
 const Opcode OPCODE_TABLE[NUM_OPCODES] = {
    {"mov", 0, 2, ADD_IMMEDIATE | ADD_DIRECT | ADD_INDIRECT_REGISTER | ADD_REGISTER, ADD_DIRECT | ADD_INDIRECT_REGISTER | ADD_REGISTER},
    {"cmp", 1, 2, ADD_IMMEDIATE | ADD_DIRECT | ADD_INDIRECT_REGISTER | ADD_REGISTER, ADD_IMMEDIATE | ADD_DIRECT | ADD_INDIRECT_REGISTER | ADD_REGISTER},
    {"add", 2, 2, ADD_IMMEDIATE | ADD_DIRECT | ADD_INDIRECT_REGISTER | ADD_REGISTER, ADD_DIRECT | ADD_INDIRECT_REGISTER | ADD_REGISTER},
    {"sub", 3, 2, ADD_IMMEDIATE | ADD_DIRECT | ADD_INDIRECT_REGISTER | ADD_REGISTER, ADD_DIRECT | ADD_INDIRECT_REGISTER | ADD_REGISTER},
    {"lea", 4, 2, ADD_DIRECT | ADD_INDIRECT_REGISTER, ADD_DIRECT | ADD_INDIRECT_REGISTER | ADD_REGISTER},
    {"clr", 5, 1, 0, ADD_DIRECT | ADD_INDIRECT_REGISTER | ADD_REGISTER},
    {"not", 6, 1, 0, ADD_DIRECT | ADD_INDIRECT_REGISTER | ADD_REGISTER},
    {"inc", 7, 1, 0, ADD_DIRECT | ADD_INDIRECT_REGISTER | ADD_REGISTER},
    {"dec", 8, 1, 0, ADD_DIRECT | ADD_INDIRECT_REGISTER | ADD_REGISTER},
    {"jmp", 9, 1, 0, ADD_DIRECT | ADD_INDIRECT_REGISTER},
    {"bne", 10, 1, 0, ADD_DIRECT | ADD_INDIRECT_REGISTER},
    {"red", 11, 1, 0, ADD_DIRECT | ADD_INDIRECT_REGISTER | ADD_REGISTER},
    {"prn", 12, 1, 0, ADD_IMMEDIATE | ADD_DIRECT | ADD_INDIRECT_REGISTER | ADD_REGISTER},
    {"jsr", 13, 1, 0, ADD_DIRECT | ADD_INDIRECT_REGISTER},
    {"rts", 14, 0, 0, 0},
    {"stop", 15, 0, 0, 0}};

/* Finds an opcode by its name given as a parameter */
const Opcode *find_opcode(const char *mnemonic) {
    int i;
    for (i = 0; i < NUM_OPCODES; i++) {
        if (strcmp(OPCODE_TABLE[i].mnemonic, mnemonic) == 0) {
            return &OPCODE_TABLE[i];
        }
    }
    return NULL; /* Opcode not found */
}
/* Checks if an addressing mode is allowed */
int is_addressing_mode_allowed(int allowed_modes, AddressingMode mode) {
    return (allowed_modes & mode) != 0;
}


/* Calculate_word_count:
 * Calculate the number of words required for an instruction.
 */
int calculate_word_count(const Opcode *op, AddressingMode src_mode, AddressingMode dest_mode) {
    int count = 1;  /* Start with 1 for the instruction word */

    switch (op->operands) {
        case 0:
            return count;  /* Only the instruction word */
        
        case 1:
            /* For single operand instructions, always add one word for the operand */
            return count + 1;
        
        case 2:
        /* For two operand instructions */
        if ((src_mode & (ADD_REGISTER | ADD_INDIRECT_REGISTER)) && (dest_mode & (ADD_REGISTER | ADD_INDIRECT_REGISTER))) { /* (1000 & 1100) != 0 */
            return count + 1; /* Instruction word + one word for both registers */
            } else {
                /* Add a word for each operand */
                count += 1;  /* For source operand */
                count += 1;  /* For destination operand */
                return count;
            }
        
        default:
            /* This shouldn't happen with your instruction set, but good to handle */
            printf ("Error: Invalid number of operands\n");
            return -1;  /* Indicate an error */
    }
}
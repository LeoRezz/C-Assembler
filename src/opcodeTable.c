#include "opcodeTable.h"
#include "util.h"


#define NUM_OPCODES 16

/*
 * OPCODE_TABLE: A static constant array containing information about opcodes.
 *
 * Each entry in the array represents an opcode and consists of:
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
 * and check addressing mode validity during assembly or simulation.
 */
static const Opcode OPCODE_TABLE[NUM_OPCODES] = {
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

const Opcode *find_opcode(const char *mnemonic) {
    int i;
    for (i = 0; i < NUM_OPCODES; i++) {
        if (strcmp(OPCODE_TABLE[i].mnemonic, mnemonic) == 0) {
            return &OPCODE_TABLE[i];
        }
    }
    return NULL; /* Opcode not found */
}

int is_addressing_mode_allowed(int allowed_modes, AddressingMode mode) {
    return (allowed_modes & mode) != 0;
}


/* calculate_word_count:
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
            if ((src_mode == ADD_REGISTER) && (dest_mode == ADD_REGISTER)) {
                return count + 1;  /* Instruction word + one word for both registers */
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


const char *addressing_mode_to_string(AddressingMode mode) {
    switch (mode) {
    case ADD_IMMEDIATE:
        return "Immediate";
    case ADD_DIRECT:
        return "Direct";
    case ADD_INDIRECT_REGISTER:
        return "Indirect Register";
    case ADD_REGISTER:
        return "Register";
    default:
        return "Unknown";
    }
}

void print_allowed_modes(int modes) {
    printf("Allowed modes: ");
    if (modes & ADD_IMMEDIATE)
        printf("Immediate ");
    if (modes & ADD_DIRECT)
        printf("Direct ");
    if (modes & ADD_INDIRECT_REGISTER)
        printf("Indirect Register ");
    if (modes & ADD_REGISTER)
        printf("Register ");
    printf("\n");
}

void print_opcode_info(const Opcode *op) {
    if (op == NULL) {
        printf("Invalid opcode\n");
        return;
    }

    printf("Mnemonic: %s\n", op->mnemonic);
    printf("Opcode: %d\n", op->opcode);
    printf("Number of operands: %d\n", op->operands);

    if (op->operands > 0) {
        printf("Source operand: ");
        print_allowed_modes(op->src_modes);
    }
    if (op->operands > 1) {
        printf("Destination operand: ");
        print_allowed_modes(op->dest_modes);
    }
}

void print_opcode_table() {
    int i;
    printf("Opcode Table:\n");
    printf("%-6s %-5s %-8s %-20s %-20s\n", "Mnem.", "Code", "Operands", "Src Modes", "Dest Modes");
    printf("------ ----- -------- -------------------- --------------------\n");

    for (i = 0; i < NUM_OPCODES; i++) {
        printf("%-6s %-5d %-8d ",
               OPCODE_TABLE[i].mnemonic,
               OPCODE_TABLE[i].opcode,
               OPCODE_TABLE[i].operands);

        print_allowed_modes(OPCODE_TABLE[i].src_modes);
        print_allowed_modes(OPCODE_TABLE[i].dest_modes);
        printf("\n");
    }
}

const char *opcode_to_string(int opcode) {
    int i;
    for (i = 0; i < NUM_OPCODES; i++) {
        if (OPCODE_TABLE[i].opcode == opcode) {
            return OPCODE_TABLE[i].mnemonic;
        }
    }
    return "Invalid opcode";
}

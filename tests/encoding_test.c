#include "tokenizer.h"

static enum {
    ADD_IMMEDIATE,
    ADD_DIRECT,
    ADD_INDIRECT_REGISTER,
    ADD_REGISTER
} AddressingMode;

int main() {
    Line line1 = {  /* MAIN:    add #5, r3 */
        .address = 100,
        .type = LINE_INSTRUCTION,
        .label = "MAIN",
        .content.inst = {
            .opcode = ADD,
            .operand_types = {ADD_IMMEDIATE, ADD_REGISTER},
            .operands = {{.immediate = 5}, {.reg = 3}}
        }
    };
    Line line2 = {  /* LOOP:    mov r2, STR */
        .address = 103,
        .type = LINE_INSTRUCTION,
        .label = "LOOP",
        .content.inst = {
            .opcode = MOV,
            .operand_types = {ADD_REGISTER, ADD_DIRECT},
            .operands = {{.reg = 2}, {.symbol = "STR"}}
        }
    };
    Line line3 = {  /* DATA:    .data 6, -9, 15 */
        .type = LINE_DATA,
        .label = "DATA",
        .type = LINE_DATA,
        .content.data = {
            .type = DATA_INT,
            .value_count = 3,
            .content.int_values = {6, -9, 15}
        }
    };
    
    return 0;
}

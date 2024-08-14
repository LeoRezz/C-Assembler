#include "../src/tokenizer.h"
#include "../src/parser.h"
#include "../src/opcodeTable.h"
#include "../src/util.h"
#include "../src/symbolTable.h"

int IC = 100;
int DC = 0;

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

    print_parsed_line(&line1);
    print_parsed_line(&line2);
    print_parsed_line(&line3);
    
    return 0;
}

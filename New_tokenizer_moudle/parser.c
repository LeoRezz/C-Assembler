#include "parser.h"
extern int IC, DC, current_line;
int get_addressing_type(TokenType type);
int calculate_instruction_words(int opcode);

void print_parsed_line(Line *parsed_line);

Line *parse_line(Token *token_arr, int token_count) {
    int i;
    int instruction_length = 0;
    int label_definition = 0;
    i = 0;
    Line *parsed_line = calloc(1, sizeof(Line));
    if (!parsed_line) {
        // Handle memory allocation error
        return NULL;
    }

    /* Check for label */
    if (token_arr[0].type == LABEL_DEF) {
        label_definition = 1;
        strcpy(parsed_line->label, token_arr[0].value);
        i++;
    }

    /* TODO: Syntax error handling */
    /* Checks for instruction */
    if (i < token_count) {
        /* Checks first token for an opcode */
        if (token_arr[i].type >= MOV && token_arr[i].type <= STOP) {
            parsed_line->type = LINE_INSTRUCTION;
            parsed_line->content.inst.opcode = ((token_arr[i].type) - MOV);
            i++;
            parsed_line->content.inst.operand_types[0] = get_addressing_type(token_arr[i].type);
            i + 2; /* Skip comma */
            parsed_line->content.inst.operand_types[1] = get_addressing_type(token_arr[i++].type);

            if (label_definition) {
                addSymbol(parsed_line->label, &IC, parsed_line->type);
                label_definition = 0;
            }
            instruction_length = calculate_instruction_words(parsed_line->content.inst.opcode);
            IC += instruction_length + 1;
        }
    }

    /* Check for data */
    return parsed_line;
}

int get_addressing_type(TokenType type) {

    if (type == HASH) {
        return ADD_IMMEDIATE;
    }

    if (type == LABEL_USE) {
        return ADD_DIRECT;
    }

    if (type == ASTERISK) {
        return ADD_INDIRECT_REGISTER;
    }

    if (type >= R0 && type <= R7) {
        return ADD_REGISTER;
    }
}
int calculate_instruction_words(int opcode) {
    if (opcode + MOV >= MOV && opcode + MOV <= LEA) {
        return 2; // Two operand instructions
    }

    if (opcode + MOV >= CLR && opcode + MOV <= JSR) {
        return 1; // One operand instructions
    }

    if (opcode + MOV == RTS || opcode + MOV == STOP) {
        return 0; // Zero operand instructions
    }

    return -1; // Unexpected token type
}

void print_parsed_line(Line *parsed_line) {
    const char *line_type_to_string(LineType line_type);
    const char *operand_type_to_string(int operand_type);
    const char *opcode_to_string(int opcode);
    int i;
    printf("Line type: %s\n", line_type_to_string(parsed_line->type));
    printf("Label: %s\n", parsed_line->label);

    if (parsed_line->type == LINE_INSTRUCTION) {
        printf("Instruction: %s\n", opcode_to_string(parsed_line->content.inst.opcode));
        for (i = 0; i < 2; i++) {
            printf("Operand %d type: %s\n", i, operand_type_to_string(parsed_line->content.inst.operand_types[i]));
            if (parsed_line->content.inst.operand_types[i] == ADD_IMMEDIATE) {
                printf("Operand %d immediate value: %d\n", i, parsed_line->content.inst.operands[i].immediate);
            } else if (parsed_line->content.inst.operand_types[i] == ADD_REGISTER) {
                printf("Operand %d register: %d\n", i, parsed_line->content.inst.operands[i].reg);
            } else if (parsed_line->content.inst.operand_types[i] == ADD_DIRECT) {
                printf("Operand %d symbol: %s\n", i, parsed_line->content.inst.operands[i].symbol);
            }
        }
    }
}











const char *line_type_to_string(LineType line_type) {
    switch (line_type) {
    case LINE_INSTRUCTION:
        return "INSTRUCTION";
    case LINE_DATA:
        return "DATA";
    case LINE_DIRECTIVE:
        return "DIRECTIVE";
    default:
        /* Handle unexpected line type */
        return "UNKNOWN";
    }
}
const char *operand_type_to_string(int operand_type) {
    switch (operand_type) {
    case ADD_IMMEDIATE:
        return "IMMEDIATE";
    case ADD_DIRECT:
        return "DIRECT";
    case ADD_INDIRECT_REGISTER:
        return "INDIRECT REGISTER";
    case ADD_REGISTER:
        return "REGISTER";
    default:
        return "UNKNOWN OPERAND TYPE";
    }
}

const char *opcode_to_string(int opcode) {
    switch (opcode + MOV) {
    case MOV:
        return "MOV";
    case CMP:
        return "CMP";
    case ADD:
        return "ADD";
    case SUB:
        return "SUB";
    case LEA:
        return "LEA";
    case CLR:
        return "CLR";
    case NOT:
        return "NOT";
    case INC:
        return "INC";
    case DEC:
        return "DEC";
    case JMP:
        return "JMP";
    case BNE:
        return "BNE";
    case RED:
        return "RED";
    case PRN:
        return "PRN";
    case JSR:
        return "JSR";
    case RTS:
        return "RTS";
    case STOP:
        return "STOP";
    default:
        return "UNKNOWN";
    }
}
#include "parser.h"
extern int IC, DC, current_line;
int get_addressing_type(TokenType type);
int calculate_instruction_words(int opcode);
Line *parse_line(Token *token_arr, int token_count) {
    int i;
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

    /* Check for instruction or directive */
    if (i < token_count) {
        if (token_arr[i].type >= MOV && token_arr[i].type <= STOP) {
            parsed_line->type = LINE_INSTRUCTION;
            parsed_line->content.inst.opcode = ((token_arr[i].type) - MOV);
            i++;
            parsed_line->content.inst.operand_types[0] = get_addressing_type(token_arr[i].type);
            i + 2; /* Skip comma */
            parsed_line->content.inst.operand_types[1] = get_addressing_type(token_arr[i++].type);
        }

        if (label_definition) {
            addSymbol(parsed_line->label, &IC, parsed_line->type);
            label_definition = 0;
        }
        int instruction_length = calculate_instruction_words(parsed_line->content.inst.opcode);
        IC += instruction_length + 1;
    }

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
    switch (opcode + MOV) {
    case MOV:
    case CMP:
    case ADD:
    case SUB:
    case LEA:
        /* code for two-operand instructions */
        return 2;  // Assuming these instructions always use 2 words
    
    case CLR:
    case NOT:
    case INC:
    case DEC:
    case JMP:
    case BNE:
    case RED:
    case PRN:
    case JSR:
        /* code for one-operand instructions */
        return 1;  // Assuming these instructions always use 1 word
    
    case RTS:
    case STOP:
        /* code for zero-operand instructions */
        return 0;  // These instructions don't use additional words
    
    default:
        /* Handle unexpected token type */
        return -1;  // Return an error code
    }
}
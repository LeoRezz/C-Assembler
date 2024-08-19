#include "parser.h"
#include "memory_manager.h"
#define PARSE(a)                   \
    if (!(a)) {                    \
        parsed_line->type = ERROR; \
        return parsed_line;        \
    }

/* Helper functions for parsing instructions and data */
int get_addressing_type(Token token);
int is_data(TokenType type);
int parse_instruction_line(Line *parsed_line, Token *token_arr, int *i, const Opcode *op,int tok_count);
int add_operand_value(Line *parsed_line, Token *token_arr, int *i, int op_index);
int parse_data_line(Line *parsed_line, Token *token_arr, int *current_token, int *data_count);

Line *parse_line(Token *token_arr, int token_count, int current_line) {
    const Opcode *opcode;
    int word_count;
    int data_count;
    int current_token;
    int label_def_flag;
    Line *parsed_line;
    int tok_len;

    TRY(parsed_line = calloc(1, sizeof(Line)));
    parsed_line->line_number = current_line;
    data_count = 0;
    current_token = 0;
    label_def_flag = 0;

    /* Check for label */
    if (token_arr[current_token].type == LABEL_DEF) {
        tok_len = strlen(token_arr[current_token].value);
        if(tok_len >= MAX_LABEL_LENGTH) {
            printf("Error in line %d: Label too long\n", parsed_line->line_number);
            parsed_line->type = ERROR;
            return parsed_line;
        }
        token_count--;
        strcpy(parsed_line->label, token_arr[current_token].value);
        label_def_flag = 1;
        current_token++; /* Skips label definition */
    }

    opcode = find_opcode(token_arr[current_token].value);
    /* Checks if it's an instruction line */
    if (opcode != NULL) {
        token_count--;
        PARSE(parse_instruction_line(parsed_line, token_arr, &current_token, opcode , token_count));
        if (label_def_flag) {
            if(!add_symbol(parsed_line->label, get_IC(), SYMBOL_CODE)){
                printf("Error in line %d: Symbol '%s' already exists\n", parsed_line->line_number ,parsed_line->label);
                parsed_line->type = ERROR;
                return parsed_line;
            }
        }
        word_count = calculate_word_count(opcode, parsed_line->content.inst.operand_types[0], parsed_line->content.inst.operand_types[1]);

        parsed_line->address = get_IC();
        increment_IC(word_count);

        /* TODO: Error checking */
        return parsed_line;
    }



    /* Check for data */
    if (is_data(token_arr[current_token].type)) {
        /* TODO: DATA value count BUGGED at zero */
        PARSE (parse_data_line(parsed_line, token_arr, &current_token, &data_count));
        if (label_def_flag) {
            if(!add_symbol(parsed_line->label, get_DC(), SYMBOL_DATA)){
                printf("Error in line %d: Symbol '%s' already exists\n", parsed_line->line_number ,parsed_line->label);
                parsed_line->type = ERROR;
                return parsed_line;
            }
        }
        parsed_line->address = get_DC();
        increment_DC(data_count);

        /* TODO: Error checking */

        return parsed_line;
    }
    /* Check for entry or extern line */
    if (token_arr[current_token].type == ENTRY || token_arr[current_token].type == EXTERN) {
        if (!is_valid_label(token_arr[current_token + 1].value)) {
            printf("Error in line %d: Invalid label '%s'\n", parsed_line->line_number, token_arr[current_token + 1].value);
            parsed_line->type = ERROR;
            return parsed_line;
        }
        switch (token_arr[current_token].type) {
        case ENTRY:
            parsed_line->type = LINE_ENTRY;
            current_token++;
                strncpy(parsed_line->label, token_arr[current_token].value, MAX_LABEL_LENGTH);
                if (!add_symbol(token_arr[current_token].value, 0, SYMBOL_ENTRY)) {
                    printf("Error in line %d: Symbol '%s' already exists\n", parsed_line->line_number, token_arr[current_token].value);
                    parsed_line->type = ERROR;
                    return parsed_line;
                }
            
            break;
        case EXTERN:
            parsed_line->type = LINE_EXTERN;
            current_token++;
            strncpy(parsed_line->label, token_arr[current_token].value, MAX_LABEL_LENGTH);
            if(!add_symbol(token_arr[current_token].value, 0, SYMBOL_EXTERNAL)){
                printf("Error in line %d: Symbol '%s' already exists\n", parsed_line->line_number, token_arr[current_token].value);
                parsed_line->type = ERROR;
                return parsed_line;
            }
            break;
        default:
            printf("Error in line %d: Invalid directive '%s'\n", parsed_line->line_number, token_arr[current_token].value);
            parsed_line->type = ERROR;
            return parsed_line;
        
        }
        return parsed_line;
    }

    /* Not a valid label definition */
    printf("Error in line %d: Missing colon '%s'\n", parsed_line->line_number, token_arr[current_token].value);
    
    parsed_line->type = ERROR;
    return parsed_line;
}

int parse_data_line(Line *parsed_line, Token *token_arr, int *current_token, int *data_count) {
    int comma;
    Data *data;
    char *p;

    data = &(parsed_line->content.data);

    /* Check the type of data */
    switch (token_arr[*current_token].type) { /* .data 6 , 5 = DATA */
    case DATA:
        parsed_line->type = LINE_DATA;
        /* Parse the integer data and update DC */
        data->type = DATA_INT; 
        (*current_token)++;    /* Advancing to first integer */
        comma = 0;
        if (token_arr[*current_token].type != INTEGER) {
            printf("Error in line %d: .data directive requires an integer\n", parsed_line->line_number);
            return 0;
        }

        for (; (token_arr[*current_token].type != UNKNOWN); (*current_token)++) {
            if ((token_arr[*current_token].type == COMMA) && (token_arr[(*current_token) + 1].type != UNKNOWN)) {
                comma++;
                continue;
            }

            if ((token_arr[*current_token].type == INTEGER) && comma == 0) {
                data->content.int_values[data->value_count++] = atoi(token_arr[*current_token].value);
                (*data_count)++;
                comma--;
            } else {
                printf("Error in line %d: Invalid comma usage near '%s'\n", parsed_line->line_number, token_arr[*current_token].value);
                return 0;
            }
        }
        break;

    case STRING:
        parsed_line->type = LINE_STRING;
        /* Parse the string data and update DC */
        data->type = DATA_STRING;
        (*current_token)++; /* Advancing to string litral*/

        if (token_arr[*current_token].type == STRING_LITERAL) {

            strncpy(data->content.char_values, (token_arr[*current_token].value) + 1, MAX_DATA_VALUES);
        } else {
            printf("Error in line %d: .string directive requires a string enclosed in quotes\n", parsed_line->line_number);
            return 0;
        }

        if ((p = strrchr(data->content.char_values, '"')) != NULL) {
            *p = '\0';
        } else {
            printf("Error in line %d: invalid string literal\n", parsed_line->line_number);
            return 0;
        }
        /* +1 for the null character */
        data->value_count = strlen(data->content.char_values) + 1;
        (*data_count) = data->value_count;

        break;

    default:
        printf("Error in line %d: Invalid token type for data line: %s\n", parsed_line->line_number, token_arr[*current_token].value);
        return 0;
    }
    return (*data_count);
}
int parse_instruction_line(Line *parsed_line, Token *token_arr, int *current_token, const Opcode *opcode, int token_count ) {
    int i;
    int addressing_mode;
    Instruction *inst;
    enum{ZERO_OPERAND = 0 ,ONE_OPERAND = 1, TWO_OPERAND = 2 };

    i = 0;
    inst = &(parsed_line->content.inst);
    parsed_line->type = LINE_INSTRUCTION;
    inst->opcode = opcode->opcode;
    (*current_token)++;

    inst->operands_count = opcode->operands;

    switch (opcode->operands) {
    case ONE_OPERAND:
        addressing_mode = get_addressing_type(token_arr[*current_token]);
        if (!is_addressing_mode_allowed(opcode->dest_modes, addressing_mode)) {
            printf("Error in line %d: Invalid addressing mode for destination operand\n", parsed_line->line_number);
            return 0;
        }
        if (!add_operand_value(parsed_line, token_arr, current_token, i)) {
            printf("Error in line %d: Invalid operand value\n", parsed_line->line_number);
            return 0;
        }
        (*current_token)++;
        break;

    case TWO_OPERAND:
        if(token_count < 2){
            printf("Error in line %d: Missing operandsfor instruction '%s' \n", parsed_line->line_number, opcode->mnemonic);
            return 0;
        }
        for (i = 0; i < opcode->operands; i++) {
            /* Skip Comma */
            if (i == 1) {
                /* Check for comma between operands */
                if (token_arr[*current_token].type != COMMA) {
                    printf("Error in line %d: Missing comma between operands\n", parsed_line->line_number);
                    return 0;
                }
                (*current_token)++;
            }
            addressing_mode = get_addressing_type(token_arr[*current_token]);

            if (i == 0 && !is_addressing_mode_allowed(opcode->src_modes, addressing_mode)) {
                printf("Error in line %d: Invalid addressing mode for source operand\n", parsed_line->line_number);
                return 0;
            }
            if (i == 1 && !is_addressing_mode_allowed(opcode->dest_modes, addressing_mode)) {
                printf("Error in line %d: Invalid addressing mode for destination operand\n", parsed_line->line_number);
                return 0;
            }

            if (!add_operand_value(parsed_line, token_arr, current_token, i)) {
                printf("Error in line %d: Invalid operand value\n", parsed_line->line_number);
                return 0;
            }

            (*current_token)++;
        }
        break;

    case ZERO_OPERAND:

        break;
    }
    if (token_arr[*current_token].type != UNKNOWN) {
        printf("Error in line %d: Too many operands for instruction '%s'\n", parsed_line->line_number, opcode->mnemonic);
        return 0;
    }

    return 1;
}

int add_operand_value(Line *parsed_line, Token *token_arr, int *current_index, int op_index) {
    AddressingMode mode;
    int immediate;
    Instruction *inst = &(parsed_line->content.inst);
    immediate = 0;

    switch (token_arr[*current_index].type) {
    case IMMEDIATE:
        mode = ADD_IMMEDIATE;
        immediate = atoi(token_arr[*current_index].value + 1);
        if (immediate > MAX_IMMEDIATE_VALUE || immediate < MIN_IMMEDIATE_VALUE) {
            printf("Error in line %d: Immediate value '%s' out of range (must be between -2048 and 2047)\n", parsed_line->line_number ,token_arr[*current_index].value);
            return 0;
        } else
            inst->operands[op_index].immediate = immediate;
        break;

    case DIRECET:
        mode = ADD_DIRECT;
        strncpy(inst->operands[op_index].symbol, token_arr[*current_index].value, MAX_LABEL_LENGTH);
        break;
    case INDIRECT_REGISTER:
        mode = ADD_INDIRECT_REGISTER;
        inst->operands[op_index].reg = token_arr[*current_index].value[2] - '0';
        break;

    case REGISTER:
        mode = ADD_REGISTER;
        inst->operands[op_index].reg = token_arr[*current_index].value[1] - '0';
        break;
    default:
        printf("Error: Invalid operand %s\n", token_arr[*current_index].value);
        return 0;
    }

    inst->operand_types[op_index] = mode;
    
    return 1;
}

int get_addressing_type(Token token) {
    TokenType type = token.type;
    switch (type) {

    case IMMEDIATE:
        return ADD_IMMEDIATE;
    
    case DIRECET:
        return ADD_DIRECT;
   
    case INDIRECT_REGISTER:
        return ADD_INDIRECT_REGISTER;
   
    case REGISTER:
        return ADD_REGISTER;

    default:
        return ERROR;
        break;
    }
}

int is_data(TokenType type) { return (type == DATA || type == STRING); }

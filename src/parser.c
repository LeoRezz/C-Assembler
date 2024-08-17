#include "parser.h"
#include "memory_manager.h"
#define PARSE(a)           \
    if (!(a)) {            \
        free(parsed_line); \
        return NULL;       \
    }


/* Helper functions for parsing instructions and data */
int get_addressing_type(Token token);
int is_data(TokenType type);
int parse_instruction_line(Line *parsed_line, Token *token_arr, int *i, const Opcode *op,int tok_count);
int add_operand_value(Instruction *inst, Token *token_arr, int *i, int op_index);
int parse_data_line(Line *parsed_line, Token *token_arr, int *current_token, int *data_count);

/*------------------------------------------------------------------------*/
/* TODO: Fix the bug at the instruction parsing logic */
/* TODO: Check data parsing logic for errors */
/* TODO: More robust error handling */
/* TODO: Consider adding a function to validate the entire instruction (opcode + operands) in one go.
         To simplify the parse_instruction_line function. */
/* TODO: Consider adding a function to validate the entire data */
/*------------------------------------------------------------------------*/
Line *parse_line(Token *token_arr, int token_count, int current_line) {
    const Opcode *opcode;
    int word_count;
    int operand_count;
    int data_count;
    int current_token;
    int label_def_flag;
    Line *parsed_line;

    TRY(parsed_line = calloc(1, sizeof(Line)));
    parsed_line->line_number = current_line;
    data_count = 0; /* NOT IN USE */
    current_token = 0;
    label_def_flag = 0;

    /* Check for label */
    if (token_arr[current_token].type == LABEL_DEF) {
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
            add_symbol(parsed_line->label, get_IC(), SYMBOL_CODE);
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
            add_symbol(parsed_line->label, get_DC(), SYMBOL_DATA);
        }
        parsed_line->address = get_DC();
        increment_DC(data_count);

        /* TODO: Error checking */

        return parsed_line;
    }
    /* Check for entry or extern line */
    if (token_arr[current_token].type == ENTRY || token_arr[current_token].type == EXTERN) {
        switch (token_arr[current_token].type) {
        case ENTRY:
            parsed_line->type = LINE_ENTRY;
            current_token++;
            strncpy(parsed_line->label, token_arr[current_token].value, MAX_LABEL_LENGTH);
            add_symbol(token_arr[current_token].value, 0, SYMBOL_ENTRY);
            break;
        case EXTERN:
            parsed_line->type = LINE_EXTERN;
            current_token++;
            strncpy(parsed_line->label, token_arr[current_token].value, MAX_LABEL_LENGTH);
            add_symbol(token_arr[current_token].value, 0, SYMBOL_EXTERNAL);
            break;
        }
        return parsed_line;
    }

    /* Error */
    printf("EROOR: Expected Opcode or directive instead of '%s'\n", token_arr[current_token].value);
    free(parsed_line);
    return NULL;
}

int parse_data_line(Line *parsed_line, Token *token_arr, int *current_token, int *data_count) {
    int token_count;
    int is_even;
    Data *data;
    char *p;
    printf("parsing data line\n");

    is_even = *current_token;
    data = &(parsed_line->content.data);

    /* Check the type of data */
    switch (token_arr[*current_token].type) { /* .data 6 , 5 = DATA */
    case DATA:
        parsed_line->type = LINE_DATA;
        /* Parse the integer data and update DC */
        data->type = DATA_INT; 
        (*current_token)++;    /* Advancing to first integer */
        int comma = 0;
        /* TODO: figure a way to handle missing or extranouse commas errors */
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
                printf("Error: Invalid comma usage or missing comma between integer values near '%s'\n", token_arr[*current_token].value);                
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
            printf("Invalid string literal\n");
            return 0;
        }
        /* +1 for the null character */
        data->value_count = strlen(data->content.char_values) + 1;
        (*data_count) = data->value_count;

        break;

    default:
        printf("Invalid token type for data line: %s\n", token_arr[*current_token].value);
        return 0;
    }
    return (*data_count);
}
int parse_instruction_line(Line *parsed_line, Token *token_arr, int *current_token, const Opcode *opcode, int token_count) {
    int i;
    int addressing_mode;
    Instruction *inst;
    enum{ZERO_OPERAND = 0 ,ONE_OPERAND = 1, TWO_OPERAND = 2 };

    inst = &(parsed_line->content.inst);
    parsed_line->type = LINE_INSTRUCTION;
    inst->opcode = opcode->opcode;
    (*current_token)++;

    inst->operands_count = opcode->operands;

    

    

    switch (opcode->operands) {
    case ONE_OPERAND:
        addressing_mode = get_addressing_type(token_arr[*current_token]);
        if (!is_addressing_mode_allowed(opcode->dest_modes, addressing_mode)) {
            printf("Error: Invalid addressing mode for destination operand\n");
            return 0;
        }
        if (!add_operand_value(&parsed_line->content.inst, token_arr, current_token, i)) {
            printf("Error: Invalid operand value\n");
            return 0;
        }
        (*current_token)++;
        break;

    case TWO_OPERAND:

        for (i = 0; i < opcode->operands; i++) {
            /* Skip Comma */
            if (i == 1) {
                /* Check for comma between operands */
                if (token_arr[*current_token].type != COMMA) {
                    printf("Error: Missing comma between operands\n");
                    return 0;
                }
                (*current_token)++;
            }
            addressing_mode = get_addressing_type(token_arr[*current_token]);

            if (i == 0 && !is_addressing_mode_allowed(opcode->src_modes, addressing_mode)) {
                printf("Error: Invalid addressing mode for source operand\n");
                return 0;
            }
            if (i == 1 && !is_addressing_mode_allowed(opcode->dest_modes, addressing_mode)) {
                printf("Error: Invalid addressing mode for destination operand\n");
                return 0;
            }

            if (!add_operand_value(&parsed_line->content.inst, token_arr, current_token, i)) {
                printf("Error: Invalid operand value\n");
                return 0;
            }

            (*current_token)++;
        }
        break;

    case ZERO_OPERAND:

        break;
    }
    if (token_arr[*current_token].type != UNKNOWN) {
        printf("Error in line %d: Too many operands for instruction '%s'"
               "\n", parsed_line->line_number, opcode->mnemonic);
        return 0;
    }

    printf("No Errors\n");
    return 1;
}

int add_operand_value(Instruction *inst, Token *token_arr, int *current_index, int op_index) {
    AddressingMode mode;
    int immediate;

    op_index = 0;
    immediate = 0;

    switch (token_arr[*current_index].type) {
    case IMMEDIATE:
        mode = ADD_IMMEDIATE;
        immediate = atoi(token_arr[*current_index].value + 1);
        if (immediate > MAX_IMMEDIATE_VALUE || immediate < MIN_IMMEDIATE_VALUE) {
            printf("Error: Immediate value '%s' out of range (must be between -2048 and 2047)\n", token_arr[*current_index].value);
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
    char *endptr;
    long value;
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

void print_parsed_line(Line *parsed_line) {
    const char *line_type_to_string(LineType line_type);
    const char *operand_type_to_string(int operand_type);
    const char *opcode_to_string(int opcode);
    int i;

    if (parsed_line == NULL) {
        printf("Error: Parsed line is NULL\n");
        return;
    }
    printf("--------------------parsed line-----------------------\n");
    printf("Line type: %s\n", line_type_to_string(parsed_line->type));
    printf("Label: %s\n", parsed_line->label ? parsed_line->label : "N/A");

    switch (parsed_line->type) {

    case LINE_INSTRUCTION:
        printf("Instruction: %s\n",
               opcode_to_string(parsed_line->content.inst.opcode));
        for (i = 0; i < parsed_line->content.inst.operands_count; i++) {
            printf(
                "Operand %d type: %s\n", i,
                operand_type_to_string(parsed_line->content.inst.operand_types[i]));
            if (parsed_line->content.inst.operand_types[i] == ADD_IMMEDIATE) {
                printf("Operand %d immediate value: %d\n", i,
                       parsed_line->content.inst.operands[i].immediate);
            } else if (parsed_line->content.inst.operand_types[i] == ADD_REGISTER) {
                printf("Operand %d register: %d\n", i,
                       parsed_line->content.inst.operands[i].reg);
            } else if (parsed_line->content.inst.operand_types[i] == ADD_DIRECT) {
                printf("Operand %d label: %s\n", i,
                       parsed_line->content.inst.operands[i].symbol ? parsed_line->content.inst.operands[i].symbol : "N/A");
            }
        }
        break;

    case LINE_DATA:

        printf("Data type: INT\n");
        printf("Data value count: %d\n", parsed_line->content.data.value_count);

        for (i = 0; i < parsed_line->content.data.value_count; i++) {
            printf("Data value: %d\n", parsed_line->content.data.content.int_values[i]);
        }
        break;

    case LINE_STRING:
        printf("Data type: STRING\n");
        printf("String length: %d\n", parsed_line->content.data.value_count);
        printf("String: %s\n", parsed_line->content.data.content.char_values);
        break;

    case LINE_ENTRY:
        printf("Entry\n");
        break;

    case LINE_EXTERN:
        printf("Extern\n");
        break;

    default:
        printf("Invalid line type\n");
        break;
    }
    printf("------------------------------------------------------\n");

    return;
}

const char *line_type_to_string(LineType line_type) {
    switch (line_type) {
    case LINE_INSTRUCTION:
        return "INSTRUCTION";
    case LINE_DATA:
        return "DATA";
    case LINE_STRING:
        return "STRING";
    case LINE_ENTRY:
        return "ENTRY";
    case LINE_EXTERN:
        return "EXTERN";
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
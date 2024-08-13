#include "parser.h"

extern int IC, DC, current_line;

/* Helper functions for parsing instructions and data */
int get_addressing_type(TokenType type);
int calculate_instruction_words(int opcode);
void print_parsed_line(Line *parsed_line);
int is_opcode(TokenType type);
int is_data(TokenType type);
void parse_instruction_line(Line *parsed_line, Token *token_arr, int *i,int *operand_count);
void add_operand_value(Instruction *inst, Token *token_arr, int *i, int op_index);
void parse_data_line(Line *parsed_line, Token *token_arr, int *current_token, int *operand_count);

/*------------------------------------------------------------------------*/
/* TODO: Finish the instruction parsing logic, make sure operands are parsed
 * correctly */
/* TODO: Add data parsing logic */
/* TODO: Error handling */
/*------------------------------------------------------------------------*/
Line *parse_line(Token *token_arr, int token_count) {
  int operand_count;
  int data_count;
  int current_token;
  int label_def_flag;
  Line *parsed_line;

  TRY(parsed_line = calloc(1, sizeof(Line)));

  data_count = 0;
  current_token = 0;
  label_def_flag = 0;

  /* Check for label */
  if (token_arr[current_token].type == LABEL_DEF) {
    strcpy(parsed_line->label, token_arr[current_token].value);
    label_def_flag = 1;
    current_token++; /* Skips label definition */
  }
  
  /* Checks if it's an instruction line */ 
  if (is_opcode(token_arr[current_token].type)) {
      parse_instruction_line(parsed_line, token_arr, &current_token, &operand_count);
      if (label_def_flag) {
          addSymbol(parsed_line->label, &IC, SYMBOL_CODE);
    }

    IC += operand_count + 1;
    
    return parsed_line;
  }
  /* Check for data */  
  if (is_data(token_arr[current_token].type)) {
      parse_data_line(parsed_line, token_arr, &current_token, &data_count);
      if (label_def_flag) {
          addSymbol(parsed_line->label, &DC, SYMBOL_DATA);
      }

      return parsed_line;
  }

  /* Error */
  printf("Invalid line: %s\n", token_arr[current_token].value);
  return NULL;
}

void parse_data_line(Line *parsed_line, Token *token_arr, int *current_token, int *data_count) {
    int token_count;
    Data *data;

    data = &(parsed_line->content.data);
    token_count = strlen(token_arr[*current_token].value);

    /* Check the type of data */
    switch (token_arr[*current_token].type) {
    case DATA:
        /* Parse the integer data and update DC */
        data->type = DATA_INT; /* not sure if needed */
        (*current_token)++;    /* Advancing to first integer */

        for (; *current_token < token_count; (*current_token)++) {
            if (token_arr[*current_token].type == COMMA) {
                /* TODO: figure a way to handle missing or extranouse commas errors
                 * either here or in the tokenizer.c */

                continue; /* Skips comma */
            }

            if ((token_arr[*current_token].type == INTEGER)) {
                data->content.int_values[(*data_count)++] = strtol(token_arr[*current_token].value, NULL, 10);
                DC++;
            } else {
                printf("Invalid token type for integer data: %s\n", token_arr[*current_token].value);
                return;
            }
            break;
        }
    case STRING:
        /* Parse the string data and update DC */
        data->type = DATA_STRING;
        (*current_token)++; /* Advancing to string litral*/
        if (token_arr[*current_token].type == STRING_LITERAL) {
            strncpy(data->content.char_values, token_arr[*current_token].value, MAX_DATA_VALUES); /* check syntax? */
            DC += (strlen(data->content.char_values)) + 1; /* +1 for the null character */
            (*data_count)++;
        }
        break;

    default:
        printf("Invalid token type for data line: %s\n", token_arr[*current_token].value);
        return;
    }
}

void parse_instruction_line(Line *parsed_line, Token *token_arr, int *current_token, int *operand_count) {
  int op_index;
  op_index = 0;
  /* Set the line type */
  parsed_line->type = LINE_INSTRUCTION; 
  /* Gets the opcode and Skips to the next token */
  parsed_line->content.inst.opcode = ((token_arr[*current_token].type) - MOV); 
  (*current_token)++;
  *operand_count = calculate_instruction_words(parsed_line->content.inst.opcode); /* Gets the number of operands */

  /* Parse operands by number of operands */
  switch (*operand_count) {
  case 2:
    add_operand_value(&parsed_line->content.inst, token_arr, current_token, op_index);
    (*current_token)++; /* Advancing */
      if (token_arr[*current_token].type != COMMA) {
      printf("%s\n", token_arr[*current_token].value);
      printf("Missing comma\n");
      return;
    }
    (*current_token)++; /* Skips the comma */
    op_index++; /* Adderssing the second operand */
    add_operand_value(&parsed_line->content.inst, token_arr, current_token, op_index);


    parsed_line->content.inst.operand_types[1] = get_addressing_type(token_arr[*current_token].type);
    break;

  case 1:
    printf("One operand instruction\n");
    break;

  case 0:
    printf("Zero operand instruction\n");
    break;

  default:
    printf("Wrong operand count\n");
    break;
  }
}

void add_operand_value(Instruction *inst, Token *token_arr, int *i, int op_index ) {
  /*TODO: Needs to add value as well */

  switch (token_arr[*i].type) {
  case HASH:
    inst->operand_types[op_index] = ADD_IMMEDIATE;
    inst->operands[op_index].immediate = strtol(token_arr[*i].value, NULL, 10);
    break;
  case LABEL_USE:
    inst->operand_types[op_index] = ADD_DIRECT;
    strncpy(inst->operands[op_index].symbol, token_arr[*i].value, MAX_LABEL_LENGTH);
    break;
  case ASTERISK:
    (*i)++; /* Skips the asterisk */
    inst->operand_types[op_index] = ADD_INDIRECT_REGISTER;
    if (token_arr[*i].type >= R0 && token_arr[*i].type <= R7) {
      inst->operands[op_index].reg = token_arr[*i].type - R0;
    }
    break;

  case R0:
  case R1:
  case R2:
  case R3:
  case R4:
  case R5:
  case R6:
  case R7:
    inst->operand_types[op_index] = ADD_REGISTER;
    inst->operands[op_index].reg = token_arr[*i].type - R0;
    break;

  default:
    printf("Invalid operand %s\n", token_arr[*i].value);
    break;
  }
}

int get_addressing_type(TokenType type) {

  if (type == HASH) {
    return ADD_IMMEDIATE;
  }

  if (type == LABEL_USE) {
    return ADD_DIRECT;
  }
  /*       mov r5 , * r3      */
  if (type == ASTERISK) { /* Assuming token is correct? *5 */
    return ADD_INDIRECT_REGISTER;
  }

  if (type >= R0 && type <= R7) {
    return ADD_REGISTER;
  }

  return -1;
}
int calculate_instruction_words(int opcode) {
  if (opcode + MOV >= MOV && opcode + MOV <= LEA) {
    return 2; /* Two operand instructions */
  }

  if (opcode + MOV >= CLR && opcode + MOV <= JSR) {
    return 1; /* One operand instructions */
  }

  if (opcode + MOV == RTS || opcode + MOV == STOP) {
    return 0; /* Zero operand instructions */
  }

  return -1; /* Unexpected token type */
}
int is_opcode(TokenType type) { return type >= MOV && type <= STOP; }
int is_data(TokenType type) { return (type == DATA || type == STRING); }

void print_parsed_line(Line *parsed_line) {
  const char *line_type_to_string(LineType line_type);
  const char *operand_type_to_string(int operand_type);
  const char *opcode_to_string(int opcode);
  int i;
  printf("Line type: %s\n", line_type_to_string(parsed_line->type));
  printf("Label: %s\n", parsed_line->label);

  if (parsed_line->type == LINE_INSTRUCTION) {
    printf("Instruction: %s\n",
           opcode_to_string(parsed_line->content.inst.opcode));
    for (i = 0; i < 2; i++) {
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
        printf("Operand %d symbol: %s\n", i,
               parsed_line->content.inst.operands[i].symbol);
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
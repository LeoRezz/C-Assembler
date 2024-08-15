#include "tokenizer.h"
#include "opcodeTable.h"

/* To compile: gcc -g first_word_binary.c -o first_word_binary */
/*----------------------------------------------------------------------------*/

/* Function prototypes */
char* FirstTokenToBinary(Line *line, char binaryCode[]);
char* CodeToBinary(int, int);
char* InstructionToBinary(Line *);
/* Function copied from parser.c */
void print_parsed_line(Line *parsed_line);
int calculate_instruction_words(int opcode);

int CommandsArrayLength = 20;
/*/////////////////////--------------------NO ERROR CATCHING??----------------------------------*/
/*The function gets a decimal integer and covert it to binary for the amount of specified bits*/
char* CodeToBinary(int input, int numberOfBits) 
{
    int i = numberOfBits - 1;
    unsigned int value = input;
    char *binary = (char *)malloc(numberOfBits + 1); ////free
    for (; i >= 0; i--) 
    {
        binary[i] = (value & 1) ? '1' : '0';
        value >>= 1;
    }
    binary[numberOfBits] = '\0';
    return binary;
}

char *InstructionToBinary(Line *line) {
    int i = 0;
    int address = 0;
    /* char binaryCode[13] = "";   Warning: Function returns address of local variable */
    char *binaryCode = (char *)malloc(14);             /* Free later */
    binaryCode = FirstTokenToBinary(line, binaryCode); /*save to the binarycode file */
    
    if (line->content.inst.operands_count > 0) {
        /* code */

        switch (line->content.inst.operand_types[0]) {
        case 1:                                                                 /* ADD_IMMEDIATE need to change to 1 instead of 0 */
            strcat(binaryCode, CodeToBinary(line->content.inst.operands[i].immediate, 12)); /* 2's complement */
            strcat(binaryCode, CodeToBinary(4, 3));                                         /* A=1, R=0 ,E=0 */
            printf("DEBUG: In InstructionToBinary()\nswitch(operand_types[0])\ncase: ADD_IMMEDIATE\n\n");
            return binaryCode;

            /*/////////////////// finish later its 5 AM #labeltable #M&M&M//////////////////*/

        case 2: /* ADD_DIRECT  --------------- need to access labeltable address
            //label = line->content.inst.operands[i].symbol
            //strcat(binaryCode, CodeToBinary(,12)); 2's complement */
            printf("ADD_DIRECT not implemented yet\n");
            break;

        case 4: /*ADD_INDIRECT_REGISTER */
            printf("ADD_INDIRECT_REGISTER not implemented yet\n");
            /* code */
            break;

        case 8: /*ADD_REGISTER*/
            printf("ADD_REGISTER not implemented yet\n");
            /* code */
            break;

        default:
            printf("Reached default\n");
            return NULL;
            break;
        }
    } else {
        printf("NO OPERANDSSSSSSSS\n");
        return binaryCode;
    }
}

char* FirstTokenToBinary(Line *line, char *binaryCode) //only if instruction line // o 1 2 3 to 1 2 4 8
{ 
    int instruction_words;

    instruction_words = calculate_instruction_words(line->content.inst.opcode);
    char* binaryCodeToReturn = (char*) calloc(14, sizeof(char)); // not sure if neede here?
    strcat(binaryCode, CodeToBinary(line->content.inst.opcode,4)); //bits 11-14 are opcode
    switch (instruction_words) 
    {
        case 0:
            strcat(binaryCode, CodeToBinary(0,8)); // source and destination are irrelevant
            strcat(binaryCode, CodeToBinary(4,3)); // A=1,R=0 ,E=0 //100
            strcpy(binaryCodeToReturn , binaryCode); 
        return binaryCodeToReturn;

        case 1:
            strcat(binaryCode, CodeToBinary(0,4)); //source operand is irrelevant
            strcat(binaryCode, CodeToBinary(line->content.inst.operand_types[0],4)); //dest operand adressing type to binary
            strcat(binaryCode, CodeToBinary(4,3)); // A=1, R,E=0
            strcpy(binaryCodeToReturn , binaryCode);
        return binaryCodeToReturn;

        case 2:
            strcat(binaryCode, CodeToBinary(line->content.inst.operand_types[0],4)); //source operand adressing type to binary
            strcat(binaryCode, CodeToBinary(line->content.inst.operand_types[1],4)); //dest operand adressing type to binary
            strcat(binaryCode, CodeToBinary(4,3)); // A=1, R,E=0
            strcpy(binaryCodeToReturn , binaryCode);
        return binaryCodeToReturn;
        
        default:
          printf("Reached default\n");
            break; //error message?
    }
    
}


void test_CodeToBinary() {
    int test_cases[][2] = {{10, 8}, {15, 4}, {255, 8}, {0, 4}, {-5, 8}};
    int num_tests = sizeof(test_cases) / sizeof(test_cases[0]);

    for (int i = 0; i < num_tests; i++) {
        int input = test_cases[i][0];
        int bits = test_cases[i][1];
        char* result = CodeToBinary(input, bits);
        printf("Input: %d, Bits: %d, Binary: %s\n", input, bits, result);
        free(result);  // Don't forget to free the allocated memory
    }
}

int main() {
    //test_CodeToBinary();

        Line line1 = {  /* MAIN:    add #5, #3 */
        .address = 100,
        .type = LINE_INSTRUCTION,
        .label = "MAIN",
        .content.inst = {
            .opcode = ADD-MOV,
            .operand_types = {ADD_IMMEDIATE, ADD_REGISTER},
            .operands = {{.immediate = 5}, {.reg = 3}}
        }
    };

    
        Line line2 = {  /* END:    STOP */
        .address = 103,
        .type = LINE_INSTRUCTION,
        .label = "END",
        .content.inst = {
            .opcode = STOP-MOV,
            .operand_types = { },
            .operands = {{}, {}}
        }
    };
    //print_parsed_line(&line1);
    print_parsed_line(&line2);

    // char *binary = InstructionToBinary(&line1);
    // printf("Binary: %s\n", binary);
    // free(binary);
    // printf("\n");
    char *binary2 = InstructionToBinary(&line2);
    printf("Binary: %s\n", binary2);
    free(binary2);
    return 0;
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

    case LINE_DIRECTIVE:
        printf("Directive\n");
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
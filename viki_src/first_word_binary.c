#include "tokenizer.h"
#include "symbolTable.h"
#include "opcodeTable.h"
#include "parser.h"

int CommandsArrayLength = 20;
///////////////////////--------------------NO ERROR CATCHING??----------------------------------
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


char InstructionToBinary(Line *line)
{   int i = 0;
    int address = 0;
    char binaryCode[13] = "";
    FirstTokenToBinary(line, binaryCode); //save to the binarycode file

    switch (line->content.inst.operand_types[0])
    {
        case 1: //ADD_IMMEDIATE
            strcat(binaryCode, CodeToBinary(line->content.inst.operands[i].immediate,12)); //משלים ל2
            strcat(binaryCode, CodeToBinary(4,3)); // A=1, R,E=0 
            return binaryCode;                          


///////////////////// finish later its 5 AM #labeltable #M&M&M////////////////////

        case 2: //ADD_DIRECT  --------------- need to access labeltable address
            //label = line->content.inst.operands[i].symbol
            //strcat(binaryCode, CodeToBinary(,12)); //משלים ל2

            break;

        case 4: //ADD_INDIRECT_REGISTER
        /* code */
            break;

        case 8: //ADD_REGISTER
            /* code */
            break;

        default:
            break;
    }
}
    

    
    


char* FirstTokenToBinary(Line *line, char binaryCode[]) //only if instruction line
{ 
    char* binaryCodeToReturn = (char*) calloc(13, sizeof(char)); // not sure if neede here?
    strcat(binaryCode, CodeToBinary(line->content.inst.opcode,4)); //bits 11-14 are opcode
    switch (calculate_instruction_words(line->content.inst.opcode)) 
    {
        case 0:
            strcat(binaryCode, CodeToBinary(0,8)); // source and destination are irrelevant
            strcat(binaryCode, CodeToBinary(4,3)); // A=1, R,E=0
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
            break; //error message?
    }
    
}

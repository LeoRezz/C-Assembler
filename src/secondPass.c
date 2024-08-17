#include <math.h>
#include "tokenizer.h"
#include "symbolTable.h"
#include "opcodeTable.h"
#include "parser.h"
#include "memory_manager.h"
#include "parsed_program.h"
#include <unistd.h>


#define STARTING_ADDRESS 100


/*---------------------------TODO-----------------------*/
/*extren and entry tables*/
/*fix order in secondpass() first all instructions*/
/*write first line before translating...*/


int address = STARTING_ADDRESS;

void secondPass(ParsedProgram *program, int numOfLines);                  /*main function for the second transition*/
void WriteLine(char binarycode[], FILE* testfile, char* filename);         /*write a line to the object file*/
char* CodeToBinary(int input, int numberOfBits);                          /*translates  assemly to binary and returns it*/
int InstructionToBinary(Line *line, FILE* testfile, char* filename);      /*translates  an insuction line and writes it in file*/
int DataToBinary(Line *line, FILE* testfile, char* filename);             /*translates  a data line and writes to file*/
void FirstTokenToBinary(Line *line, FILE* testfile, char* filename);      /*creats the first word for the instruction line*/
int BinaryToOctal(char binarycode[]);                                     /*converts binary to octal*/
void WriteFirstLine(int ins, int data, FILE* testfile, char* filename);   /*add the first line to the object file*/
void entryFile();                                                         /*responsible for .ent file*/
void externFile();                                                     


/* draft */
void generate_ob_file(const char* filename) {
    FILE* file = fopen(filename, "w");
    if (file == NULL) {
        /* Handle error */
        return;
    }
    /* Example of working with the new memory_manager API */
    /* Write the first line with instruction and data counts */
    fprintf(file, "%d %d\n", get_IC() - INITIAL_ADDRESS, get_DC());

    /* Rest of your .ob file generation code */

    fclose(file);
}


void secondPass(ParsedProgram *program, int numOfLines) {

    char filename[] = "test.ob";   //add correct file name
    FILE* testfile;
    Line temp;
    testfile = fopen(filename, "w"); /*create object file*/ 
    fclose(testfile);

    int i = 0;
    char binaryCode[15] = "";

    for (; i < numOfLines; i++) /*TODO change order, first all instructions*/
    {   
        Line *current = &(program->lines[i]);

        if ( current->type == 0) /*instruction line*/
        {
            InstructionToBinary(current, testfile, filename);  /*lines of instruction*/
        }
        else if (current->type == 1 || current->type== 2) //line_data, line_string
        {
            DataToBinary(current, testfile, filename); /*lines of data*/
        }
                      
    }

    entryFile(); /*creates and write to .ent file*/


}
    






int InstructionToBinary(Line *line, FILE* testfile, char* filename)
{ 
    int i;
    int j;
    symbol *symbol_table;
    int symbolCount;
    
    symbol_table = get_symbol_table(); /* get the symbol table */
    symbolCount = get_symbol_count(); /* get the symbol count */


    char binaryCode[15] = "";
    FirstTokenToBinary(line, testfile, filename); //TODO: int or void with writeline?
    binaryCode[15] = "";
    if (line->content.inst.operands_count != 0)
    {
        switch (line->content.inst.operand_types[i])
        {
        case 1: //ADD_IMMEDIATE
            strcat(binaryCode, CodeToBinary(line->content.inst.operands[0].immediate,12)); //משלים ל2
            strcat(binaryCode, CodeToBinary(4,3)); /* A=1, R,E=0 */ 
            WriteLine(binaryCode, testfile, filename); /* writes the first word,TODO: write function that appends line as address and octal value of binary*/

        case 2: //ADD_DIRECT  
            for (j = 0; j < symbolCount; j++) { 
                if (strcmp(symbol_table[j].name, line->content.inst.operands[i].symbol) == 0) {
                    strcat(binaryCode, CodeToBinary(symbol_table[j].value,12)); /* bits 14-3 are for label address */
                    if (symbol_table[j].type==3) /*extren label*/
                    {
                     strcat(binaryCode, CodeToBinary(1,3)); /*  A,R=0  E=1 */                
                    }
                    else
                    {
                     strcat(binaryCode, CodeToBinary(2,3)); /*  A=0  R=1  E=0 */
                    }
                    WriteLine(binaryCode, testfile, filename);
                    
                    
                }
            }

        case 4: //ADD_INDIRECT_REGISTER
        case 8: //ADD_REGISTER
            strcat(binaryCode, CodeToBinary(0,6)); /*bits 14-9 are not used*/
            if (line->content.inst.operands_count==2)
            {
                strcat(binaryCode, CodeToBinary(line->content.inst.operands[0].reg,3)); // bits 8-6 are source reg
                strcat(binaryCode, CodeToBinary(line->content.inst.operands[1].reg,3)); // bits 3-5 are destination reg

            }
            else{
                strcat(binaryCode, CodeToBinary(0,3)); // no source operand
                strcat(binaryCode, CodeToBinary(line->content.inst.operands[0].reg,3)); // bits 3-5 are destination reg            
            }
            strcat(binaryCode, CodeToBinary(4,3)); /* A=1, R,E=0 */
            WriteLine(binaryCode, testfile, filename);

        default:
            break;
    }
    }
    
    
    
}


int DataToBinary(Line *line, FILE* testfile, char* filename){
    int i;
    int j;
    char binaryCode[15] = "";
    int lines_of_data =0;

    switch (line->content.data.type)
    {
    case 0: /* DATA_INT */
        for (i=0;i < line->content.data.value_count; i++)
        {
            strcat(binaryCode, CodeToBinary(line->content.data.content.int_values[i],15));
            WriteLine(binaryCode, testfile, filename);
            for (j = 0; j < 15; j++) {
                binaryCode[i] = '0';
            }
        }
        
        return lines_of_data;

    case 1: /*DATA_STRING*/
        for (i=0;i<sizeof(line->content.data.content.char_values); i++)
        {
            char c = line->content.data.content.char_values[i];
            strcat(binaryCode, CodeToBinary(0,7)); /*ascii only takes 8 bits, so bits 14-8 are set to o*/
                for (j = 7; j >= 0; --j) {
                binaryCode[7-i] = ((c >> i) & 1) + '0'; /*adding '0' is like adding '48' to get the right ascii value*/
                }
            WriteLine(binaryCode, testfile, filename);
        }
        strcat(binaryCode, CodeToBinary(0,15)); /*print /0 to end string*/ //lines_of_data++?
        WriteLine(binaryCode, testfile, filename);
        return lines_of_data;

    default:
        break;
    }
}



/*function writes to object file the first word*/
void FirstTokenToBinary(Line *line, FILE* testfile, char* filename ) //only if instruction line
{ 
    int lines_writen = 0;
    char binaryCode[16] = "";
    strcat(binaryCode, CodeToBinary(line->content.inst.opcode,4)); //bits 11-14 are opcode
    switch (line->content.inst.operands_count) 
    {
        case 0:
            strcat(binaryCode, CodeToBinary(0,8)); // source and destination are irrelevant
            strcat(binaryCode, CodeToBinary(4,3)); // A=1, R,E=0
            WriteLine(binaryCode, testfile, filename );
            break;

        case 1:
            strcat(binaryCode, CodeToBinary(0,4)); //source operand is irrelevant
            strcat(binaryCode, CodeToBinary(line->content.inst.operand_types[0],4)); //dest operand adressing type to binary
            strcat(binaryCode, CodeToBinary(4,3)); // A=1, R,E=0
            WriteLine(binaryCode, testfile, filename );
            break;



        case 2:
            strcat(binaryCode, CodeToBinary(line->content.inst.operand_types[0],4)); //source operand adressing type to binary
            strcat(binaryCode, CodeToBinary(line->content.inst.operand_types[1],4)); //dest operand adressing type to binary
            strcat(binaryCode, CodeToBinary(4,3)); // A=1, R,E=0
            WriteLine(binaryCode, testfile, filename );
            break;

        default:
            break; //error message?
    }
    
}


/*The function gets a decimal integer and covert it to binary for the amount of specified bits*/
char* CodeToBinary(int input, int numberOfBits) 
{
    int i = numberOfBits - 1;
    int value = input;
    char *binary = (char *)malloc(numberOfBits); ////free
    for (; i >= 0; i--) 
    {
        binary[i] = (value & 1) ? '1' : '0';
        value >>= 1;
    }
    return binary;
}

int BinaryToOctal(char binarycode[]){

    int oct = 0, ctr = 0, reminder = 0, temp = 0, i = 0;
    
    temp = strlen(binarycode); /* Size of pointer instead of string */
    while (temp != 0) {
        reminder = temp % 3;
        if (ctr == 0)
            reminder = temp % 3;
        else
            reminder = temp % (int) pow(10, ctr);
        oct += reminder * pow(2, i);
        temp /= 10;
        ctr++;
    }

    return oct;
}

void WriteLine(char binarycode[15], FILE* testfile, char* filename) /*should be void?*/
{

/* 4 digits for address, 1 space, 5 digits to octal */  
    int octal = BinaryToOctal(binarycode);
    FILE* file = fopen(filename, "a");
    if (file != NULL) {

        fprintf(file, "%04d %05d\n", 1, octal); 
        fclose(file);
    } else {
        printf("Could not open file %s\n", filename); //
    }
    address++;
}



void entryFile()
{
    int i;
    symbol *symbol_table;
    int symbolCount;
    symbol_table = get_symbol_table(); /* get the symbol table */
    symbolCount = get_symbol_count(); /* get the symbol count */
    
    char entry_name[] = "test.ent";   //add correct file name
    FILE* entry_file;
    for (i = 0; i < symbolCount; i++)
    {
        if (symbol_table->type == 2) /*SYMBOL_ENTRY*/
        {

        if(access("test.ent", F_OK) != -1) { /*checks if file already exists*/
            
            entry_file = fopen(entry_name, "a");
            fprintf(entry_file, "%s %d\n", symbol_table->name , symbol_table->value ); 
            fclose(entry_file);

        } else {

            entry_file = fopen(entry_name, "w"); /*create entry file*/ 
            fclose(entry_file);
            entry_file = fopen(entry_name, "a");
            fprintf(entry_file, "%s %d\n", symbol_table->name , symbol_table->value ); 
            fclose(entry_file);
        }
        }
    }
}



void externFile()
{
    int i;
    symbol *symbol_table;
    int symbolCount;
    symbol_table = get_symbol_table(); /* get the symbol table */
    symbolCount = get_symbol_count(); /* get the symbol count */
    
    char extern_name[] = "test.ext";   //add correct file name
    FILE* extern_file;
    if (symbol_table->type == 3) /*SYMBOL_EXTERN*/
    {

    if(access("test.ent", F_OK) != -1) { /*checks if file already exists*/
            
        extern_file = fopen(extern_name, "a");
        fprintf(extern_file, "%s %d\n", symbol_table->name , address ); 
        fclose(extern_file);

    } else {

        extern_file = fopen(extern_name, "w"); /*create entry file*/ 
        fclose(extern_file);
        extern_file = fopen(extern_name, "a");
        fprintf(extern_file, "%s %d\n", symbol_table->name , symbol_table->value ); 
        fclose(extern_file);
    }

    } 
}
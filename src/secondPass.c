
#include <unistd.h>



#include "memory_manager.h"
#include "parser.h"
#include "parsed_program.h"
#include "tokenizer.h"

#define STARTING_ADDRESS 100
#define WORD_SIZE 20


int address = STARTING_ADDRESS;
int data_counter;
extern int error_flag;
int secondPass( char *filename, ParsedProgram *program, int numOfLines);
void generate_ob_file( char *filename, char *ins_name, char *data_name);
char* CodeToBinary(int input, int numberOfBits);
int BinaryToOctal(char binarycode[]);
void DataToBinary(Line *line, FILE* testfile, char* filename);
void InstructionToBinary(Line *line, FILE *testfile, char *filename, char *file_extern);
void FirstTokenToBinary(Line *line, FILE* testfile, char* filename );
int hasCommonWord(Line *line);
void WriteLine(char *binarycode, FILE* testfile, char* filename);
void entryFile( char *filename);
void externFile(symbol symbol ,  char *filename);

/*TODO:: correct file names!!!*/

int secondPass( char *filename, ParsedProgram *program, int numOfLines) {
    FILE *testfile;
    FILE *datafile;
    char ins_name[8];
    char data_name[8];
    int i;
    
    error_flag = 0;
    data_counter = 0;

    strcpy(ins_name, "ins.ob");
    strcpy(data_name, "data.ob");

    i = 0;
    for (; i < numOfLines; i++) {
        Line *current = &(program->lines[i]);

        if (current->type == LINE_INSTRUCTION) /*instruction line*/
        {
            InstructionToBinary(current, testfile, ins_name , filename);        /*lines of instruction*/
        } else if (current->type == LINE_DATA || current->type == 2)            /*line_data, line_string*/
        {
            DataToBinary(current, datafile, data_name); /*lines of data*/
        }
    }

    generate_ob_file("ps.ob", ins_name, data_name);
    
    /*creates and write to .ent file*/
    entryFile(filename); 

    return (error_flag == 0);
}

void generate_ob_file( char* filename, char* ins_name, char* data_name) {

    char c;
    FILE *file, *ins, *data;

    
    if (error_flag == 0)
    {    
        file = fopen(filename, "w");
        if (file == NULL) {
        printf("error while openening object file.\n");
        error_flag = 1;
        return;
    }
        fprintf(file, "%d %d\n", get_IC() - INITIAL_ADDRESS, get_DC());
        fclose(file);

        ins = fopen(ins_name, "r");
        if (ins == NULL) {
            printf("Error while opening file.\n");
            error_flag = 1;
            return;
        }

        file = fopen(filename, "a");
        if (file == NULL) {
            printf("Error while opening object file.\n");
            fclose(ins);  /* Close the other file before returning */
            return; 
        }
        while ((c = fgetc(ins)) != EOF)
        fputc(c, file);
        fclose(ins);

        data = fopen(data_name, "r");
        if (data == NULL) {
            printf("Error while opening file.\n");
            fclose(file); /*close ob file*/
            return;
        }
        while ((c = fgetc(data)) != EOF)
        fputc(c, file);
        fclose(data);

        fclose(file);

        remove(ins_name);
        remove(data_name);

        
    }
    

}

void DataToBinary(Line *line, FILE* testfile, char* filename){ /*extrenFile()*/
    int i;
    int j;
    char c;
    char str[MAX_TOKEN_LENGTH] = "";
    char binaryCode[WORD_SIZE] = "";
    char *temp;

    switch (line->content.data.type)
    {
    case DATA_INT: 
        for (i=0;i < line->content.data.value_count; i++)
        {
            memset(binaryCode, 0, sizeof(binaryCode)); /*reset binarycode*/
            
            temp = CodeToBinary(line->content.data.content.int_values[i],15);
            strcat(binaryCode, temp);
            free(temp);
            WriteLine(binaryCode, testfile, filename);
            data_counter++;

        }
    break; /* extern label  */

    case DATA_STRING:
        i = 0;
        while (line->content.data.content.char_values[i] != '\0') {
            memset(binaryCode, 0, sizeof(binaryCode)); /*reset binarycode*/
            memset(str, 0, sizeof(str));               /*reset temp*/
            c = line->content.data.content.char_values[i];
            temp = CodeToBinary(0, 7);
            strcat(binaryCode, temp); /*ascii only takes 8 bits, so bits 14-8 are set to o*/
            free(temp);
            for (j = 7; j >= 0; --j) {
                str[7 - j] = ((c >> j) & 1) + '0'; /*adding '0' is like adding '48' to get the right ascii value*/
            }
            strcat(binaryCode, str);
            WriteLine(binaryCode, testfile, filename);
            data_counter++;
            i++;
        }
        memset(binaryCode, 0, sizeof(binaryCode));
        temp =  CodeToBinary(0,15);
        strcat(binaryCode,temp); /*print /0 to end string*/ 
        free(temp);
        WriteLine(binaryCode, testfile, filename);
        data_counter++;
        break;

    default:
        break;
    }
}

void InstructionToBinary(Line *line, FILE *testfile, char *filename , char *file_extern ) {
    int i;
    int j;
    char *temp;
    symbol *symbol_table;
    int symbolCount;
    char binaryCode[WORD_SIZE] = "";

    symbol_table = get_symbol_table(); /* get the symbol table */
    symbolCount = get_symbol_count();  /* get the symbol count */

    FirstTokenToBinary(line, testfile, filename); 

    if ((line->content.inst.operands_count == 2) && hasCommonWord(line))
    {   
        temp = CodeToBinary(0,6);
        strcat(binaryCode, temp); /*bits 14-9 are not used*/
        free(temp);
        
        temp = CodeToBinary(line->content.inst.operands[0].reg,3);
        strcat(binaryCode, temp); /* bits 8-6 are source reg */
        free(temp);

        temp = CodeToBinary(line->content.inst.operands[1].reg,3);
        strcat(binaryCode, temp); /* bits 3-5 are destination reg */
        free(temp);

        temp = CodeToBinary(4,3);
        strcat(binaryCode, temp); /* A=1, R,E=0 */
        free(temp);
        WriteLine(binaryCode, testfile, filename);
    }
    else{    
    for ( i = 0; i < line->content.inst.operands_count; i++)
    {
        memset(binaryCode, 0, sizeof(binaryCode)); /*reset binarycode*/
        switch (line->content.inst.operand_types[i]) 
        {
            case ADD_IMMEDIATE: 
                temp = CodeToBinary(line->content.inst.operands[0].immediate,12);
                strcat(binaryCode,temp ); 
                free(temp);

                temp =  CodeToBinary(4,3);
                strcat(binaryCode,temp); /* A=1, R,E=0 */ 
                free(temp);
                WriteLine(binaryCode, testfile, filename); /* writes the first word,TODO: write function that appends line as address and octal value of binary*/
            break;

            case ADD_DIRECT:  
                for (j = 0; j < symbolCount; j++) { 
                   if (strcmp(symbol_table[j].name, line->content.inst.operands[i].symbol) == 0) {
                        temp = CodeToBinary(symbol_table[j].value,12);
                        strcat(binaryCode, temp); /* bits 14-3 are for label address */
                        free(temp);

                        if (symbol_table[j].type == SYMBOL_EXTERNAL) /*extren label*/
                        {
                            temp = CodeToBinary(1,3);
                            strcat(binaryCode, temp ); /*  A,R=0  E=1 */   
                            free(temp);
                            externFile(symbol_table[j], file_extern);
                        } 
                        else
                        {
                            temp = CodeToBinary(2,3);
                            strcat(binaryCode, temp ); /*  A=0  R=1  E=0 */
                            free(temp);
                        }
                        WriteLine(binaryCode, testfile, filename); 
                        break;                  
                    }
                }
            break;

        case 4: /*ADD_INDIRECT_REGISTER*/
        case 8: /*ADD_REGISTER*/

            if (i==0) /*source operand*/
            {
                temp =  CodeToBinary(0,6);
                strcat(binaryCode, temp); /*bits 14-9 are not used*/
                free(temp);

                temp =  CodeToBinary(line->content.inst.operands[0].reg,3);
                strcat(binaryCode,temp); /* bits 8-6 are source reg */
                free(temp);

                temp = CodeToBinary(0,3);
                strcat(binaryCode, temp ); 
                free(temp);

                temp =  CodeToBinary(4,3);
                strcat(binaryCode,temp); /* A=1, R,E=0 */
                free(temp);

                WriteLine(binaryCode, testfile, filename);

            } else { /*destination operand*/

                temp = CodeToBinary(0, 6);
                strcat(binaryCode, temp); /*bits 14-9 are not used*/
                free(temp);

                temp = CodeToBinary(0, 3);
                strcat(binaryCode, temp);
                free(temp);

                temp = CodeToBinary(line->content.inst.operands[1].reg, 3);
                strcat(binaryCode, temp); /* bits 3-5 are destination reg */
                free(temp);

                temp = CodeToBinary(4, 3);
                strcat(binaryCode, temp); /* A=1, R,E=0 */
                free(temp);
                WriteLine(binaryCode, testfile, filename);
            }
            break;
        default:
            break;
        }
    }
    }
}

/*function writes to object file the first word for each instruction line*/
void FirstTokenToBinary(Line *line, FILE* testfile, char* filename )
{ 
    char binaryCode[WORD_SIZE] = "";
    char *temp;
    temp = CodeToBinary(line->content.inst.opcode,4);
    strcat(binaryCode, temp ); /*bits 11-14 are opcode */
    free(temp);
    switch (line->content.inst.operands_count) 
    {
        case 0:
            temp =  CodeToBinary(0,8);
            strcat(binaryCode,temp); /* source and destination are irrelevant */
            free(temp);

            temp =  CodeToBinary(4,3);
            strcat(binaryCode,temp); /* A=1, R,E=0 */
            free(temp);
            WriteLine(binaryCode, testfile, filename );
            break;

        case 1:
            temp =  CodeToBinary(0,4);
            strcat(binaryCode,temp); 
            free(temp);

            temp =  CodeToBinary(line->content.inst.operand_types[0],4);/*dest operand adressing type to binary */
            strcat(binaryCode,temp); 
            free(temp);

            temp = CodeToBinary(4,3);
            strcat(binaryCode, temp); /* A=1, R,E=0 */
            free(temp);

            WriteLine(binaryCode, testfile, filename );
            break;


        case 2:
            temp = CodeToBinary(line->content.inst.operand_types[0],4);  /*source operand adressing type to binary */
            strcat(binaryCode, temp); /* A=1, R,E=0 */
            free(temp);

            temp = CodeToBinary(line->content.inst.operand_types[1],4); /*dest operand adressing type to binary */
            strcat(binaryCode, temp); /* A=1, R,E=0 */
            free(temp);


            temp = CodeToBinary(4,3);
            strcat(binaryCode, temp); /* A=1, R,E=0 */
            free(temp);

            WriteLine(binaryCode, testfile, filename );
            break;

        default:
        printf("error with first token switch case");
            break; /*error message? */
    }
    
}

int hasCommonWord(Line *line) {
    if ((line->content.inst.operand_types[0] == ADD_INDIRECT_REGISTER || line->content.inst.operand_types[0] == ADD_REGISTER) && (line->content.inst.operand_types[1] == ADD_INDIRECT_REGISTER || line->content.inst.operand_types[1] == ADD_REGISTER)) {
        return 1;
    } else {
        return 0;
    }
}

char* CodeToBinary(int input, int numberOfBits) 
{
    int i = numberOfBits - 1;
    int value = input;
    char *binary = (char *)calloc(numberOfBits+1,sizeof(char)); /*/*free??? */
    for (; i >= 0; i--) 
    {
 
        binary[i] = (value & 1) ? '1' : '0';
        value >>= 1;
    }

    return binary;

}

int BinaryToOctal(char binarycode[]) {
    int j, i;
    int oct;
    int len;

    oct = 0;
    i = 0;

    len = strlen(binarycode);
    while (i < len) {
        int rem = 0;
        for (j = 0; j < 3 && i < len; j++, i++) {
            rem = rem * 2 + (binarycode[i] - '0');
        }
        oct = oct * 10 + rem;
    }
    return oct;
}

void WriteLine(char *binarycode, FILE* testfile, char* filename) /*should be void?*/
{

/* 4 digits for address, 1 space, 5 digits to octal */  
    int octal;
    int adr;
    int data_adr;
    FILE *file;

    data_adr = get_IC() + data_counter;
    octal = BinaryToOctal(binarycode);
    if (strcmp(filename, "data.ob") == 0) {
        adr = data_adr;
    } else {
        adr = address;
    }

    file = fopen(filename, "a");
    fprintf(file, "%04d %05d\n", adr, octal); /* 1 to address */
    fclose(file);
    address++;
}

void entryFile( char *filename) { /* ps */
    int i;
    char entry_name[MAX_FILENAME]; /* entry file name */
    FILE *entry_file;
    symbol *symbol_table;
    int symbolCount;

    symbol_table = get_symbol_table(); /* get the symbol table */
    symbolCount = get_symbol_count();  /* get the symbol count */
    append_extension(entry_name, filename, "ent");

    
    for (i = 0; i < symbolCount; i++) {

        if (symbol_table[i].type == SYMBOL_ENTRY) {
            if (access("test.ent", F_OK) != -1) { /*checks if file already exists*/
                entry_file = fopen(entry_name, "a");
                fprintf(entry_file, "%s %d\n", symbol_table[i].name, symbol_table[i].value);
                fclose(entry_file);

            } else {
                entry_file = fopen(entry_name, "w"); /*create entry file*/
                fclose(entry_file);
                entry_file = fopen(entry_name, "a");
                fprintf(entry_file, "%s %d\n", symbol_table[i].name, symbol_table[i].value);
                fclose(entry_file);
            }
        }
    }
}

void externFile(symbol symbol ,  char *filename) {
    FILE *extern_file;
    char extern_name[MAX_FILENAME]; 

    append_extension(extern_name, filename, "ext");

    if (access("test.ext", F_OK) != -1) { /*checks if file already exists*/

        extern_file = fopen(extern_name, "a");
        fprintf(extern_file, "%s %d\n", symbol.name, address);
        fclose(extern_file);

    } else {

        extern_file = fopen(extern_name, "w"); /*create entry file*/
        fclose(extern_file);
        extern_file = fopen(extern_name, "a");
        fprintf(extern_file, "%s %d\n", symbol.name, address);
        fclose(extern_file);
    }
}
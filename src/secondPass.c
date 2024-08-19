
#include <unistd.h>

#include "secondPass.h"
#include "memory_manager.h"
#include "parser.h"
#include "tokenizer.h"

#define STARTING_ADDRESS 100
#define WORD_SIZE 20


int address = STARTING_ADDRESS;
int data_counter;
int error_flag;

/*responsible for the second pass */
int secondPass( char *filename, ParsedProgram *program, int numOfLines);
void generate_ob_file( char *filename, char *ins_name, char *data_name);
char* CodeToBinary(int input, int numberOfBits);
int BinaryToOctal(char binarycode[]);
void DataToBinary(Line *line, char* filename);
void InstructionToBinary(Line *line, char *ins_name ,char *file_extern);
void FirstTokenToBinary(Line *line, char *ins_name );
int hasCommonWord(Line *line);
void WriteLine(char *binarycode, char *filename);
void entryFile( char *filename);
void externFile(symbol symbol , char *filename, int type);
void error_accured(char *filename);



int secondPass( char *origin_name, ParsedProgram *program, int numOfLines) {
    
    char ins_name[MAX_FILENAME]; 
    char data_name[MAX_FILENAME]; 

    int i;
    
    strcpy(ins_name, "ins.ob");
    strcpy(data_name, "data.ob");
    error_flag = 0;
    data_counter = 0;



    i = 0;
    for (; i < numOfLines; i++) {
        Line *current = &(program->lines[i]);

        if (current->type == LINE_INSTRUCTION) /*instruction line*/
        {
            InstructionToBinary(current, ins_name ,origin_name);        /*lines of instruction*/
        } else if (current->type == LINE_DATA || current->type == 2)            /*line_data, line_string*/
        {
            DataToBinary(current, data_name); /*lines of data*/
        }
    }

    generate_ob_file(origin_name, ins_name, data_name);
    
    /*creates and write to .ent file*/
    entryFile(origin_name); 

    return (error_flag == 0);

    if (error_flag==1)
    {
        error_accured(origin_name);
    }
    
    return (error_flag == 0);
    
}

void generate_ob_file( char* filename, char* ins_name, char* data_name) {
    
    char c;
    FILE *file, *ins, *data;
    char ob_file[MAX_FILENAME]; /* entry file name */

    append_extension(ob_file, filename, ".ob");
    
    if (error_flag == 0)
    {    
        file = fopen(ob_file, "w");
        if (file == NULL) {
        printf("error while openening object file.\n");
        error_flag = 1;
        return;
    }
        fprintf(file, "%d %d\n", get_IC() - INITIAL_ADDRESS, get_DC());
        fclose(file);


        if (access(ins_name, F_OK) != -1) { /*checks if thare are any instruction is .as file*/
            ins = fopen(ins_name, "r");
            if (ins == NULL) {
                printf("Error while opening file\n");
                error_flag = 1;
                return;
            }

            file = fopen(ob_file, "a");
            if (file == NULL) {
                printf("Error while opening object file.\n");
                fclose(ins);  /* Close the other file before returning */
                return; 
            }
            while ((c = fgetc(ins)) != EOF){
            fputc(c, file);
            }
            fclose(ins);
            fclose(file);
        }
       
        if (access(data_name, F_OK) != -1) { /*checks if thare are any data is .as file*/
            data = fopen(data_name, "r");
            if (data == NULL) {
                printf("Error while opening file\n");
                error_flag = 1;
                return;
            }

            file = fopen(ob_file, "a");
            if (file == NULL) {
                printf("Error while opening object file.\n");
                fclose(data);  /* Close the other file before returning */
                return; 
            }
            while ((c = fgetc(data)) != EOF){
            fputc(c, file);
            }
            fclose(data);
            fclose(file);
        }

        remove(ins_name);
        remove(data_name);
        
    }
    
}

void DataToBinary(Line *line, char* filename){ /*extrenFile()*/
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
            WriteLine(binaryCode,  filename);
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
            WriteLine(binaryCode, filename);
            data_counter++;
            i++;
        }
        memset(binaryCode, 0, sizeof(binaryCode));
        temp =  CodeToBinary(0,15);
        strcat(binaryCode,temp); /*print /0 to end string*/ 
        free(temp);
        WriteLine(binaryCode, filename);
        data_counter++;
        break;

    default:
        break;
    }
}

void InstructionToBinary(Line *line, char *ins_name ,char *file_extern  ) {
    int i;
    int j;
    char *temp;
    symbol *symbol_table;
    int symbolCount;
    char binaryCode[WORD_SIZE] = "";

    symbol_table = get_symbol_table(); /* get the symbol table */
    symbolCount = get_symbol_count();  /* get the symbol count */

    FirstTokenToBinary(line, ins_name); 

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
        WriteLine(binaryCode, ins_name);
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
                WriteLine(binaryCode, ins_name); /* writes the first word,TODO: write function that appends line as address and octal value of binary*/
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
                            externFile(symbol_table[j], file_extern, 1);
                        } 
                        else
                        {
                            temp = CodeToBinary(2,3);
                            strcat(binaryCode, temp ); /*  A=0  R=1  E=0 */
                            free(temp);
                        }
                        WriteLine(binaryCode, ins_name); 
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

                WriteLine(binaryCode, ins_name);

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
                WriteLine(binaryCode, ins_name);
            }
            break;
        default:
            break;
        }
    }
    }
}

/*function writes to object file the first word for each instruction line*/
void FirstTokenToBinary(Line *line, char *ins_name )
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
            WriteLine(binaryCode, ins_name );
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

            WriteLine(binaryCode, ins_name );
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

            WriteLine(binaryCode, ins_name );
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

void WriteLine(char *binarycode, char *filename) 
{
    
/* 4 digits for address, 1 space, 5 digits to octal */  
    int octal;
    int adr;
    int data_adr;
    FILE *file;

    data_adr = get_IC() + data_counter;
    octal = BinaryToOctal(binarycode);
    if (strcmp("ins.ob",filename)==0) { /*instruction*/
        adr = address;
    } else {
        adr = data_adr;
    }

    file = fopen(filename, "a");
    fprintf(file, "%04d %05d\n", adr, octal); 
    fclose(file);
    address++;
}

void entryFile( char *filename) { 
    int i;
    char entry_name[MAX_FILENAME]; /* entry file name */
    FILE *entry_file;
    symbol *symbol_table;
    int symbolCount;

    symbol_table = get_symbol_table(); /* get the symbol table */
    symbolCount = get_symbol_count();  /* get the symbol count */
    append_extension(entry_name, filename, ".ent");

    
    for (i = 0; i < symbolCount; i++) {

        if (symbol_table[i].type == SYMBOL_ENTRY) {
            if (access(entry_name, F_OK) != -1) { /*checks if file already exists*/
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

void externFile(symbol symbol , char *filename, int type) {
    FILE *extern_file;
    int adr;
    char extern_name[MAX_FILENAME]; 
    int data_adr;

    append_extension(extern_name, filename, ".ext");

    data_adr = get_IC() + data_counter;
    if (type == 1) { /*instruction*/
        adr = address;
    } else {
        adr = data_adr;
    }

    if (access(extern_name, F_OK) != -1) { /*checks if file already exists*/

        extern_file = fopen(extern_name, "a");
        fprintf(extern_file, "%s %d\n", symbol.name, adr);
        fclose(extern_file);

    } else {

        extern_file = fopen(extern_name, "w"); /*create entry file*/
        fclose(extern_file);
        extern_file = fopen(extern_name, "a");
        fprintf(extern_file, "%s %d\n", symbol.name, adr);
        fclose(extern_file);
    }
}

void error_accured(char *filename){

    char file[MAX_FILENAME]; 
    append_extension(file, filename, ".ob");
    if (access(file, F_OK) != -1) {
        remove(file); 
    }
    strcpy(file, "");

    append_extension(file, filename, ".ent");
    if (access(file, F_OK) != -1) {
            remove(file);

    }
    strcpy(file, "");

        append_extension(file, filename, ".ext");
    if (access(file, F_OK) != -1) {
        remove(file);

    }
}


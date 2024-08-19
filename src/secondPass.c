
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

int secondPass( char *filename, ParsedProgram *program, int numOfLines);
void generate_ob_file( char *filename, char *ins_name, char *data_name);
char* CodeToBinary(int input, int numberOfBits);
int BinaryToOctal(char binarycode[]);
void DataToBinary(Line *line, char* data_name);
void InstructionToBinary(Line *line, char *ins_name ,char *file_extern);
void FirstTokenToBinary(Line *line, char *ins_name );
int hasCommonWord(Line *line);
void WriteLine(char *binarycode, char *filename);
void entryFile( char *filename);
void externFile(symbol symbol , char *filename);
void error_accured(char *filename);



/*this function is responsible for the second pass of the assembly*/
int secondPass( char *origin_name, ParsedProgram *program, int numOfLines) {
    
    char ins_name[MAX_FILENAME]; 
    char data_name[MAX_FILENAME]; 

    int i;
    
    strcpy(ins_name, "ins.ob");
    strcpy(data_name, "data.ob");
    error_flag = 0;
    data_counter = 0;



    i = 0;
    for (; i < numOfLines; i++) {   /*runs through the program line by line*/
        Line *current = &(program->lines[i]);

        if (current->type == LINE_INSTRUCTION)    /*if instruction line*/
        {
            InstructionToBinary(current, ins_name ,origin_name);        
        } else if (current->type == LINE_DATA || current->type == LINE_STRING)    /*if line_data or line_string*/
        {
            DataToBinary(current, data_name); 
        }
    }


    
    
    /*creates and writes the .ob and .ent file*/
    if (error_flag==0){
    entryFile(origin_name); 
    generate_ob_file(origin_name, ins_name, data_name); 
    }
    else  /*erase other files if created*/
    {
        error_accured(origin_name);
    }
    
    return (error_flag == 0);

}

/*called at the end of the second pass and generates the .ob file*/
void generate_ob_file( char* filename, char* ins_name, char* data_name) {
    
    char c;
    FILE *file, *ins, *data;
    char ob_file[MAX_FILENAME]; 

    append_extension(ob_file, filename, ".ob");  /*create the correct file name*/
    
    file = fopen(ob_file, "w");
    if (file == NULL) {
    printf("error while openening object file.\n");
    error_flag = 1;
    return;
    }
    fprintf(file, "%d %d\n", get_IC() - INITIAL_ADDRESS, get_DC());
    fclose(file);

    if (access(ins_name, F_OK) != -1)  /*first writes all instruction lines to .ob file if there are any*/
    { /*checks if thare are any instruction is .as file*/
        ins = fopen(ins_name, "r");
        if (ins == NULL)
        {
            printf("Error while opening file\n");
            error_flag = 1;
            return;
        }

        file = fopen(ob_file, "a");
        if (file == NULL)
        {
            printf("Error while opening object file.\n");
            fclose(ins); /* Close the instruction file */
            return;
        }
        while ((c = fgetc(ins)) != EOF)
        {
            fputc(c, file);
            }
            fclose(ins);
            fclose(file);
    }
       
        if (access(data_name, F_OK) != -1) {  /*writes all data lines to .ob file if there are any*/
            data = fopen(data_name, "r");
            if (data == NULL) {
                printf("Error while opening file\n");
                error_flag = 1;
                return;
            }

            file = fopen(ob_file, "a");
            if (file == NULL) {
                printf("Error while opening object file.\n");
                fclose(data);  /* Close the data file */
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
   
/*this function is responsible for translating the data lines to binary code stored in binarycode[]*/
void DataToBinary(Line *line, char* filename){ 
    int i;
    int j;
    char c;
    char str[MAX_TOKEN_LENGTH] = "";
    char binaryCode[WORD_SIZE] = "";
    char *temp;
    
    
    switch (line->content.data.type)
    {
    case DATA_INT: /*if .data line, which means int variebles*/
        for (i=0;i < line->content.data.value_count; i++)
        {
            memset(binaryCode, 0, sizeof(binaryCode)); /*reset binarycode*/
            
            temp = CodeToBinary(line->content.data.content.int_values[i],15); /*each int takes the whole 'word', 15 bits*/
            strcat(binaryCode, temp);
            free(temp);
            WriteLine(binaryCode,  filename);
            data_counter++;

        }
    break; /* extern label  */

    case DATA_STRING:
        i = 0;
        while (line->content.data.content.char_values[i] != '\0') {
            memset(binaryCode, 0, sizeof(binaryCode));   /*reset binarycode*/
            memset(str, 0, sizeof(str));                 /*reset temp*/
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

/*this function is responsible for translating the instruction lines to binary code stored in binarycode[], also uses FirstTokenToBinary*/
void InstructionToBinary(Line *line, char *ins_name ,char *file_extern  ) {
    int i;
    int j;
    char *temp;
    symbol *symbol_table;
    int symbolCount;
    char binaryCode[WORD_SIZE] = "";

    symbol_table = get_symbol_table(); /* get the symbol table */
    symbolCount = get_symbol_count();  /* get the symbol count */

    FirstTokenToBinary(line, ins_name); /*generate and write the first 'word' for the instrauction line*/

    if ((line->content.inst.operands_count == 2) && hasCommonWord(line)) /*if both operands are *reg or reg they have a common word*/
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
    for ( i = 0; i < line->content.inst.operands_count; i++)  /*if create the 'extra-word' for each operand depanding on their AddressingMode */
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
                WriteLine(binaryCode, ins_name); 
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
            error_flag = 1;
            break;
        }
    }
    }
}

/*function gets the instruction line and creates the first 'word' for the line*/
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

            temp =  CodeToBinary(line->content.inst.operand_types[0],4); /*dest operand adressing type to binary */
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
            error_flag = 1;
            break; /*error message? */
    }
    
}

/*function checks if instruction line uses a common word for both operands*/
int hasCommonWord(Line *line) {
    if ((line->content.inst.operand_types[0] == ADD_INDIRECT_REGISTER || line->content.inst.operand_types[0] == ADD_REGISTER) && (line->content.inst.operand_types[1] == ADD_INDIRECT_REGISTER || line->content.inst.operand_types[1] == ADD_REGISTER)) {
        return 1;
    } else {
        return 0;
    }
}

/*helper function, gets the value and the amount of bits it needs to be stored in and returns it an binary[] */
char* CodeToBinary(int input, int numberOfBits) 
{
    int i = numberOfBits - 1;
    int value = input;
    char *binary = (char *)calloc(numberOfBits+1,sizeof(char));  /*freed by calling function*/
    for (; i >= 0; i--) 
    {
 
        binary[i] = (value & 1) ? '1' : '0';
        value >>= 1;
    }

    return binary;

}

/*helper function, recives the binarycode and translates it to octal*/
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

/*writes the formated line to data or instruction files*/
void WriteLine(char *binarycode, char *filename) 
{
    int octal;
    int adr;
    int data_adr;
    FILE *file;

    octal = BinaryToOctal(binarycode);
    if (strcmp("ins.ob",filename)==0) { /*instruction*/
        adr = address;
    } else {
        data_adr = get_IC() + data_counter;
        adr = data_adr;
    }

    file = fopen(filename, "a");
    fprintf(file, "%04d %05d\n", adr, octal); /* 4 digits for address, 1 space, 5 digits to octal */ 
    fclose(file);
    address++;
}

/*creates entry file if needed by passing through the symbol tables and looking for ENTRY_LABEL*/
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
            if (access(entry_name, F_OK) != -1) { /*if file exists no need to create it only append to it*/
                entry_file = fopen(entry_name, "a");
                fprintf(entry_file, "%s %d\n", symbol_table[i].name, symbol_table[i].value);
                fclose(entry_file);

            } else {
                entry_file = fopen(entry_name, "w"); /*create entry file and append first line*/
                fclose(entry_file);
                entry_file = fopen(entry_name, "a");
                fprintf(entry_file, "%s %d\n", symbol_table[i].name, symbol_table[i].value);
                fclose(entry_file);
            }
        }
    }
}

/*creates extern file if needed, function is called in InstructionToBinary if EXTERN_TABLE is found as an operand*/
void externFile(symbol symbol , char *filename) {
    FILE *extern_file;
    char extern_name[MAX_FILENAME]; 

    append_extension(extern_name, filename, ".ext"); /*create correct file name for .ext file*/


    if (access(extern_name, F_OK) != -1) { /*if file exists no need to create it only append to it*/

        extern_file = fopen(extern_name, "a");
        fprintf(extern_file, "%s %d\n", symbol.name, address);
        fclose(extern_file);

    } else {

        extern_file = fopen(extern_name, "w"); /*create extren file and append first line*/
        fclose(extern_file);
        extern_file = fopen(extern_name, "a");
        fprintf(extern_file, "%s %d\n", symbol.name, address);
        fclose(extern_file);
    }
}

/*if error accured during the second pass deletes the all the files created so far*/
void error_accured(char *filename){

    char file[MAX_FILENAME]; 
    char data_name[MAX_FILENAME]; 
    char ins_name[MAX_FILENAME]; 

    strcpy(file, "");
    strcpy(data_name, "data.ob");
    strcpy(ins_name, "ins.ob");

    append_extension(file, filename, ".ext");
    if (access(file, F_OK) != -1) {
        remove(file);
    }

    if (access(data_name, F_OK) != -1) {
        remove(data_name); 
    }

    if (access(ins_name, F_OK) != -1) {
        remove(ins_name); 
    }
}


#ifndef SECONDPASS_H
#define SECONDPASS_H

#include "parsed_program.h"
#include "symbolTable.h"

/*this function is responsible for the second pass of the assembly*/
int secondPass(char *filename, ParsedProgram *program, int numOfLines);

/*called at the end of the second pass and generates the .ob file*/
void generate_ob_file(char *filename, char *ins_name, char *data_name);

/*helper function, gets the value and the amount of bits it needs to be stored in and returns it an binary[] */
char* CodeToBinary(int input, int numberOfBits);

/*helper function, recives the binarycode and translates it to octal*/
int BinaryToOctal(char binarycode[]);

/*this function is responsible for translating the data lines to binary code stored in binarycode[]*/
void DataToBinary(Line *line, char* filename);

/*this function is responsible for translating the instruction lines to binary code stored in binarycode[], also uses FirstTokenToBinary*/
void InstructionToBinary(Line *line, char *ins_name ,char *file_extern);

/*function gets the instruction line and creates the first 'word' for the line*/
void FirstTokenToBinary(Line *line, char *ins_name );

/*function checks if instruction line uses a common word for both operands*/
int hasCommonWord(Line *line);

/*writes the formated line to data or instruction files*/
void WriteLine(char *binarycode, char *filename);

/*creates entry file if needed by passing through the symbol tables and looking for ENTRY_LABEL*/
void entryFile(char *filename);

/*creates extern file if needed, function is called in InstructionToBinary if EXTERN_TABLE is found as an operand*/
void externFile(symbol symbol , char *filename);

/*if error accured during the second pass deletes the all the files created so far*/
void error_accured(char *filename);

#endif 
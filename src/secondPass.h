#ifndef SECONDPASS_H
#define SECONDPASS_H

#include "parsed_program.h"
#include "symbolTable.h"

int secondPass(char *filename, ParsedProgram *program, int numOfLines);
void generate_ob_file(char *filename, char *ins_name, char *data_name);
char* CodeToBinary(int input, int numberOfBits);
int BinaryToOctal(char binarycode[]);
void DataToBinary(Line *line, char* filename);
void InstructionToBinary(Line *line, char *ins_name ,char *file_extern);
void FirstTokenToBinary(Line *line, char *ins_name );
int hasCommonWord(Line *line);
void WriteLine(char *binarycode, char *filename);
void entryFile(char *filename);
void externFile(symbol symbol , char *filename, int type);
void error_accured(char *filename);

#endif 
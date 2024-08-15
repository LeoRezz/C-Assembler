#ifndef PARSED_PROGRAM_H
#define PARSED_PROGRAM_H

#include "tokenizer.h" /* For Line definition */

#define INITIAL_CAPACITY 10

typedef struct ParsedProgram {
    Line* lines;
    int size;
    int capacity;
} ParsedProgram;

ParsedProgram* create_parsed_program(void);
void grow_parsed_program(ParsedProgram* program);
void add_line_to_program(ParsedProgram* program, Line* line);
void free_parsed_program(ParsedProgram* program);

#endif /* PARSED_PROGRAM_H */
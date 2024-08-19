#ifndef PARSED_PROGRAM_H
#define PARSED_PROGRAM_H

#include "tokenizer.h" /* For Line definition */

#define INITIAL_CAPACITY 10

typedef struct ParsedProgram {
    Line *lines;
    int count; /* Current lines number */
    int capacity; /* For realloc */
} ParsedProgram;

/* intitilzzes space for the lines array  */
ParsedProgram* init_parsed_program(void);
/* Realloctes space for the array */
void grow_parsed_program(ParsedProgram* program);
/* Adds lines to the program line's array */
void add_line_to_program(ParsedProgram* program, Line* line);
/* Fress after we are done with the memory allocated */
void free_parsed_program(ParsedProgram* program);
/* Updates the data lines to the current IC */
void update_data_lines(int IC, ParsedProgram *ParsedProgram);

#endif /* PARSED_PROGRAM_H */
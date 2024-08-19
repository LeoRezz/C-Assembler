#include "parsed_program.h"
#include "opcodeTable.h"
#include "line.h"
#include <stdlib.h>

#define INITIAL_ADDRESS 100

/* Initilzzes space for the lines array */
ParsedProgram *init_parsed_program() {

    ParsedProgram *program;

    program = (ParsedProgram *)calloc(1, sizeof(ParsedProgram));
    if (program == NULL)
        return NULL;

    program->lines = (Line *)calloc(INITIAL_CAPACITY, sizeof(Line));
    if (program->lines == NULL) {
        free(program);
        return NULL;
    }

    program->count = 0;
    program->capacity = INITIAL_CAPACITY;
    return program;
}

void grow_parsed_program(ParsedProgram *program) {

    int new_capacity;
    Line *new_lines;

    new_capacity = program->capacity * 2;
    new_lines = (Line *)realloc(program->lines, new_capacity * sizeof(Line));
    if (new_lines == NULL) {
        /* Handle reallocation error*/
        printf("Failed to reallocate memory for symbol_table\n");
        exit(1);
    }
    program->lines = new_lines;
    program->capacity = new_capacity;
}

void add_line_to_program(ParsedProgram *program, Line *line) {

    if (program->count >= program->capacity) {
        grow_parsed_program(program);
    }
    if (line != NULL) {

        program->lines[program->count] = *line;
        program->count++;
    }
}

void update_data_lines(int IC, ParsedProgram *ParsedProgram) {
    int i;
    for (i = 0; i < ParsedProgram->count; i++) {
        if (ParsedProgram->lines[i].type == LINE_DATA || ParsedProgram->lines[i].type == LINE_STRING) {
            ParsedProgram->lines[i].address += IC; /* DC = DC + IC */
        }
    }
}

void free_parsed_program(ParsedProgram *program) {
    
    if (program != NULL) {
        free(program->lines);
        free(program);
    }
}
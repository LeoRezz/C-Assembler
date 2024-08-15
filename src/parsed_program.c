#include "parsed_program.h"
#include <stdlib.h>

ParsedProgram *create_parsed_program() {

    ParsedProgram *program;

    program = (ParsedProgram *)calloc(1, sizeof(ParsedProgram));
    if (program == NULL)
        return NULL;

    program->lines = (Line *)calloc(INITIAL_CAPACITY, sizeof(Line));
    if (program->lines == NULL) {
        free(program);
        return NULL;
    }

    program->size = 0;
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

    if (program->size == program->capacity) {
        grow_parsed_program(program);
    }
    if (program->size < program->capacity) {
        program->lines[program->size] = *line;
        program->size++;
    }
    /* If we can't grow and we're at capacity, the line is not added */
}

void free_parsed_program(ParsedProgram *program) {
    
    if (program != NULL) {
        free(program->lines);
        free(program);
    }
}
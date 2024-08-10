/* In your implementation file, e.g., program.c */
#include <stdlib.h>
#include "program.h"

struct ParsedProgram* create_parsed_program(void)
{
    struct ParsedProgram* program;
    program = (struct ParsedProgram*)malloc(sizeof(struct ParsedProgram));
    if (program != NULL) {
        int i;
        program->line_count = 0;
        for (i = 0; i < MAX_PROGRAM_LINES; i++) {
            program->lines[i] = NULL;
        }
    }
    return program;
}

void add_line_to_program(struct ParsedProgram* program, struct Line* line)
{
    if (program != NULL && program->line_count < MAX_PROGRAM_LINES) {
        program->lines[program->line_count] = line;
        program->line_count++;
    }
}

void free_parsed_program(struct ParsedProgram* program)
{
    int i;
    if (program != NULL) {
        for (i = 0; i < program->line_count; i++) {
            free(program->lines[i]);
        }
        free(program);
    }
}
#include "parsed_program.h"
<<<<<<< HEAD
#include <stdlib.h>

=======
#include "opcodeTable.h"
#include <stdlib.h>
#define INITIAL_ADDRESS 100

void print_binary(int num, int bits);
>>>>>>> sandbox
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
<<<<<<< HEAD
    if (program->size < program->capacity) {
        program->lines[program->size] = *line;
        program->size++;
    }
    /* If we can't grow and we're at capacity, the line is not added */
=======
   
        program->lines[program->size] = *line;
        program->size++;   
}

void update_data_lines(int IC, ParsedProgram *ParsedProgram) {
    int i;
    for (i = 0; i < ParsedProgram->size; i++) {
        if (ParsedProgram->lines[i].type == LINE_DATA || ParsedProgram->lines[i].type == LINE_STRING) {
            ParsedProgram->lines[i].address += IC;
        }
    }
>>>>>>> sandbox
}

void free_parsed_program(ParsedProgram *program) {
    
    if (program != NULL) {
        free(program->lines);
        free(program);
    }
<<<<<<< HEAD
=======
}

void print_parsed_program(const ParsedProgram* program)
{
    int i;
    
    if (program == NULL)
    {
        printf("Parsed program is NULL\n");
        return;
    }
    printf("\n\n-------------------------------\n");
    printf("Parsed Program:\n");
    printf("Total lines: %d\n", program->size);
    printf("-------------------------------\n");

    for (i = 0; i < program->size; i++)
    {
        Line* line = &(program->lines[i]);
        printf("Line %d (Address: %d):\n", line->line_number, line->address);
        printf("  Type: ");
        
        switch(line->type)
        {
            case LINE_INSTRUCTION:
                printf("Instruction\n");
                printf("  Opcode: %s ", opcode_to_string(line->content.inst.opcode));
                print_binary(line->content.inst.opcode, 4);
                printf("\n");
                printf("  Operands: %d\n", line->content.inst.operands_count);
                /* Add more details about operands if needed */
                break;
            case LINE_DATA:
                printf("Data\n");
                printf("  Type: %s\n", line->content.data.type == DATA_INT ? "Integer" : "String");
                printf("  Value count: %d\n", line->content.data.value_count);
                /* Add more details about data values if needed */
                break;
            case LINE_STRING:
                printf("String\n");
                printf("  Value: %s\n", line->content.data.content.char_values);
                break;
            case LINE_DIRECTIVE:
                printf("Directive\n");
                /* Add more details about directives if needed */
                break;
            default:
                printf("Unknown\n");
        }

        if (line->label[0] != '\0')
        {
            printf("  Label: %s\n", line->label);
        }

        printf("-------------------------------\n");
    }

    
}

void print_binary(int num, int bits) {
    int i;
    for (i = bits - 1; i >= 0; i--) {
        printf("%d", (num >> i) & 1);
    }
>>>>>>> sandbox
}
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "secondPass.h"
#include "symbolTable.h"
#include "opcodeTable.h"
#include "memory_manager.h"
#include "parsed_program.h"

#define WORD_SIZE 15
#define A_R_E_BITS 3

/* Function prototypes */
static void write_instruction(FILE *fp_ob, FILE *fp_ext, Line *line);
static void write_data(FILE *fp_ob, Line *line);
static void write_entry(FILE *fp_ent, Line *line);

void second_pass(ParsedProgram *program, const char *ob_filename, const char *ent_filename, const char *ext_filename)
{
    int i;
    FILE *fp_ob, *fp_ext, *fp_ent;
    
    fp_ob = fopen(ob_filename, "w");
    fp_ext = fopen(ext_filename, "w");
    fp_ent = fopen(ent_filename, "w");

    if (!fp_ob || !fp_ext || !fp_ent) {
        printf("Error opening output files\n");
        return;
    }

    /* Write first line of .ob file with instruction and data word counts */
    fprintf(fp_ob, "%d %d\n", get_IC() - INITIAL_ADDRESS, get_DC());

    for (i = 0; i < program->count; i++) {
        Line *line = &(program->lines[i]);
        
        switch(line->type) {
            case LINE_INSTRUCTION:
                write_instruction(fp_ob, fp_ext, line);
                break;
            case LINE_DATA:
            case LINE_STRING:
                write_data(fp_ob, line);
                break;
            case LINE_ENTRY:
                write_entry(fp_ent, line);
                break;
            case LINE_EXTERN:
                /* External symbols are handled when writing instructions */
                break;
        }
    }

    fclose(fp_ob);
    fclose(fp_ext);
    fclose(fp_ent);
}

static void write_instruction(FILE *fp_ob, FILE *fp_ext, Line *line)
{
    int word_count = 1; /* Start with 1 for the instruction word */
    unsigned int instruction_word = 0;
    unsigned int operand_word = 0;
    int i;
    symbol *sym;
    
    /* Build the first word of the instruction */
    instruction_word |= (line->content.inst.opcode & 0xF) << 11;
    instruction_word |= (line->content.inst.operand_types[0] & 0xF) << 7;
    instruction_word |= (line->content.inst.operand_types[1] & 0xF) << 3;
    instruction_word |= 0x4; /* A=1, R=0, E=0 */

    fprintf(fp_ob, "%04d %05o\n", line->address, instruction_word);

    /* Write operand words */
    for (i = 0; i < line->content.inst.operands_count; i++) {
        operand_word = 0;
        switch (line->content.inst.operand_types[i]) {
            case ADD_IMMEDIATE:
                operand_word = (line->content.inst.operands[i].immediate & 0xFFF) << 3;
                operand_word |= 0x4; /* A=1, R=0, E=0 */
                break;
            case ADD_DIRECT:
            case ADD_INDIRECT_REGISTER:
                sym = find_symbol(line->content.inst.operands[i].symbol);
                if (sym) {
                    operand_word = (sym->value & 0xFFF) << 3;
                    if (sym->type == SYMBOL_EXTERNAL) {
                        operand_word |= 0x1; /* A=0, R=0, E=1 */
                        fprintf(fp_ext, "%s %04d\n", sym->name, line->address + word_count);
                    } else {
                        operand_word |= 0x2; /* A=0, R=1, E=0 */
                    }
                } else {
                    printf("Error: Symbol %s not found\n", line->content.inst.operands[i].symbol);
                }
                break;
            case ADD_REGISTER:
                operand_word = (line->content.inst.operands[i].reg & 0x7) << 3;
                operand_word |= 0x4; /* A=1, R=0, E=0 */
                break;
        }
        fprintf(fp_ob, "%04d %05o\n", line->address + word_count, operand_word);
        word_count++;
    }
}

static void write_data(FILE *fp_ob, Line *line)
{
    int i;
    unsigned int data_word;

    if (line->type == LINE_DATA) {
        for (i = 0; i < line->content.data.value_count; i++) {
            data_word = (line->content.data.content.int_values[i] & 0x7FFF) << 3;
            data_word |= 0x4; /* A=1, R=0, E=0 */
            fprintf(fp_ob, "%04d %05o\n", line->address + i, data_word);
        }
    } else if (line->type == LINE_STRING) {
        for (i = 0; line->content.data.content.char_values[i] != '\0'; i++) {
            data_word = (line->content.data.content.char_values[i] & 0x7F) << 3;
            data_word |= 0x4; /* A=1, R=0, E=0 */
            fprintf(fp_ob, "%04d %05o\n", line->address + i, data_word);
        }
        /* Write null terminator */
        fprintf(fp_ob, "%04d %05o\n", line->address + i, 0x4);
    }
}

static void write_entry(FILE *fp_ent, Line *line)
{
    symbol *sym = find_symbol(line->label);
    if (sym) {
        fprintf(fp_ent, "%s %04d\n", sym->name, sym->value);
    } else {
        printf("Error: Entry symbol %s not found\n", line->label);
    }
}
#include "tokenizer.h"
#include "symbolTable.h"
#include "opcodeTable.h"
#include "parser.h"
#include "parsed_program.h"
#include "memory_manager.h"

void first_pass(FILE *input_file, ParsedProgram *parsed_program);
void print_token_arr(Token *token_arr, int token_count);

/* Global state, Instruction and Data count*/
int current_line = 0;

/* ----------------------TODO----------------------- */
/*
   Make sure macro names arn't declared as a label //add them to symbol table
   Make sure to fix the parser's addressing mode bugs, and comma handeling
   
   secondPass:
   Write data to binary
   Binary to octal
   Entry and extern files 
   Make sure instruction to binary is correct
   Make sure writeLine is correct
   Make sure code image isn't bigger than 4096
   Make sure to build an ob file, and possibly ext and ent if needed.
   
     */
    int main() {
    FILE *input_file;
    ParsedProgram *parsed_program;

    
    /* test of detecting diffrenet lexical errors, syntax errors will be detected in the parser */
    input_file = fopen("../input/asm1.am", "r");
    TRY(input_file); /* TRY macro checks for errors while opening file */
    init_memory_counters();
    parsed_program = init_parsed_program(); 
    init_symbol_table(); /* Initialize symbol table */
    first_pass(input_file, parsed_program);
  
    update_data_symbols(get_IC());

    update_data_lines(get_IC(), parsed_program);

    print_parsed_program(parsed_program);
    print_symbol_table();

    free_symbol_table();
    free_parsed_program(parsed_program);
    fclose(input_file);
    return 0;
}
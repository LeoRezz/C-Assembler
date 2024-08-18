#include "memory_manager.h"
#include "opcodeTable.h"
#include "parsed_program.h"
#include "parser.h"
#include "symbolTable.h"
#include "tokenizer.h"

void first_pass(FILE *input_file, ParsedProgram *parsed_program);
void print_token_arr(Token *token_arr, int token_count);

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
   Make sure to build an ob file, and possibly ext and ent if needed.

     */
int main() {
    ParsedProgram *parsed_program;
    char filename[100] = "ps.am";
    /* test of detecting diffrenet lexical errors, syntax errors will be detected in the parser */
   
    init_memory_counters();
    parsed_program = init_parsed_program();
    init_symbol_table(); /* Initialize symbol table */

    first_pass(filename, parsed_program);

    update_data_symbols(get_IC());

    update_data_lines(get_IC(), parsed_program);

    print_parsed_program(parsed_program);
    resolve_entrys();
    print_symbol_table();

    free_symbol_table();
    free_parsed_program(parsed_program);
    return 0;
}
#include "tokenizer.h"
#include "symbolTable.h"
#include "opcodeTable.h"
#include "parser.h"
#include "parsed_program.h"

void first_pass(FILE *input_file, ParsedProgram *parsed_program);
void print_token_arr(Token *token_arr, int token_count);

/* Global state, Instruction and Data count*/
int IC = INITIAL_ADDRESS;
int DC = 0;
int current_line = 0;
/* ----------------------TODO----------------------- */
/* Make sure to address to Extern and entry ! */
/* Make sure there is no extern and entry on the same label name
   Make sure macro names arn't declared as a label
   Make sure reserved words aren't valid label defenitions
   
   
   secondPass:
   Make sure code image isn't bigger than 4096
   Make sure to build an ob file, and possibly ext and ent if needed.
   
     */
int main() {
    FILE *input_file;
    ParsedProgram *parsed_program;

    
    /* test of detecting diffrenet lexical errors, syntax errors will be detected in the parser */
    input_file = fopen("../input/asm1_data_mix.am", "r");
    TRY(input_file); /* TRY macro checks for errors while opening file */

    parsed_program = create_parsed_program(); 
    init_symbol_table(); /* Initialize symbol table */
    first_pass(input_file, parsed_program);
  
    update_data_symbols(IC);

    update_data_lines(IC, parsed_program);

    print_parsed_program(parsed_program);
    print_symbol_table();

    free_symbol_table();
    free_parsed_program(parsed_program);
    return 0;
}
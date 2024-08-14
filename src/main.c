#include "tokenizer.h"
#include "symbolTable.h"
#include "opcodeTable.h"
#include "parser.h"
#define INITIAL_ADDRESS 100

void first_pass(FILE *input_file);
void print_token_arr(Token *token_arr, int token_count);

/* Global state, Instruction and Data count*/
int IC = INITIAL_ADDRESS;
int DC = 0;
int current_line = 0;

int main() {
    FILE *input_file;

    /* test of detecting diffrenet lexical errors, syntax errors will be detected in the parser */
    input_file = fopen("../input/data.am", "r");
    TRY(input_file); /* TRY macro checks for errors while opening file */
    initSymbolTable(); /* Initialize symbol table */
    first_pass(input_file);
    updateDataSymbols(IC);
    printSymbolTable();
    freeSymbolTable();
    return 0;
}
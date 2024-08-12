#include "tokenizer.h"
#include "symbolTable.h"
#include "opcodeTable.h"
#include "parser.h"
#define INITIAL_ADDRESS 100

/* Global state, Instruction and Data count*/
int IC = INITIAL_ADDRESS;
int DC = 0;
int current_line = 0;

/* prototypes: should be in header file*/
void first_pass(FILE *input_file);
void print_token_arr(Token *token_arr, int token_count);

/* Main for testing */
int main() {
    FILE *input_file;

    /* test of detecting diffrenet lexical errors, syntax errors will be detected in the parser */
    input_file = fopen("source_code.txt", "r");
    TRY(input_file); /* TRY macro checks for errors while opening file */
    initSymbolTable(); /* Initialize symbol table */
    first_pass(input_file);
    updateDataSymbols(IC);
    printSymbolTable();
    freeSymbolTable();
    return 0;
}
/* Main for testing */
void first_pass(FILE *input_file) {
    char *p;
    Token *token_arr;
    Line *parsed_line;

    int token_count;
    int i;
    char line[MAX_LINE];

    /* Read each line from the source file */
    while (fgets(line, MAX_LINE, input_file)) {
        if (p = strrchr(line, '\n')) *p = '\0'; /* Remove the newline character at the end*/

        /* Tokenizes a given line of assembly code into an array of tokens with assigned type. */
        token_arr = tokenize_line(line, &token_count);
		
		/* Parse the tokens in the line */
        parsed_line = parse_line(token_arr, token_count);


        print_parsed_line(parsed_line);
        printf("\n");
    }
    free(token_arr);
    free(parsed_line);
    fclose(input_file);
}


void print_token_arr(Token *token_arr, int token_count) {
    /* output printer */
    int i;
    for (i = 0; i < token_count; i++) {
        printf("%s\t%s\n", token_arr[i].value, token_type_to_string(token_arr[i].type));
    }
}

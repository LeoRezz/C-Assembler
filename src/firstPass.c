#include "tokenizer.h"
#include "symbolTable.h"
#include "opcodeTable.h"
#include "parser.h"

extern int IC, DC, current_line;

/* prototypes: should be in header file*/
void first_pass(FILE *input_file);
void print_token_arr(Token *token_arr, int token_count);

void first_pass(FILE *input_file) {
    char *p;
    Token *token_arr;
    Line *parsed_line;

    int token_count;
    char line[MAX_LINE];

    /* Read each line from the source file */
    while (fgets(line, MAX_LINE, input_file)) {
        if ((p = strrchr(line, '\n')) != NULL) *p = '\0'; /* Remove the newline character at the end*/

        /* Tokenizes a given line of assembly code into an array of tokens with assigned type. */
        /* TODO: check reserved words for label defenitionssssssss */
        token_arr = tokenize_line(line, &token_count);
        print_token_arr(token_arr, token_count);
        /* Parse the tokens in the line */
        parsed_line = parse_line(token_arr, token_count);
        printf("\n");
        print_parsed_line(parsed_line);

        printf("\n");
        printf("\n\n");
    }
    free(token_arr);
    free(parsed_line);
    fclose(input_file);
}


void print_token_arr(Token *token_arr, int token_count) {
    /* output printer */
    int i;
    printf("--------------------Tokenized result:-----------------------\n");    
    for (i = 0; i < token_count; i++) {
        printf("%s\t%s\n", token_arr[i].value, token_type_to_string(token_arr[i].type));
    }
    printf("------------------------------------------------------------\n\n\n");
}

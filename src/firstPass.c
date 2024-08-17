#include "tokenizer.h"
#include "symbolTable.h"
#include "opcodeTable.h"
#include "parser.h"
#include "parsed_program.h"


/* prototypes: should be in header file*/
void first_pass(FILE *input_file, ParsedProgram *parsed_program);
void print_token_arr(Token *token_arr, int token_count);

void first_pass(FILE *input_file, ParsedProgram *parsed_program ) {
    int current_line;
    char *p;
    Token *token_arr;
    Line *parsed_line;
    int token_count;
    char line[MAX_LINE];

    current_line = 0;
    token_count = 0;
    /* Read each line from the source file */
    while (fgets(line, MAX_LINE, input_file)) {
        current_line++;
        line[strcspn(line, "\r\n")] = '\0'; /* Remove the newline character at the end*/

        /* Tokenizes a given line of assembly code into an array of tokens with assigned type. */
        /* TODO: check reserved words for label defenitionssssssss */
        token_arr = tokenize_line(line, &token_count, current_line);
        if (token_arr == NULL) {
            printf("Line %d: '%s'\n\n", current_line, line);
            continue;
        }

        print_token_arr(token_arr, token_count);
        /* Parse the tokens in the line */
        if ((parsed_line = parse_line(token_arr, token_count, current_line)) == NULL) {
            printf("Invalid line %d: %s\n", current_line, line);
            free(token_arr);
            continue;
        }
        add_line_to_program(parsed_program, parsed_line);
        printf("\n");
        print_parsed_line(parsed_line);

        printf("\n");
        printf("\n\n");
        free(token_arr); /* Free the token array, no need for him*/
        free(parsed_line); /* Free the parsed line, because it was added to parsed_program */
    }
    
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

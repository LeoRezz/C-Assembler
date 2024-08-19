#include "firstPass.h"

/* This function is responsible for the first pass
 * it takes the input file, tokenizes and parses each line
 * it resposible of incrementing the IC and DC
 * and adding symbols to the symbol table
 * @return error_flag : 1 if an error occured, 0 otherwise
 */

int first_pass(char *am_filename, ParsedProgram *parsed_program) {
    int error_flag;
    int current_line;
    int is_preprocess;
    Token *token_arr;
    Line *parsed_line;
    int token_count;
    char line[MAX_LINE];
    FILE *am_file;

    am_file = fopen(am_filename, "r");
    if (!am_file) {
        printf("Failed to open input file: %s\n", am_filename);
        error_flag = 1;
        return 0;
    }
    parsed_line = NULL;
    error_flag = 0;
    current_line = 0;
    token_count = 0;
    is_preprocess = 0;

    /* Read each line from the source file */
    while (fgets(line, MAX_LINE, am_file)) {
        current_line++;

        line[strcspn(line, "\r\n")] = '\0'; /* Remove the newline character at the end*/

        /* Tokenizes a given line of assembly code into an array of tokens with assigned type. */
        token_arr = tokenize_line(line, &token_count, current_line, is_preprocess);
        if (token_arr == NULL) {
            printf("Line %d: '%s'\n\n", current_line, line);
            error_flag = 1;
            free(token_arr);
            continue;
        }

        /* Parse the tokens in the line */
        if ((parsed_line = parse_line(token_arr, token_count, current_line)) && parsed_line->type == -2) {
            printf("Line %d: '%s'\n\n", current_line, line);
            free(parsed_line);
            free(token_arr);
            error_flag = 1;
            continue;
        }
        add_line_to_program(parsed_program, parsed_line);

        free(token_arr); /* Free the token array, no need for him*/
        free(parsed_line); /* Free the parsed line, because it was added to parsed_program */
    }

    update_data_symbols(get_IC());

    update_data_lines(get_IC(), parsed_program);
    resolve_entrys();
    fclose(am_file);
    return (error_flag == 0);
}

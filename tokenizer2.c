#include "opcodeTable.h"
#include "symbolTable.h"
#include "tokenizer.h"
#include "util.h"
#define MAX_WORD 100


/* Global state, Instruction and Data count*/
extern int IC;
extern int DC;
extern int current_line;

TokenizedLine *tokenize_line(char *line) {
    int error;
    TokenizedLine *result;
    TRY(result = (TokenizedLine *)calloc(1, sizeof(TokenizedLine)));
    result->num_of_tokens = 0;
    result->type = ERROR;
    result->line_number = current_line;
    const char *line_ptr = line; // Use a pointer to traverse the line
    char token[MAX_TOKEN_LENGTH];

    while (my_getword(token, MAX_TOKEN_LENGTH, &line_ptr, &error) != EOF) {
        if (error != TOKEN_OK) {
        switch(error) {
            case ERROR_INVALID_IMMEDIATE:
                printf("Error: Invalid immediate value in token %s\n", token);
                break;
            case ERROR_INVALID_REGISTER:
                printf("Error: Invalid register in token %s\n", token);
                break;
            default:
                printf("Unknown error in token %s\n", token);
            }
            // Handle error (e.g., stop processing, continue to next line, etc.)
            return NULL;
        }
        strncpy(result->tokens[result->num_of_tokens], token, MAX_TOKEN_LENGTH - 1);
        /* Increment token count */
        result->num_of_tokens++;
    }
    if (result->tokens[1][0] == ':') {
        printf("Label definition detected\n");
        result->type = LABEL_DEFINITION;
    }

    return result;
}

void print_tokenized_line(TokenizedLine *tok_line) {
    int i;
    for (i = 0; i < tok_line->num_of_tokens; i++) {
        printf("Token %d: '%s'\n", i + 1, tok_line->tokens[i]);
    }
}


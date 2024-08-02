#include "tokenizer.h"
#include "symbolTable.h"
#include <stdio.h>
#include <string.h>
#define TRY(a)      \
    if (!(a)) {     \
        perror(#a); \
        return 0;   \
    }
#define INITIAL_TOKEN_LINES_SIZE 20
TokenizedLine **tokinzerTable = NULL;
int tokenizerCount = 0;
int tokenizerCapacity = 0;

/* Global state, Instruction and Data count*/
extern int IC;
extern int DC;
extern int current_line;

/*Initialize the tokinzer table */
void initTokenizer() {
    tokinzerTable = malloc(INITIAL_TOKEN_LINES_SIZE * sizeof(TokenizedLine *));
    if (tokinzerTable == NULL) { /* Handle allocation error */
        printf("Failed to allocate memory for tokinzerTable\n");
        exit(1);
    }
    tokenizerCount = 0;
    tokenizerCapacity = INITIAL_TOKEN_LINES_SIZE;
}

int allocate_tokenized_line(TokenizedLine **tok_line) {
    *tok_line = (TokenizedLine *)malloc(sizeof(TokenizedLine));
    if (*tok_line == NULL) {
        return 0;
    }
    return 1;
}

int expand_tokenizer_table() {
    if (tokenizerCount >= tokenizerCapacity) {
        printf("Tokenizer table is full. Size: %d, Capacity: %d\n", tokenizerCount, tokenizerCapacity);
        int new_capacity = tokenizerCapacity * 2;
        TokenizedLine **new_table = realloc(tokinzerTable, new_capacity * sizeof(TokenizedLine *));
        if (new_table == NULL) {
            printf("Failed to reallocate memory for tokinzerTable\n");
            return 0;
        }
        tokinzerTable = new_table;
        tokenizerCapacity = new_capacity;
    }
    return 1;
}

int process_tokens(char *line, TokenizedLine *tok_line) {
    char *token;
    char *temp;
    int len;
    int token_count = 0;

    token = strtok(line, " \t\n");
    while (token != NULL && token_count < MAX_TOKENS) {
        len = strlen(token);
        temp = (char *)malloc((len + 1) * sizeof(char));
        if (temp == NULL) {
            return 0;
        }
        strcpy(temp, token);
        tok_line->tokens[token_count] = temp;
        token_count++;
        token = strtok(NULL, " \t\n");
    }
    tok_line->num_of_tokens = token_count;
    return 1;
}

int add_to_tokenizer_table(TokenizedLine *tok_line) {
    tok_line->line_number = tokenizerCount + 1;
    tokinzerTable[tokenizerCount] = tok_line;
    tokenizerCount++;
    return 1;
}

TokenizedLine *tokenize_line(char *line) {
    TokenizedLine *tok_line;

    TRY(allocate_tokenized_line(&tok_line));
    TRY(expand_tokenizer_table());
    TRY(process_tokens(line, tok_line));
    TRY(add_to_tokenizer_table(tok_line));

    return tok_line;
}


#include "tokenizer.h"
#include "symbolTable.h"
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#define TRY(a)      \
    if (!(a)) {     \
        perror(#a); \
        return NULL;\
    }

#define INITIAL_TOKEN_LINES_SIZE 20


static int is_valid_instruction(const char *token);

TokenizedProgram tokenizedProgram = {NULL, 0, 0};

/* Global state, Instruction and Data count*/
extern int IC;
extern int DC;
extern int current_line;

void initTokenizer() {
    tokenizedProgram.lines = malloc(INITIAL_TOKEN_LINES_SIZE * sizeof(TokenizedLine));
    if (tokenizedProgram.lines == NULL) {
        printf("Failed to allocate memory for tokenizedProgram\n");
        exit(1);
    }
    tokenizedProgram.size = 0;
    tokenizedProgram.capacity = INITIAL_TOKEN_LINES_SIZE;
}

static int expand_tokenized_program() {
    if (tokenizedProgram.size >= tokenizedProgram.capacity) {
        int new_capacity = tokenizedProgram.capacity * 2;
        TokenizedLine *new_lines = realloc(tokenizedProgram.lines, new_capacity * sizeof(TokenizedLine));
        if (new_lines == NULL) {
            printf("Failed to reallocate memory for tokenizedProgram\n");
            return 0;
        }
        tokenizedProgram.lines = new_lines;
        tokenizedProgram.capacity = new_capacity;
    }
    return 1;
}

static TokenType determine_token_type(const char *token) {
    if (token[0] == '.') return TOKEN_DIRECTIVE;
    if (isalpha(token[0])) {
        // Check if it's a valid instruction (you'll need to implement this)
        if (is_valid_instruction(token)) return TOKEN_INSTRUCTION;
        return TOKEN_LABEL;
    }
    if (token[0] == '"') return TOKEN_STRING;
    if (isdigit(token[0]) || token[0] == '-' || token[0] == '+') return TOKEN_NUMBER;
    return TOKEN_OPERAND; // Default case, might need refinement
}

TokenizedLine *tokenize_line(char *line) {
    char *token_start, *token_end;
    int token_count = 0;
    TokenizedLine *tok_line;

    TRY(expand_tokenized_program());
    tok_line = &tokenizedProgram.lines[tokenizedProgram.size];
    tok_line->num_tokens = 0;

    token_start = line;
    while (*token_start != '\0' && token_count < MAX_TOKENS) {
        // Skip leading whitespace
        while (isspace(*token_start)) token_start++;
        if (*token_start == '\0') break;

        // Find end of token
        if (*token_start == '"') {
            // String token
            token_end = strchr(token_start + 1, '"');
            if (token_end == NULL) {
                // Unterminated string
                printf("Error: Unterminated string on line %d\n", current_line);
                return NULL;
            }
            token_end++;
        } else {
            token_end = token_start;
            while (*token_end != '\0' && !isspace(*token_end)) token_end++;
        }

        // Create token
        int token_length = token_end - token_start;
        if (token_length > MAX_TOKEN_LENGTH) {
            printf("Error: Token too long on line %d\n", current_line);
            return NULL;
        }

        Token *token = &tok_line->tokens[token_count];
        strncpy(token->value, token_start, token_length);
        token->value[token_length] = '\0';
        token->type = determine_token_type(token->value);
        token->position = token_start - line;

        token_count++;
        token_start = token_end;
    }

    tok_line->num_tokens = token_count;
    tokenizedProgram.size++;

    return tok_line;
}

// You'll need to implement this function
static int is_valid_instruction(const char *token) {
    // Check if the token is a valid instruction
    // Return 1 if it is, 0 otherwise
    // This will depend on your specific instruction set
    return 0; // Placeholder
}
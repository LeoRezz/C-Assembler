#ifndef TOKENIZER_H
#define TOKENIZER_H
#define MAX_TOKENS 40
#define MAX_TOKEN_LENGTH 70
#define MAX_LABEL_LENGTH 31

typedef enum {
    ERROR,
    LABEL_DEFINITION,
    LABEL_INSTRUCTION,
    LABEL_DATA,
    STRING,
    DATA,
    INSTRUCTION,
    EXTERN,
    ENTRY
} LineType;

typedef struct {
    char tokens[MAX_TOKENS][MAX_TOKEN_LENGTH];
    int num_of_tokens;
    int line_number;
    LineType type;
} TokenizedLine;

void print_tokenized_line(TokenizedLine *tok_line);
TokenizedLine *tokenize_line(char *line);
void initTokenizer();


#endif
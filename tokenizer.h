#ifndef TOKENIZER_H
#define TOKENIZER_H
#define MAX_TOKENS 40
#define MAX_LABEL_LENGTH 31

typedef enum {
    ERROR = -1,
    LABEL_CODE,
    LABEL_DATA,
    INSTRUCTION,
    DIRECTIVE,
    DATA,
} LineType;

typedef struct {
    char *tokens[MAX_TOKENS];
    int num_of_tokens;
    int line_number;
    LineType type;

} TokenizedLine;


typedef struct {
    TokenizedLine *lines;
    int capacity;
    int size;
} TokenizedProgram;

TokenizedLine *tokenize_line(char *line);
void initTokenizer();

#endif
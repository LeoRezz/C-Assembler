#ifndef TOKENIZER_H
#define TOKENIZER_H
#define MAX_TOKENS 40
#define MAX_TOKEN_LENGTH 70
#define MAX_LABEL_LENGTH 31

typedef enum {
    TOKEN_UNKNOWN,
    TOKEN_LABEL,
    TOKEN_INSTRUCTION,
    TOKEN_DIRECTIVE,
    TOKEN_OPERAND,
    TOKEN_STRING,
    TOKEN_NUMBER
} TokenType;

typedef enum {
    INSTRUCTION,
    DIRECTIVE,
    LABEL_CODE,
    LABEL_DATA
} LineType;

typedef struct {
    char value[MAX_TOKEN_LENGTH];
    int position;
    TokenType type;
} Token;

typedef struct {
    Token tokens[MAX_TOKENS];
    int num_tokens;
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
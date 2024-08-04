#ifndef TOKENIZER_H
#define TOKENIZER_H
#define MAX_TOKENS 40
#define MAX_TOKEN_LENGTH 70
#define MAX_LABEL_LENGTH 31

typedef enum {
    TOKEN_UNKNOWN,
    TOKEN_LABEL, /* LABEL:*/
    TOKEN_INSTRUCTION, /* mov , add , cmp */
    TOKEN_DIRECTIVE, /* .data , .string , .extern , .entry */
    TOKEN_OPERAND, /* r0 , *r5 , #-2 , LABEL , ... */
    TOKEN_STRING, /* "hello" */
    TOKEN_NUMBER /* 0 , 1 , 2 , 3 , ... */
} TokenType;

typedef enum {
    ERROR = -1,
    DATA,
    INSTRUCTION,
    LABEL_INSTRUCTION,
    LABEL_DATA,
    EXTERN,
    ENTRY
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
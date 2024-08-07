#ifndef TOKENIZER_H
#define TOKENIZER_H
#define MAX_TOKENS 40
#define MAX_TOKEN_LENGTH 70
#define MAX_LABEL_LENGTH 31

typedef enum {
    TOKEN_UNKNOWN,
    TOKEN_LABEL, /* LABEL:*/
    TOKEN_OPCODE, /* mov , add , cmp */
    TOKEN_DIRECTIVE, /* .data , .string , .extern , .entry */
    TOKEN_OPERAND, /**/
    TOKEN_COMMA,
    TOKEN_REGISTER,
    TOKEN_STRING, /* "hello" */
    TOKEN_NUMBER /* 0 , 1 , 2 , 3 , ... */
} TokenType;

typedef enum {
    ERROR,
    LABEL_DEFINITION,
    LABEL_INSTRUCTION,
    LABEL_DATA,
    DATA,
    INSTRUCTION,
    EXTERN,
    ENTRY
} LineType;

typedef struct {
    char value[MAX_TOKEN_LENGTH];
    TokenType type;
} Token;

typedef struct {
    Token tokens[MAX_TOKENS];
    int num_of_tokens;
    int line_number;
    LineType type;
} TokenizedLine;


TokenizedLine *tokenize_line(char *line);
void initTokenizer();


#endif
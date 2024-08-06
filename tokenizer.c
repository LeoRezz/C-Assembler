#include "tokenizer.h"
#include "symbolTable.h"
#include "opcodeTable.h"
#include "util.h"

#define INITIAL_TOKEN_LINES_SIZE 20

static int is_valid_instruction(const char *token);
static TokenType determine_token_type(const char *token);
static LineType determine_line_type(char *line);
/* Global state, Instruction and Data count*/
extern int IC;
extern int DC;
extern int current_line;


static LineType determine_line_type(char *line) {
    /*  A line can be of five types:
    1.  LABEL: OPCODE OPERAND, OPERAND, LABEL_INSTRUCTION.
    2.  OPCODE OPERAND, OPERAND, INSTRUCTION
    3.  LABEL: .data OR .string, LABEL_DATA
    4.  .data OR .string, DATA
    5.  .extern OR .entry, EXTERN OR ENTRY */

    int label_definition;
    int len;
    char *p;
    label_definition = 0;
    len = strlen(line) + 1; /* + 1 for \0 */

    /* MAIN:	add  r3, LIST 
           ^                   */
    fprintf(stderr, "in determine_line_type, line: '%s'\n", line);
    if (p = strchr(line, ':')) { /* LABEL: */
        fprintf(stderr, "determine_line_type: line contains label\n");
        label_definition = 1;
        p++;
        while (isspace(*p)) {
            p++;
        }
    }

    if (p) {
        len = strlen(p);
        fprintf(stderr, "determine_line_type: checking for entry/extern\n");
        if (strncmp(p, ".entry ", len) == 0) {
            fprintf(stderr, "determine_line_type: line is entry\n");
            return ENTRY;
        }
        if (strncmp(p, ".extern ", len) == 0) {
            fprintf(stderr, "determine_line_type: line is extern\n");
            return EXTERN;
        }
        fprintf(stderr, "determine_line_type: checking for data/string\n");
        if (strncmp(p, ".data ", 5) == 0) {
            fprintf(stderr, "determine_line_type: line is data/string\n");
            return label_definition ? LABEL_DATA : DATA;
        }
        if (strncmp(p, ".string ", 7) == 0) {
            fprintf(stderr, "determine_line_type: line is data/string\n");
            return label_definition ? LABEL_DATA : DATA;
        }
        
    }
    fprintf(stderr, "determine_line_type: line is label_instruction or instruction\n");
    return label_definition ? LABEL_INSTRUCTION : INSTRUCTION;
}

static TokenType determine_token_type(const char *token)
{
    if (token[0] == '.') {
        printf("Token is a directive\n");
        return TOKEN_DIRECTIVE;
    }
    if (token[0] == 'r' && strlen(token) == 2 && token[1] >= '0' && token[1] <= '7') {
        printf("Token is a register\n");
        return TOKEN_OPERAND;
    }
    if (isalnum((int)token[0])) {
        printf("Token is a label or instruction\n");
        if (is_valid_instruction(token)) {
            printf("Token is an instruction\n");
            return TOKEN_INSTRUCTION;
        }
        printf("Token is a label\n");
        return TOKEN_LABEL;
    }
    if (token[0] == '"') {
        printf("Token is a string\n");
        return TOKEN_STRING;
    }
    if (isdigit((int)token[0]) || token[0] == '-' || token[0] == '+') {
        printf("Token is a number\n");
        return TOKEN_NUMBER;
    }
    printf("Token is an operand\n");
    return TOKEN_OPERAND;
}

static int is_valid_instruction(const char *token)
{
    if (find_opcode(token) != NULL) {
        return 1;
    }
    return 0;
}
TokenizedLine *tokenize_line(char *line) {
    Token tok_temp;
    int token_count;
    char *token;
    char *temp;
    int len;
    char *p;
    TokenizedLine *tok_line;
    TRY(tok_line = (TokenizedLine *)malloc(sizeof(TokenizedLine)));
    /* Skip whitespace */
    while (isspace(*line)) {
        line++;
    }

    /* Determine line type */    
    tok_line->type = determine_line_type(line);
    if (tok_line->type == ERROR) {
        printf("Invalid line: %s\n", line);
        free(tok_line);
        return NULL;
    }

    /* Tokenize line */
    token_count = 0;
    token = strtok(line, " \t\n");
    while (token != NULL && token_count < MAX_TOKENS) {
        len = strlen(token) + 1; /* +1 for null terminator */
        strncpy(tok_line->tokens[token_count].value, token, MAX_TOKEN_LENGTH - 1);
        tok_line->tokens[token_count].value[MAX_TOKEN_LENGTH - 1] = '\0'; // Ensure null-termination
        token_count++;
        token = strtok(NULL, " \t\n");
    }
    tok_line->num_of_tokens = token_count;


    printf("Tokenized line: %s\n", line);

    return tok_line;
}

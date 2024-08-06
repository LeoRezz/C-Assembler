#include "tokenizer.h"
#include "symbolTable.h"
#include "opcodeTable.h"
#include "util.h"

#define INITIAL_TOKEN_LINES_SIZE 20

static int is_valid_instruction(const char *token);
static TokenType determine_token_type(const char *token, LineType line_type);
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
        if (strncmp(p, ".entry ", 7) == 0) {
            fprintf(stderr, "determine_line_type: line is entry\n");
            return ENTRY;
        }
        if (strncmp(p, ".extern ", 8) == 0) {
            fprintf(stderr, "determine_line_type: line is extern\n");
            return EXTERN;
        }
        fprintf(stderr, "determine_line_type: checking for data/string\n");
        if (strncmp(p, ".data ", 6) == 0) {
            fprintf(stderr, "determine_line_type: line is data/string\n");
            return label_definition ? LABEL_DATA : DATA;
        }
        if (strncmp(p, ".string ", 8) == 0) {
            fprintf(stderr, "determine_line_type: line is data/string\n");
            return label_definition ? LABEL_DATA : DATA;
        }
        
    }
    fprintf(stderr, "determine_line_type: line is label_instruction or instruction\n");
    return label_definition ? LABEL_INSTRUCTION : INSTRUCTION;
}

static TokenType determine_token_type(const char *token, LineType line_type) {
    printf("\tToken: '%s'\n", token);
    switch (line_type) {
        case LABEL_INSTRUCTION:
            printf("Token is a label or instruction\n");
            if (is_valid_instruction(token)) {
                printf("Token is an instruction\n");
                return TOKEN_INSTRUCTION;
            }
            printf("Token is a label\n");
            return TOKEN_LABEL;
        case LABEL_DATA:
            printf("Token is a label\n");
            return TOKEN_LABEL;
        case INSTRUCTION:
            printf("Token is a label or instruction\n");
            if (is_valid_instruction(token)) {
                printf("Token is an instruction\n");
                return TOKEN_INSTRUCTION;
            }
            printf("Token is a label\n");
            return TOKEN_LABEL;
        case DATA:
            printf("Token is a number, string, or register\n");
            if (token[0] == '"') {
                printf("Token is a string\n");
                return TOKEN_STRING;
            }
            if (isdigit((int)token[0]) || token[0] == '-' || token[0] == '+') {
                printf("Token is a number\n");
                return TOKEN_NUMBER;
            }
            if (token[0] == 'r' && strlen(token) == 2 && token[1] >= '0' && token[1] <= '7') {
                printf("Token is a register\n");
                return TOKEN_OPERAND;
            }
            printf("Token is an operand\n");
            return TOKEN_OPERAND;
        case EXTERN || ENTRY:
            printf("Token is a directive\n");
            return TOKEN_DIRECTIVE;
        default:
            printf("Invalid line type\n");
            return ERROR;
    }
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
    char temp[MAX_TOKEN_LENGTH];
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
    printf("Tokenizing line: %s\n", line);
    char *current = line;
    token_count = 0;
    while (token_count < MAX_TOKENS) {
        char temp[MAX_TOKEN_LENGTH] = {0};
        my_getword(temp, MAX_TOKEN_LENGTH, &current);
        
        printf("Token: %s\n", temp);

        int token_len = strlen(temp);
        if (token_len <= 0) break;  // End of line or error

        strncpy(tok_line->tokens[token_count].value, temp, MAX_TOKEN_LENGTH - 1);
        tok_line->tokens[token_count].type = determine_token_type(temp, tok_line->type);
        tok_line->line_number = current_line;
        token_count++;

        // Skip whitespace
        while (isspace(*(current))) {
            printf("Skipping whitespace: %c\n", *(current));
            current++;
        }
    }
    tok_line->num_of_tokens = token_count;

    printf("Tokenized line: %s\n", line);

    return tok_line;
}

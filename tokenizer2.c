#include "opcodeTable.h"
#include "symbolTable.h"
#include "tokenizer.h"
#include "util.h"
#define MAX_WORD 100


/* Global state, Instruction and Data count*/
extern int IC;
extern int DC;
extern int current_line;

// int my_getword(char *word, int lim, char **line) {
//     int c;
//     char *w = word;

//     /* Skip leading whitespace */
//     while (isspace(**line))
//         (*line)++;

//     if (**line == '\0')
//         return EOF;

//     c = **line;
//     (*line)++;

//     /* If not alphanumeric, return the character */
//     if (!isalnum(c)) {
//         *w++ = c;
//         *w = '\0';
//         return c;
//     }

//     *w++ = c;
//     /* Read alphanumeric characters until limit is reached or non-alphanumeric is
//      * found */
//     for (; --lim > 1; w++) {
//         if (!isalnum(**line))
//             break;
//         *w = **line;
//         (*line)++;
//     }

//     *w = '\0';
//     return *word;
// }

TokenizedLine *tokenize_line(char *line) {
    TokenizedLine *result;
    TRY(result = (TokenizedLine *)malloc(sizeof(TokenizedLine)));
    result->num_of_tokens = 0;
    result->type = ERROR;
    result->line_number = current_line;

    char *line_ptr = line; // Use a pointer to traverse the line
    char token[MAX_TOKEN_LENGTH];

    /* Copy line to buffer (assuming comments are already removed) */
    // /* Tokenize */
    // /* Tokenizing the line by it's type */
    // token = strtok(buffer, " \t\n");
    // while (token != NULL && result.num_of_tokens < MAX_TOKENS) {
    //     /* Copy token to result */
    //     strncpy(result.tokens[result.num_of_tokens], token, MAX_TOKEN_LENGTH - 1);
    //     result.tokens[result.num_of_tokens][MAX_TOKEN_LENGTH - 1] = '\0';
    //     /* Increment token count */
    //     result.num_of_tokens++;
    //     /* Get next token */
    //     token = strtok(NULL, " \t\n");
    // }
    while (my_getword(token, MAX_TOKEN_LENGTH, &line_ptr) != EOF) {
        strncpy(result->tokens[result->num_of_tokens], token, MAX_TOKEN_LENGTH - 1);
        /* Increment token count */
        result->num_of_tokens++;
    }
    print_tokenized_line(result);
    return result;
}

void print_tokenized_line(TokenizedLine *tok_line) {
    int i;
    for (i = 0; i < tok_line->num_of_tokens; i++) {
        printf("Token %d: '%s'\n", i + 1, tok_line->tokens[i]);
    }
}

// int main() {
//     char *p;
//     char line[MAX_LINE_LENGTH];
//     char word[MAX_WORD];
//     int line_number = 1;
//     char *line_ptr;
//     int token_length;
//     TokenizedLine tok_line = {{0}, 0};
//     FILE *fp = fopen("asm1.as", "r");
//     if (fp == NULL) {
//         printf("Error opening file!\n");
//         return 1;
//     }

//     // Test my_getword function
//     while (fgets(line, MAX_LINE, fp) != NULL) {
//         if (p = strrchr(line, '\n'))
//             *p = '\0';

//         tok_line = tokenize_line(line);
//         print_tokenized_line(&tok_line);

//     }

//     fclose(fp);

//     return 0;
// }
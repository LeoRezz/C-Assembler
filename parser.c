#include "parser.h"

/* Global state, Instruction and Data count*/
extern int IC;
extern int DC;
extern int current_line;

void parse_tokenized_line(TokenizedLine *tokenized_line, int *IC) {
    char *p;
    /* Is the first field in the line a symbol? */
    if ((strchr(tokenized_line->tokens[0], ':')) != NULL) {
        tokenized_line->tokens[0][strlen(tokenized_line->tokens[0]) - 1] = '\0'; /*Remove colon */
        tokenized_line->type = LABEL_CODE;
        addSymbol(tokenized_line->tokens[0], IC, SYMBOL_CODE);
    }
    *IC += 1; /*TODO: CHANGE THIS Increment IC */
    
    printf("IC: %d\n", IC);
}

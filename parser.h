#ifndef PARSER_H
#define PARSER_H
/* Gets a TokenizedLine object and goes through each token */
#include "opcodeTable.h"
#include "symbolTable.h"
#include "tokenizer.h"
int parse_tokenized_line(TokenizedLine *tokenized_line, int *IC, int *DC);
#endif
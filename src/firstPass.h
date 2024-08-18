#ifndef FIRST_PASS_H
#define FIRST_PASS_H

#include "tokenizer.h"
#include "symbolTable.h"
#include "opcodeTable.h"
#include "parser.h"
#include "parsed_program.h"
#include "memory_manager.h"

/* Function prototypes */
int first_pass(char *am_filename, ParsedProgram *parsed_program);
void print_token_arr(Token *token_arr, int token_count);
#endif /* FIRST_PASS_H */
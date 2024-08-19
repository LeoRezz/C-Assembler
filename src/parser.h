#ifndef PARSER_H
#define PARSER_H

/* TODO: Add brief description of this header file's purpose */

#include "symbolTable.h"
#include "opcodeTable.h"
#include "util.h"
#include "tokenizer.h"

/* Function prototypes */
Line *parse_line(Token *token_arr, int token_count, int current_line);
void print_parsed_line(Line *parsed_line);
int calculate_instruction_words(int opcode);
int is_valid_label(char *label);

/* TODO: Document each function prototype */

#endif /* PARSER_H */
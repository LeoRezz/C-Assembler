#ifndef PARSER_H
#define PARSER_H

/* TODO: Add brief description of this header file's purpose */

#include "symbolTable.h"
#include "opcodeTable.h"
#include "util.h"
#include "tokenizer.h"

/* Function prototypes */

/* Parses a tokenized array to store values in a parsed line sturct */
Line *parse_line(Token *token_arr, int token_count, int current_line);
int calculate_instruction_words(int opcode);
/* Checks if a label is valid */
int is_valid_label(char *label);

/* TODO: Document each function prototype */

#endif 
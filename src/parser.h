#include "symbolTable.h"
#include "opcodeTable.h"
#include "util.h"
#include "tokenizer.h"

Line *parse_line(Token *token_arr, int token_count, int current_line);
void print_parsed_line(Line *parsed_line);
int calculate_instruction_words(int opcode);

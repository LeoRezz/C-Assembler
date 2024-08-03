#include "parser.h"

/* Global state, Instruction and Data count*/
extern int IC;
extern int DC;
extern int current_line;

/* prototypes */
static void handle_label(TokenizedLine *tokenized_line, int *IC,int *DC);
static void handle_directive(TokenizedLine *tokenized_line, int *DC);

/* Parses a tokenized line and updates IC and DC accordingly */
/* There can be either a label or a directive or an instruction at the beginning
 * of the line*/
void parse_tokenized_line(TokenizedLine *tokenized_line, int *IC,int *DC) {
  tokenized_line->type = INSTRUCTION;
  handle_label(tokenized_line, IC, DC);
  handle_directive(tokenized_line, DC);
  handle_instruction(tokenized_line, IC);
}

void handle_instruction(TokenizedLine *tokenized_line, int *IC) {
  // Log the beginning of handling the instruction
  printf("handle_instruction: Begin handling instruction\n");

  // Determine the index of the instruction (opcode)
  int opcode_index = 0;
  if (tokenized_line->type == LABEL_CODE) {
    opcode_index = 1;
  }

  // Log the tokens in the tokenized line
  printf("handle_instruction: Opcode: %s\n",
         tokenized_line->tokens[opcode_index]);

  // Find the opcode in the opcode table
  const Opcode *op = find_opcode(tokenized_line->tokens[opcode_index]);
  if (op == NULL) {
    printf("Error: Unknown opcode '%s' at line %d\n",
           tokenized_line->tokens[opcode_index], current_line);
    return;
  }

  // Validate the number of operands
  int num_operands = tokenized_line->num_of_tokens - opcode_index - 1;
  if (num_operands != op->operands) {
    fprintf(stderr, "Error: Incorrect number of operands for '%s' at line %d\n",
            op->mnemonic, current_line);
    return;
  }

  // Process operands and update IC
  *IC += 1; // For the opcode word
  for (int i = 1; i <= op->operands; i++) {
    // Here you would typically encode each operand
    // For now, we'll just increment IC for each operand
    *IC += 1;
  }

  printf("handle_instruction: Processed instruction '%s' with %d operands\n",
         op->mnemonic, op->operands);
  printf("handle_instruction: Updated IC to %d\n", *IC);
}

/* Handles labels */
static void handle_label(TokenizedLine *tokenized_line, int *IC, int *DC) {
  char *p;
  int length = strlen(tokenized_line->tokens[0]);
  p = tokenized_line->tokens[0] + length - 1;

  if (tokenized_line->tokens[0][0] == '.' ||
      tokenized_line->tokens[1][0] == '.') {
    // Log that the label is a directive label
    tokenized_line->type = DIRECTIVE;
  }

  if (*p == ':') {
    // Log that a colon was found at the end of the label
    printf("handle_label: Colon found at the end of the label\n");

    // Remove the colon from the label
    tokenized_line->tokens[0][length - 1] = '\0';

    // Log the updated label
    printf("handle_label: Updated label: %s\n", tokenized_line->tokens[0]);

    if (tokenized_line->tokens[1][0] == '.') {
      // Log that the label is a data label
      printf("handle_label: Label is a data label\n");
      tokenized_line->type = LABEL_DATA;
    } else {
      // Log that the label is a code label
      printf("handle_label: Label is a code label\n");
      tokenized_line->type = LABEL_CODE;
    }
    if (tokenized_line->type == LABEL_CODE) {
      addSymbol(tokenized_line->tokens[0], IC, SYMBOL_CODE); /* code */
    } else if (tokenized_line->type == LABEL_DATA) {
      handle_directive(tokenized_line, DC); /* data */
    }
  }

  // Log the end of handling the label
  printf("handle_label: End handling label\n");
}
static void handle_directive(TokenizedLine *tokenized_line, int *DC) {
    // Determine the index of the instruction (opcode)
  int opcode_index = 0;
  if (tokenized_line->type == LABEL_CODE || tokenized_line->type == LABEL_DATA) {
    opcode_index = 1;
  }
  if (strcmp(tokenized_line->tokens[opcode_index], ".data") == 0) {
    printf("handle_directive: Found .data directive\n");
    tokenized_line->type = DIRECTIVE;
    int num_items = tokenized_line->num_of_tokens - 1;
    printf("handle_directive: Number of data items = %d\n", num_items);
    *DC += num_items; // Count all data items
  } else if (strcmp(tokenized_line->tokens[opcode_index], ".string") == 0) {
    printf("handle_directive: Found .string directive\n");
    char *string = tokenized_line->tokens[1];
    printf("handle_directive: String = %s\n", string);
    int string_length = strlen(string) - 2 +
                        1; // String length + null terminator, excluding quotes
    printf("handle_directive: String length = %d\n", string_length);
    DC += string_length;
  }
}

int calculate_instruction_words(TokenizedLine *tokenized_line) {
  char *opcode;
  int word_count;
  word_count = 1; // Every instruction has at least one word

  // Get the opcode
  if (tokenized_line->type == INSTRUCTION) {
    opcode = tokenized_line->tokens[0];
  } else if (tokenized_line->type == LABEL_CODE) {
    opcode = tokenized_line->tokens[1];
  }

  Opcode *instruction = find_opcode(opcode);

  if (instruction == NULL) {
    printf("calculate_instruction_words: Error: Unknown opcode: %s\n", opcode);
    exit(1);
  }
  // Add the operands' words
  word_count += instruction->operands;

  return word_count;
}

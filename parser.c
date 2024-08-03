#include "parser.h"

extern int IC, DC, current_line;
static void handle_label(TokenizedLine *tokenized_line, int *IC, int *DC);
static void handle_directive(TokenizedLine *tokenized_line, int *DC);
static void handle_instruction(TokenizedLine *tokenized_line, int *IC);

void parse_tokenized_line(TokenizedLine *tokenized_line, int *IC, int *DC) {
  tokenized_line->type = INSTRUCTION;
  handle_label(tokenized_line, IC, DC);
  handle_directive(tokenized_line, DC);
  handle_instruction(tokenized_line, IC);
}

static void handle_instruction(TokenizedLine *tokenized_line, int *IC) {
  Token *token = &tokenized_line->tokens[0];
  const Opcode *op = find_opcode(token->value);

  if (!op) {
    printf("Error: Unknown opcode '%s' at line %d\n", token->value,
           current_line);
    return;
  }

  *IC += 1 + op->operands;
  printf("Processed instruction '%s' with %d operands. Updated IC to %d\n",
         op->mnemonic, op->operands, *IC);
}

static void handle_label(TokenizedLine *tokenized_line, int *IC, int *DC) {
  Token *label_token = &tokenized_line->tokens[0];
  char *label = label_token->value;
  int length = strlen(label);

  if (label[0] == '.' || tokenized_line->tokens[1].value[0] == '.') {
    tokenized_line->type = DIRECTIVE;
  }

  if (label[length - 1] == ':') {
    label[length - 1] = '\0';
    tokenized_line->type =
        (tokenized_line->tokens[1].value[0] == '.') ? LABEL_DATA : LABEL_CODE;

    if (tokenized_line->type == LABEL_CODE) {
      if (!addSymbol(label, IC, SYMBOL_CODE)) {
        printf("Error: Failed to add code symbol '%s' at line %d\n", label,
               current_line);
      }
    } else if (tokenized_line->type == LABEL_DATA) {
      if (!addSymbol(label, DC, SYMBOL_DATA)) {
        printf("Error: Failed to add data symbol '%s' at line %d\n", label,
               current_line);
      }
      handle_directive(tokenized_line, DC);
    }
  }
}

static void handle_directive(TokenizedLine *tokenized_line, int *DC) {
  int opcode_index =
      (tokenized_line->type == LABEL_CODE || tokenized_line->type == LABEL_DATA)
          ? 1
          : 0;
  const char *directive =
      tokenized_line->tokens[opcode_index]
          .value; // Assuming Token has a 'value' field of type char*
  if (strcmp(directive, ".data") == 0) {
    tokenized_line->type = DIRECTIVE;
    *DC += tokenized_line->num_tokens -
           1; // Assuming TokenizedLine has a 'num_tokens' field
  } else if (strcmp(directive, ".string") == 0) {
    *DC += strlen(tokenized_line->tokens[opcode_index + 1].value) -
           1; // Assuming Token has a 'value' field of type char*
  }
}

int calculate_instruction_words(TokenizedLine *tokenized_line) {
  const char *opcode = (tokenized_line->type == INSTRUCTION)
                           ? tokenized_line->tokens[0].value
                           : tokenized_line->tokens[1].value;
  const Opcode *instruction = find_opcode(opcode);

  if (!instruction) {
    printf("Error: Unknown opcode: %s\n", opcode);
    exit(1);
  }

  return 1 + instruction->operands;
}
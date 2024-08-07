#include "tokenizer.h"
#include "opcodeTable.h"
#include "symbolTable.h"
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
  fprintf(stderr,
          "determine_line_type: line is label_instruction or instruction\n");
  return label_definition ? LABEL_INSTRUCTION : INSTRUCTION;
}

int is_register(const char *str) {
  return (str[0] == 'r' && str[1] >= '0' && str[1] <= '7');
}

int is_number(const char *str) {
  int i = 0;
  if (str[i] == '#') {
    i++;
  }

  if (str[i] == '+' || str[i] == '-') {
    i++;
  }
  for (; str[i] != '\0'; i++) {
    if (!isdigit(str[i])) {
      return 0;
    }
  }
  return i > 0;
}

TokenType get_token_type(const char *token) {
  if (token[strlen(token) - 1] == ':') {
    return TOKEN_LABEL;
  }
  if (token[0] == '.') {
    return TOKEN_DIRECTIVE;
  }
  if (is_register(token)) {
    return TOKEN_REGISTER;
  }
  if (is_number(token)) {
    return TOKEN_NUMBER;
  }
  if (token[0] == '"' && token[strlen(token) - 1] == '"') {
    return TOKEN_STRING;
  }
  if (strcmp(token, ",") == 0) {
    return TOKEN_COMMA;
  }
  /* Check if it's a valid instruction (you'd need to implement this) */
  if (is_valid_instruction(token)) {
    return TOKEN_OPCODE;
  }
  return TOKEN_UNKNOWN;
}
static int is_valid_instruction(const char *token) {
  if (find_opcode(token) != NULL) {
    return 1;
  }
  return 0;
}

TokenizedLine *tokenize_line(char *line) {
  TokenizedLine *result;
  TRY(result = (TokenizedLine *)malloc(sizeof(TokenizedLine)));
  char buffer[MAX_LINE];
  char *token;
  int in_string = 0;
  int i, j;
  int type, token_count;
  /* Add line to buffer, without the newline */
  for (i = 0; line[i] != '\0' && line[i] != '\n'; i++) {
    buffer[i] = line[i];
  }
  buffer[i] = '\0';
  /* Initialize line */
  result->num_of_tokens = 0;
  result->line_number = current_line;
  result->type = ERROR;

  /* Determine line type by analyzing the first and second token */
  type = result->type = determine_line_type(line);
  if (type == ERROR) {
    printf("Invalid line: %s\n", line);
    free(result);
    return NULL;
  }

  /* Tokenize line */
  /* Tokenizing the line by it's type */
  token_count = 0;
  switch (type) {
  case LABEL_INSTRUCTION: /* LABEL: OPCODE OPERAND, OPERAND  max tokens = 4 +
                             comma*/
    i = 0;
    while (i < strlen(buffer) && result->num_of_tokens < MAX_TOKENS) {
      /* Skip whitespace */
      while (isspace(buffer[i]))
        i++;
      if (buffer[i] == '\0')
        break;

      /* Handle other tokens */
      j = i;
      while (!isspace(buffer[j]) && buffer[j] != ',' && buffer[j] != '\0')
        j++;
      if (buffer[j] == ',')
        j++;

      /* Store token */
      strncpy(result->tokens[result->num_of_tokens].value, buffer + i, j - i);
      result->tokens[result->num_of_tokens].value[j - i] = '\0';
      result->tokens[result->num_of_tokens].type =
          get_token_type(result->tokens[result->num_of_tokens].value);
      result->num_of_tokens++;

      i = j;
    }
    break;
  case LABEL_DATA: /* LABEL: .data/.string  max tokens = */
    break;
  case INSTRUCTION:
    /* code */
    break;
  case DATA:
    /* Skip whitespace */
    while (isspace(buffer[i]))
      i++;
    if (buffer[i] == '\0')
      break;

    /* Handle strings */
    if (buffer[i] == '"') {
      in_string = 1;
      j = i + 1;
      while (buffer[j] != '"' && buffer[j] != '\0')
        j++;
      if (buffer[j] == '"') {
        j++;
        in_string = 0;
      }
      break;
    case EXTERN:
      break;
    case ENTRY:
      break;

    default:
      break;
    }

    result->num_of_tokens = token_count;

    printf("Tokenized line: %s\n", line);

    return result;
  }
}
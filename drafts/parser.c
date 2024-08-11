#include "parser.h"

extern int IC, DC, current_line;
static void handle_label(TokenizedLine *tokenized_line, int *IC, int *DC);
static void handle_directive(TokenizedLine *tokenized_line, int *DC);
static void handle_instruction(TokenizedLine *tokenized_line, int *IC);
int calculate_instruction_words(TokenizedLine *tokenized_line);
int parse_tokenized_line(TokenizedLine *tok_line, int *IC, int *DC) {
    int i, j, instruction_length, data_length, label_definition = 0;
    const Opcode *opcode;
    i = 0;
    instruction_length = 0;

    /* Is there a label definition ? */
    if (tok_line->tokens[1][0] == ':') {
        printf("Label definition detected\n");
        tok_line->type = LABEL_DEFINITION;
        label_definition = 1;
        i = 2; /* advance to the instruction/directive */
    }
    /* Is this an instruction line? */
    if ((opcode = find_opcode(tok_line->tokens[i])) != NULL) {
        printf("Instruction '%s' has %d operands\n", tok_line->tokens[i], opcode->operands);
        if (label_definition) {
            tok_line->type = LABEL_INSTRUCTION;
        } else {
            tok_line->type = INSTRUCTION;
        }
        /* Calculate the number of words for this instruction */
        instruction_length = opcode->operands;
        if (instruction_length == 2) { /* if 2 operands, check for two registers */
            if (tok_line->tokens[i + 1][0] == 'r' && tok_line->tokens[i + 3][0] == 'r') {
                printf("Operands %s and %s are both registers\n", tok_line->tokens[i + 1], tok_line->tokens[i + 3]);
                instruction_length--;
            }
        }
        /* Add instruction length to IC */
        if (label_definition) {
            addSymbol(tok_line->tokens[0], IC, SYMBOL_CODE);
            printf("Added symbol '%s' to IC\n", tok_line->tokens[0]);
        }
        *IC += instruction_length + 1; /* add one word for instruction */
        printf("Added %d words to IC\n", instruction_length + 1);
        i++;
    }

    if (strcmp(tok_line->tokens[i], ".string") == 0) {
        i++; /* skip over the.string directive */
        printf(".string Directive detected\n");
        data_length = strlen(tok_line->tokens[i]) - 1;
        if (label_definition) {
            tok_line->type = LABEL_DATA;
            addSymbol(tok_line->tokens[0], DC, SYMBOL_DATA);
            printf("Added symbol '%s' to DC = %d\n", tok_line->tokens[0], *DC);
        } else
            tok_line->type = DATA;

        *DC = *DC + data_length;
        printf("Added %d words to DC\n", data_length);
    }

    if (strcmp(tok_line->tokens[i], ".data") == 0) {
        /* i++;  skip over the .data directive */
        printf(".data Directive detected\n");
        if (tok_line->num_of_tokens == 2) {
            data_length = 1;
        } else {
            data_length = 1 + ((tok_line->num_of_tokens) / 2) - i; /* TODO: need to exclude commas*/
        }

        if (label_definition) {
            tok_line->type = LABEL_DATA;
            addSymbol(tok_line->tokens[0], DC, SYMBOL_DATA);
            printf("Added symbol '%s' to DC = %d\n", tok_line->tokens[0], *DC);
        } else
            tok_line->type = DATA;

        *DC = *DC + data_length;
        printf("Added %d words to DC\n", data_length);
    }

    if (strcmp(tok_line->tokens[i], ".entry") == 0) {

        printf(".entry Directive detected\n");
    }
    if (strcmp(tok_line->tokens[i], ".extern") == 0) {
        
        printf(".extern Directive detected\n");
    } else {
        printf("Unhandled directive or instruction '%s' at line %d\n", tok_line->tokens[i], current_line);
        return 0;
    }
}

static void handle_instruction(TokenizedLine *tokenized_line, int *IC) {
    const Opcode *op = find_opcode(tokenized_line->tokens[0]);

    if (!op) {
        printf("Error: Unknown opcode '%s' at line %d\n", tokenized_line->tokens[0],
               current_line);
        return;
    }

    int words = calculate_instruction_words(tokenized_line);
    *IC += words;
    printf("Processed instruction '%s' with %d operands. Updated IC to %d\n",
           op->mnemonic, op->operands, *IC);
}

static void handle_label(TokenizedLine *tokenized_line, int *IC, int *DC) {
}

static void handle_directive(TokenizedLine *tokenized_line, int *DC) {
}

int calculate_instruction_words(TokenizedLine *tokenized_line) {
    // Determine the opcode of the instruction

    // Find the Opcode struct corresponding to the opcode

    // If no Opcode struct is found, print an error message and exit

    // Calculate and return the number of words required for the instruction
    return 1;
}

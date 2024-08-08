#include "parser.h"

extern int IC, DC, current_line;
static void handle_label(TokenizedLine *tokenized_line, int *IC, int *DC);
static void handle_directive(TokenizedLine *tokenized_line, int *DC);
static void handle_instruction(TokenizedLine *tokenized_line, int *IC);
int calculate_instruction_words(TokenizedLine *tokenized_line);
void parse_tokenized_line(TokenizedLine *tok_line, int *IC, int *DC) {
    int i, instruction_length, label_definition = 0;
    const Opcode *opcode;
    i = 0;

    printf("Parsing line %d\n", tok_line->line_number);

    /* Is there a label definition ? */
    if (tok_line->tokens[1][0] == ':') {
        printf("Label definition detected\n");
        tok_line->type = LABEL_DEFINITION;
        label_definition = 1;
        i = 2;
    }

    /* Is this an instruction line? */
    if ((opcode = find_opcode(tok_line->tokens[i])) != NULL) {
        printf("Instruction detected\n");
        if (label_definition) {
            tok_line->type = LABEL_INSTRUCTION;
        } else {
            tok_line->type = INSTRUCTION;
        }
        /* Calculate the number of words for this instruction */
        instruction_length = opcode->operands;
        printf("Instruction '%s' has %d operands\n", tok_line->tokens[i], instruction_length);
        /* Add instruction length to IC */
        if (label_definition) {
            addSymbol(tok_line->tokens[0], IC, SYMBOL_CODE);
            /* code */
        }

        *IC += instruction_length + 1;
        i++;
    }

    if (tok_line->tokens[i][0] == '.') {
        printf("Directive detected\n");
        handle_directive(tok_line, DC);
        if (label_definition) {
            addSymbol(tok_line->tokens[0], DC, SYMBOL_DATA);
            *DC = *DC + tok_line->num_of_tokens - 4;
        }        
    }
    if (tok_line->type == LABEL_DATA) {
        printf("Label data detected\n");
        handle_label(tok_line, IC, DC);
        handle_directive(tok_line, DC);
    }
    if (tok_line->type == INSTRUCTION) {
        printf("Instruction detected\n");
        handle_instruction(tok_line, IC);
    }
    if (tok_line->type == DATA) {
        printf("Data detected\n");
        handle_directive(tok_line, DC);
    }
    if (tok_line->type == EXTERN || tok_line->type == ENTRY) {
        printf("External or entry detected\n");
        handle_directive(tok_line, DC);
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

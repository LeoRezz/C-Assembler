#include "parser.h"

extern int IC, DC, current_line;
int is_data_line(TokenizedLine *tok_line, int *i);
int is_instruction_line(TokenizedLine *tok_line);

Instruction create_instruction(TokenizedLine *tok_line, int *IC) {
    Instruction inst;
    int instruction_size;
    inst.address = *IC;
    /* Parse and set other instruction fields */
    *IC += instruction_size; /* Increment IC by the size of this instruction */
    return inst;
}

Data create_data(TokenizedLine *tok_line, int *DC, int *i) {
    int start_index = *i + 1;
    Data data;
    long value;
    char *endptr;
    int data_size;
    data.value_count = 0;
    data.address = *DC;
    data.type = tok_line->type == DATA ? DATA_INT : DATA_STRING;

    switch (data.type) {

    case DATA_INT:
        printf("Creating integer data:");
        for (start_index; start_index < tok_line->num_of_tokens; start_index++) {
            if (tok_line->tokens[start_index][0] != ',') { /* Skip commas */
                printf(" %s", tok_line->tokens[start_index]);
                value = strtol(tok_line->tokens[start_index], &endptr, 10);
                data.content.int_values[data.value_count++] = (int)value;
            }
        }
        printf("\n");
        printf("Data parsed. Size: %d\n", data.value_count);
        *DC += data.value_count;

        break;

    case DATA_STRING:
        printf("Create data: %s\n", tok_line->tokens[start_index]);
        strcpy(data.content.char_values, tok_line->tokens[start_index] + 1);
        data.content.char_values[strlen(data.content.char_values) - 1] = '\0'; /* Remove '"' */
        data.value_count = strlen(data.content.char_values) + 1;
        printf("Parsed data: %s\n", data.content.char_values);
        *DC += data.value_count; /* Increment DC by the size of this data item */

        break;

    default:
        break;
    }

    return data;
}

Line *parse_line(TokenizedLine *tok_line, int *IC, int *DC) {
    int i, label_declaration_flag;
    Line *result;
    i = 0;
    label_declaration_flag = tok_line->type == LABEL_DEFINITION ? 1 : 0;

    TRY(result = (Line *)calloc(1, sizeof(Line))); /* Allocate memory for the line struct */

    if (is_data_line(tok_line, &i)) {
        result->type = LINE_DATA;
        result->content.data = create_data(tok_line, DC, &i);
    } else if (is_instruction_line(tok_line)) {
        result->type = LINE_INSTRUCTION;
        result->content.inst = create_instruction(tok_line, IC);
    } else {
        /* Handle other line types or errors */
    }

    return result;
}

int is_data_line(TokenizedLine *tok_line, int *i) {
    /* Logging statements for debugging */

    if (tok_line->type == LABEL_DEFINITION) {
        *i = 2; /* advance to the instruction/directive */
    }

    if (strcmp(tok_line->tokens[*i], ".string") == 0) {
        printf("is_data_line: .string directive detected\n");
        tok_line->type = STRING;
        return 1;
    }

    if (strcmp(tok_line->tokens[*i], ".data") == 0) {
        printf("is_data_line: .data directive detected\n");
        tok_line->type = DATA;

        return 1;
    }

    printf("is_data_line: Neither .string nor .data directive detected\n");
    return 0;
}

int is_instruction_line(TokenizedLine *tok_line) {

    return 0;
}
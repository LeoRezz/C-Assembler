#include "parser.h"

Line *parse_line(Token *token_arr, int token_count) {
    int i;
    i = 0;
    Line *parsed_line = calloc(1, sizeof(Line));
    if (!parsed_line) {
        // Handle memory allocation error
        return NULL;
    }

    

    /* Check for label */
    if (token_arr[0].type == LABEL_DEF) {
        strcpy(token_arr[0].value, parsed_line->label);
        i++;
    }

    /* Check for instruction or directive */
    if (i < token_count) {
        if (token_arr[i].type >= MOV && token_arr[i].type <= STOP) {
            parsed_line->type = LINE_INSTRUCTION;
            parsed_line->content.inst.opcode = ((token_arr[i].type) - MOV);
            i++;
        }
        
    }

    return parsed_line;
}
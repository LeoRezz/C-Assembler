#include "firstPass.h"

#define INITIAL_ADDRESS 100
#define MAX_LINE 82
#define MAX_LABEL_LENGTH 31

/* Global state, Instruction and Data count*/
int IC = INITIAL_ADDRESS;
int DC = 0;
int current_line = 0;

/* prototypes: should be in header file*/
void first_pass(FILE *input_file);

int main(int argc, char *argv[]) {
    FILE *input_file;
    char *file_name;
    input_file = fopen("asm1.am", "r"); /* TODO: change later to argv[1] */
    /* fopen() return NULL if unable to open file in given mode. */
    if (!input_file) {
        printf("Failed to open input file: %s\n", argv[1]);
        free(file_name);
        exit(EXIT_FAILURE);
    }
    initSymbolTable(); /* Initialize symbol table */
    first_pass(input_file);
    updateDataSymbols(IC);
    printSymbolTable();
    freeSymbolTable();
}

/* Pass 1 Algorithm:
 * 1. Initialize: DC = 0, IC = 0
 * 2. Read the next line from the source file. If end of file, go to step 16.
 * 3. Is the first field in the line a symbol? If not, go to step 5.
 * 4. Turn on "symbol definition" flag.
 * 5. Is this a data storage instruction (.data or .string)? If not, go to
 * step 8.
 * 6. If there's a symbol definition, add it to the symbol table with the
 *    attribute "data" and value DC. (If symbol already exists in table, report
 * an error).
 * 7. Identify data type, encode it, and update DC accordingly. Go back to
 * step 2.
 * 8. Is this an .extern or .entry directive? If not, go to step 11.
 * 9. If it's .extern, add each symbol (operand) to the symbol table
 *    without a value and with the attribute "external". Go back to step 2.
 * 10. If there's a symbol definition, add it to the symbol table with the
 *     attribute "code" and value IC+100. (If symbol already exists in table,
 * report an error).
 * 11. Search for the operation name in the operations table. If not
 *     found, report an error in the instruction name.
 * 12. Analyse the instruction's operands and calculate L (instruction length).
 *     Build the binary code for the first word of the instruction.
 * 13. Update IC = IC + L, and go back to step 2.
 * 14. Source file has been fully read. If errors were found in Pass 1, stop
 * here.
 * 15. Update values in the symbol table for all symbols marked as "data"
 *     by adding IC+100.
 */

void first_pass(FILE *file) {
    char *p;
    /* Implement Pass 1 algorithm here */
    char line[MAX_LINE];
    TokenizedLine *tok_line;
    /* Read each line from the source file */
    while (fgets(line, MAX_LINE, file)) {
        if (p = strrchr(line, '\n'))
            *p = '\0';

        current_line++;                 /* Increment line number for error reporting */
        tok_line = tokenize_line(line); /* Tokenize the line */
        if (tok_line == NULL) {
            printf("Error tokenizing line %d\n", current_line);
            free(tok_line);
            return;
        }
        parse_tokenized_line(tok_line, &IC, &DC); /* Parse the line */
        printf("IC in firstPass: %d\n", IC);
        printf("DC in firstPass: %d\n", DC);
        free(tok_line);
    }
}

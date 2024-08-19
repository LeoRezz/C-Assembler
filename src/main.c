#include "firstPass.h"
#include "preprocessor.h"

/* global error flag */
int error_flag = 0;

void reset_assembler_state();
void reset_assembler_state(ParsedProgram *parsed_program);
int secondPass(char *filename, ParsedProgram *program, int numOfLines);

int main(int argc, char *argv[]) {
    int i;
    ParsedProgram *parsed_program;
    
    /* Variables to store filenames and flags */
    char as_filename[MAX_FILENAME];
    char am_filename[MAX_FILENAME];
    
    /* Check if at least one input file is provided */
    if (argc < 2) {
        printf("Usage: ./assembler <input_file1> <input_file2> ...\n");
        return 1;
    }

    /* Process each input file */
    for (i = 1; i < argc; i++) { /* ps */

        /* Construct filenames */

        /* Input filename */
        if (strlen(argv[i]) > MAX_FILENAME) {
            printf("Error: filename too long\n");
            continue; /* Move to next file */
        }     

        append_extension(as_filename, argv[i], ".as");
            

        append_extension(am_filename, argv[i], ".am");

        /* Preprocess (macro expansion) */
        if (!preprocess(as_filename, am_filename)) {
            printf("\nError in preprocessing %s\n", as_filename);
            continue; /* Move to next file */
        }

        printf("Assembling %s\n", am_filename);
        
        /* Initialize assembler state */

        init_memory_counters();
        parsed_program = init_parsed_program();

        /* First pass */
        if (!first_pass(am_filename, parsed_program)) {
            printf("\nError in first pass for %s\n", am_filename);
            reset_assembler_state(parsed_program);
            continue; /* Move to next file */
        }

        printf("First pass complete\n");

        /* Second pass */
        if (!secondPass(argv[i], parsed_program, parsed_program->count)) { 
            printf("\nError in second pass for %s\n", am_filename);
            reset_assembler_state(parsed_program);
            continue; /* Move to next file */
        }

        printf("Second pass complete\n");
        printf("Assembly of %s successful\n", argv[i]);

        /* Reset for next file */
        reset_assembler_state(parsed_program);
    }

    return 0;
}

void reset_assembler_state(ParsedProgram *parsed_program) {
    free_symbol_table();
    free_parsed_program(parsed_program);
}

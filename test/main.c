#include "firstPass.h"
#include "preprocessor.h"
#include "second_pass.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void reset_assembler_state(ParsedProgram *parsed_program);

int main(int argc, char *argv[]) {
    int i;
    ParsedProgram *parsed_program;
    
    /* Variables to store filenames and flags */
    char input_filename[MAX_FILENAME];
    char am_filename[MAX_FILENAME];
    char ob_filename[MAX_FILENAME];
    char ent_filename[MAX_FILENAME];
    char ext_filename[MAX_FILENAME];
    /* flags for file existence */
    int has_entries;
    int has_externals;

    has_entries = 0;
    has_externals = 0;

    /* Check if at least one input file is provided */
    if (argc < 2) {
        printf("Usage: ./assembler <input_file1> <input_file2> ...\n");
        return 1;
    }

    /* Process each input file */
    for (i = 1; i < argc; i++) {
        /* Construct filenames */
        append_extension(input_filename, argv[i], ".as");
        append_extension(am_filename, argv[i], ".am");
        append_extension(ob_filename, argv[i], ".ob");
        append_extension(ent_filename, argv[i], ".ent");
        append_extension(ext_filename, argv[i], ".ext");

        /* Preprocess (macro expansion) */
        if (!preprocess(input_filename, am_filename)) {
            printf("Error in preprocessing %s\n", input_filename);
            continue; /* Move to next file */
        }

        init_memory_counters();
        parsed_program = init_parsed_program();
        init_symbol_table(); /* Initialize symbol table */

        /* First pass */
        if (!first_pass(am_filename, parsed_program)) {
            printf("Error in first pass for %s\n", am_filename);
            reset_assembler_state(parsed_program);
            continue; /* Move to next file */
        }

        /* Second pass */
        second_pass(parsed_program, ob_filename, ent_filename, ext_filename);

        /* Check for entries and externals */
        has_entries = check_entries(parsed_program);
        has_externals = check_externals(parsed_program);

        /* If no entries or externals, remove the corresponding files */
        if (!has_entries) {
            remove(ent_filename);
        }
        if (!has_externals) {
            remove(ext_filename);
        }

        /* Reset for next file */
        reset_assembler_state(parsed_program);
    }

    return 0;
}

void reset_assembler_state(ParsedProgram *parsed_program) {
    free_symbol_table();
    free_parsed_program(parsed_program);
}

/* Helper functions to check for entries and externals */
int check_entries(ParsedProgram *program) {
    int i;
    for (i = 0; i < program->count; i++) {
        if (program->lines[i].type == LINE_ENTRY) {
            return 1;
        }
    }
    return 0;
}

int check_externals(ParsedProgram *program) {
    int i;
    for (i = 0; i < program->count; i++) {
        if (program->lines[i].type == LINE_EXTERN) {
            return 1;
        }
    }
    return 0;
}
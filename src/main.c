#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "preprocessor.h"
#include "firstPass.h"
#include "secondPass.h"


#define MAX_FILENAME 100

int main(int argc, char *argv[])
{
    int i;
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
        printf("Error: No input files provided.\n");
        return 1;
    }

    /* Process each input file */
    for (i = 1; i < argc; i++) {
        /* Construct filenames */


        /* Replace .as extension with appropriate extensions */
        /*TODO: Trim the .as */
        trim_extention(argv[i]); /* To Implement */
        strncpy(am_filename,argv[i],MAX_FILENAME);

        /* Preprocess (macro expansion) */
        if (!preprocess(input_filename, am_filename)) {
            printf("Error in preprocessing %s\n", input_filename);
            continue;  /* Move to next file */
        }

        /* First pass */
        if (!first_pass(am_filename)) {
            printf("Error in first pass for %s\n", am_filename);
            continue;  /* Move to next file */
        }

        /* Second pass */
        if (!second_pass()) {
            printf("Error in second pass for %s\n", am_filename);
            continue;  /* Move to next file */
        }

        /* Generate output files */
        generate_object_file(ob_filename);

        has_entries = check_entries();
        has_externals = check_externals();

        if (has_entries) {
            generate_entries_file(ent_filename);
        }

        if (has_externals) {
            generate_externals_file(ext_filename);
        }

        /* Reset for next file */
        reset_assembler_state();
    }

<<<<<<< HEAD
    secondPass(parsed_program, parsed_program->count);

    free_symbol_table();
    free_parsed_program(parsed_program);
    fclose(input_file);
=======
>>>>>>> 7a881c4384ce478bfa9e3ba94c1ac3ef0a32f7ac
    return 0;
}
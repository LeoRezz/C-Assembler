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
    char input_filename[MAX_FILENAME];
    char am_filename[MAX_FILENAME];
    char ob_filename[MAX_FILENAME];
    char ent_filename[MAX_FILENAME];
    char ext_filename[MAX_FILENAME];
    int has_entries = 0;
    int has_externals = 0;

    /* Check if at least one input file is provided */
    if (argc < 2) {
        fprintf(stderr, "Error: No input files provided.\n");
        return 1;
    }

    /* Process each input file */
    for (i = 1; i < argc; i++) {
        /* Construct filenames */
        /* add copy my_strdup*/

        /* Replace .as extension with appropriate extensions */

        /* Preprocess (macro expansion) */
        if (!preprocess(input_filename, am_filename)) {
            fprintf(stderr, "Error in preprocessing %s\n", input_filename);
            continue;  /* Move to next file */
        }

        /* First pass */
        if (!first_pass(am_filename)) {
            fprintf(stderr, "Error in first pass for %s\n", am_filename);
            continue;  /* Move to next file */
        }

        /* Second pass */
        if (!second_pass()) {
            fprintf(stderr, "Error in second pass for %s\n", am_filename);
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

    return 0;
}
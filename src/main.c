#include "firstPass.h"
#include "preprocessor.h"
// #include "secondPass.h"

int error_count = 0;
void reset_assembler_state();
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

        /* Input filename */
        append_extension(input_filename, argv[i], ".as");
        append_extension(am_filename, argv[i], ".am");

        /* Preprocess (macro expansion) */
        if (!preprocess(input_filename, am_filename)) {
            printf("Error in preprocessing %s\n", input_filename);
            continue; /* Move to next file */
        }

        init_memory_counters();
        parsed_program = init_parsed_program();

        /* First pass */
        if (!first_pass(am_filename, parsed_program)) {
            printf("Error in first pass for %s\n", am_filename);
            reset_assembler_state(parsed_program);
            continue; /* Move to next file */
        }


        // /* Second pass */
        // if (!second_pass()) {
        //     printf("Error in second pass for %s\n", am_filename);
        //     continue;  /* Move to next file */
        // }

        // /* Generate output files */
        // generate_object_file(ob_filename);

        // has_entries = check_entries();
        // has_externals = check_externals();

        // if (has_entries) {
        //     generate_entries_file(ent_filename);
        // }

        // if (has_externals) {
        //     generate_externals_file(ext_filename);
        // }

        /* Reset for next file */
        reset_assembler_state(parsed_program);
    }

    return 0;
}

void reset_assembler_state(ParsedProgram *parsed_program) {
    free_symbol_table();
    free_parsed_program(parsed_program);
}

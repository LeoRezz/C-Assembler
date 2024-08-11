/* In a header file, e.g., program.h */
#define MAX_PROGRAM_LINES 1000 /* Adjust as needed */
#include "parser.h"
struct ParsedProgram {
    struct Line *lines[MAX_PROGRAM_LINES];
    int line_count;
};

/* Function prototypes */
struct ParsedProgram *create_parsed_program(void);
void add_line_to_program(struct ParsedProgram *program, Line *line);
void free_parsed_program(struct ParsedProgram *program);
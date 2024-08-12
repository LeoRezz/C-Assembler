#include "tokenizer.h"

/* Main for testing */
int main() {
	FILE *input_file;
	char *p;
	Token *tokens_array;
	int token_count;
	int i;
	char line[MAX_LINE];
	
	/* test of detecting diffrenet lexical errors, syntax errors will be detected in the parser */
	input_file = fopen("Invalid_source_code.txt","r"); 
    TRY(input_file); /* TRY macro checks for errors while opening file */
    
	/* Read each line from the source file */
	while (fgets(line, MAX_LINE, input_file)) {
		if (p = strrchr(line, '\n')) *p = '\0'; /* Remove the newline character at the end*/

		/* Tokenizes a given line of assembly code into an array of tokens with assigned type. */
		tokens_array = tokenize_line(line, &token_count);
        
        /* output printer */
		for (i = 0; i < token_count; i++) {
			printf("%s\t%s\n", tokens_array[i].value, token_type_to_string(tokens_array[i].type));
		}
		printf("\n");
	}
	free(tokens_array);
	fclose(input_file);
	return 0;
}
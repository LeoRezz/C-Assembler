#include "util.h"
#define MAX_LINE 1000
#define MAX_WORD 100

int main() {
  char line[MAX_LINE];
  char word[MAX_WORD];
  const char *line_ptr;
  int error;
  FILE *fp = fopen("./input_test/test.txt", "r");
  if (fp == NULL) {
    printf("Error opening file!\n");
    return 1;
  }

  // Test my_getword function
  while (fgets(line, MAX_LINE, fp) != NULL) {
    line_ptr = line;
    while (my_getword(word, MAX_WORD, &line_ptr, &error) != EOF) {
      printf("Word: %s\n", word);
    }
  }
  fclose(fp);

  return 0;
}
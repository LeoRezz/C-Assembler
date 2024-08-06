#include "util.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_LINE 1000
#define MAX_WORD 100

int main() {
  char line[MAX_LINE];
  char word[MAX_WORD];
  char *line_ptr;
  FILE *fp = fopen("test.txt", "r");
  if (fp == NULL) {
    printf("Error opening file!\n");
    return 1;
  }

  // Test my_getword function
  while (fgets(line, MAX_LINE, fp) != NULL) {
    line_ptr = line;
    while (my_getword(word, MAX_WORD, &line_ptr) != EOF) {
      printf("Word: %s\n", word);
    }
  }
  fclose(fp);

  return 0;
}
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_LINE 82

struct label {
    int location;
    char *label;
};

struct label labeltable[100];
int main(int argc, char *argv[]) {
    FILE *input_file = NULL;
    char *file_name = NULL;
    char line[MAX_LINE];
    if (argc != 2) {
        printf("Usage: %s <Enter .ob file name here>\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    /* Open files*/
    input_file = fopen(argv[1], "r");
    if (!input_file) {
        printf("Failed to open input file: %s\n", argv[1]);
        exit(EXIT_FAILURE);
    }

    int i = 0;
    int j = 0;
    char *token = 0;
    char buffer[MAX_LINE];
    char *temp;
    int nooflabels = 0;
    int counter = 100;

    while ((fgets(buffer, MAX_LINE, input_file)) != NULL) {
        printf("fgets is: %s\n", buffer);
        /*Checks for the first word only*/
        token = strtok(buffer, "\n\t ");
        if (token) {
            if (token[strlen(token) - 1] == ':') {
                char *p = token;
                p[strlen(token) - 1] = 0;
                temp = (char *)malloc(strlen(token) + 1);
                if (temp) {
                    strcpy(temp, token);
                    labeltable[nooflabels].label = temp;
                    labeltable[nooflabels].location = counter;
                    printf("%d\tlabel\t%s\n", labeltable[nooflabels].location, labeltable[nooflabels].label);
                    nooflabels++;
                }
            }
        }
        counter++;
    }

    /*This prints the tokenArray
     for (j = 0; j < i; j++) {
         printf("%s\n", tokenArray[j]);
     }*/
    return 0;
}
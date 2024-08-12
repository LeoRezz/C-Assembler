#include <stdio.h>
#include <ctype.h>
#include <string.h>

/*
    Exercise 6-1. Our version of getword does not properly handle underscores, string constants, comments, or preprocessor control lines. Write a better version.
*/

struct key {
    char *word;
    int count;
} keytab[] = { // Current keywords as of C17 standard (ISO/IEC 9899:2017). These must be in alphabetical order
    { "_Alignas", 0 },      { "_Alignof", 0 },  { "_Atomic", 0 },           { "_Bool", 0 },         { "_Complex", 0 },  { "_Generic", 0 },
    { "_Imaginary", 0 },    { "_Noreturn", 0 }, { "_Static_assert", 0 },    { "_Thread_local", 0 }, { "auto", 0 },      { "break", 0 },
    { "case", 0 },          { "char", 0 },      { "const", 0 },             { "continue", 0 },      { "default", 0 },   { "do", 0 },
    { "double", 0 },        { "else", 0 },      { "enum", 0 },              { "extern", 0 },        { "float", 0 },     { "for", 0 },
    { "goto", 0 },          { "if", 0 },        { "inline", 0 },            { "int", 0 },           { "long", 0 },      { "register", 0 },
    { "restrict", 0 },      { "return", 0 },    { "short", 0 },             { "signed", 0 },        { "sizeof", 0 },    { "static", 0 },
    { "struct", 0 },        { "switch", 0 },    { "typedef", 0 },           { "union", 0 },         { "unsigned", 0 },  { "void", 0 },
    { "volatile", 0 },      { "while", 0 }
};

#define MAXWORD 100
#define NKEYS (int)(sizeof keytab / sizeof keytab[0])
#define BUFSIZE 100

int getword(char *, int);
int binsearch(char *, struct key *, int);
int getch(void);
void ungetch(int);

enum statusTypes { NORMAL, STRING, SINGLE_LINE_COMMENT, MULTI_LINE_COMMENT, PREPROCESSOR };

char prevChar;      // keep track of last char
int type;           // keep track of the current status
char buf[BUFSIZE];  // buffer for ungetch
int bufp = 0;       // next free position in buf

// count C keywords
int main()
{
    int n;
    char word[MAXWORD];
    while (getword(word, MAXWORD) != EOF)
    {
        if (word[0] == '"')
        {
            if (type == NORMAL && prevChar != '\'')
                type = STRING;
            else if (type == STRING)
                type = NORMAL;
        }
        else if (word[0] == '/')
        {
            if (prevChar == '/' && type == NORMAL)
                type = SINGLE_LINE_COMMENT;
            else if (type == MULTI_LINE_COMMENT && prevChar == '*')
                type = NORMAL;
        }
        else if (word[0] == '\n' && (type == SINGLE_LINE_COMMENT || type == PREPROCESSOR))
            type = NORMAL;
        else if (word[0] == '*' && prevChar == '/' && type == NORMAL)
            type = MULTI_LINE_COMMENT;
        else if (word[0] == '#' && prevChar == '\n')
            type = PREPROCESSOR;

        if (type == NORMAL && (isalpha(word[0]) || word[0] == '_'))
            if ((n = binsearch(word, keytab, NKEYS)) >= 0)
                keytab[n].count++;
        if (strlen(word) == 1)
            prevChar = word[0];
    }
    for (n = 0; n < NKEYS; n++)
        if (keytab[n].count > 0)
            printf("%4d %s\n", keytab[n].count, keytab[n].word);
    return 0;
}

// find word in tab[0] ... tab[n-1], tab[] must be in alphabetical order
int binsearch(char *word, struct key tab[], int n)
{
    int result, mid, low = 0, high = n - 1;
    while (low <= high)
    {
        mid = (low + high) / 2;
        if ((result = strcmp(word, tab[mid].word)) < 0)
            high = mid - 1;
        else if (result > 0)
             low = mid + 1;
        else
            return mid;
    }
    return -1;
}

// get next word or character from input
int getword(char *word, int lim)
{
    int c;
    char *w = word;
    while ((c = getch()) == '\t' || c == ' ')
        ;
    if (c != EOF)
        *w++ = c;
    if (!isalpha(c) && c != '_')
    {
        *w = '\0';
        return c;
    }
    for ( ; --lim > 0; w++)
        if (!isalnum(*w = getch()) && *w != '_')
        {
            ungetch(*w);
            break;
        }
    *w = '\0';
    return word[0];
}

// get a (possibly pushed back) character
int getch(void)
{
    return (bufp > 0) ? buf[--bufp] : getchar();
}

// push character back on input
void ungetch(int c)
{
    if (bufp >= BUFSIZE)
        printf("ungetch: too many characters\n");
    else
        buf[bufp++] = c;
}
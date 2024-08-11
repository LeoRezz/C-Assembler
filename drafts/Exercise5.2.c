#include <stdio.h>
/*Exercise 5-3. Write a pointer version of the function strcat that we showed in
Chapter 2: strcat(s,t) copies the string t to the end of s.*/

/* strcat: concatenate t to end of s; s must be big enough */
void strcatt(char *s, char *t) {
  // Move s to the end of the string
  while (*s) {
    s++;
  }
  // Copy t to the end of s
  while ((*s++ = *t++))
    ;
}

int main() {
  char a[20] = "abc";
  char b[20] = "defghijk";
  strcatt(a, b);
  printf("%s", a);
  return 0;
}
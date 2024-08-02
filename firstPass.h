#ifndef FIRST_PASS_H
#define FIRST_PASS_H

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define TRY(a)      \
    if (!(a)) {     \
        perror(#a); \
        exit(1);    \
    }


#endif
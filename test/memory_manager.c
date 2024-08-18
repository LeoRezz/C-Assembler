#include "memory_manager.h"
#include <stdio.h>

static int IC;
static int DC;

/* Initilizing the IC and DC */
void init_memory_counters(void) {
    IC = INITIAL_ADDRESS;
    DC = 0;
}
/* used by the parser to increment IC acoording to word length*/
int increment_IC(int amount) {
    /* Checks if memory is at full capacity */
    if (IC + DC + amount > MEMORY_SIZE) {
        printf("Error: Memory limit exceeded\n");
        return 0;
    }
    IC += amount;
    return 1;
}

/* used by the parser to increment DC acoording to data length*/
int increment_DC(int amount) {
    if (IC + DC + amount > MEMORY_SIZE) {
        printf("Error: Memory limit exceeded\n");
        return 0;
    }
    DC += amount;
    return 1;
}

/* Returns the Instruction count */
int get_IC(void) {
    return IC;
}
/* Return the Data count */
int get_DC(void) {
    return DC;
}

#include "memory_manager.h"
#include <stdio.h>

static int IC;
static int DC;

void init_memory_counters(void) {
    IC = INITIAL_ADDRESS;
    DC = 0;
}

int increment_IC(int amount) {
    if (IC + DC + amount > MEMORY_SIZE) {
        printf("Error: Memory limit exceeded\n");
        return 0;
    }
    IC += amount;
    return 1;
}

int increment_DC(int amount) {
    if (IC + DC + amount > MEMORY_SIZE) {
        printf("Error: Memory limit exceeded\n");
        return 0;
    }
    DC += amount;
    return 1;
}

int get_IC(void) {
    return IC;
}

int get_DC(void) {
    return DC;
}

int get_total_words(void) {
    return IC + DC - INITIAL_ADDRESS;
}
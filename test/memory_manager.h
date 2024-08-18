#ifndef MEMORY_MANAGER_H
#define MEMORY_MANAGER_H

#define MEMORY_SIZE 4096
#define INITIAL_ADDRESS 100

void init_memory_counters(void);
int increment_IC(int amount);
int increment_DC(int amount);
int get_IC(void);
int get_DC(void);

#endif
#ifndef MEMORY_MANAGER_H
#define MEMORY_MANAGER_H

/* The assembler memory manager, resposible for incrementing IC and DC */

/* Constants */
#define MEMORY_SIZE 4096
#define INITIAL_ADDRESS 100

/* Function prototypes */

/* Initilizing IC to his Initial address and DC to 0 */
void init_memory_counters(void);
/* Incrementing IC and DC by the given amount */
int increment_IC(int amount);
int increment_DC(int amount);
/* Returns the Instruction count and Data count */
int get_IC(void);
int get_DC(void);

#endif
#include "symbolTable.h"
#define MAX_LABEL_LENGTH 31
#define INITIAL_SYMBOL_TABLE_SIZE 20

static void grow_symbol_table(void);

/* Global state, Instruction and Data count*/
extern int IC;
extern int DC;
extern int current_line;

static symbol *symbol_table;
static int symbol_count;
static int symbol_capacity;

/*Initialize the symbol table */
void init_symbol_table(void) {
    symbol_table = calloc(INITIAL_SYMBOL_TABLE_SIZE , sizeof(symbol));
    if (symbol_table == NULL) { /* Handle allocation error */
        printf("Failed to allocate memory for SymbolTable\n");
        exit(1);
    }
    symbol_count = 0;
    symbol_capacity = INITIAL_SYMBOL_TABLE_SIZE;
}

static int is_duplicate_symbol(char *name, int value, SymbolType type) {
    symbol *symbol_duplicate = find_symbol(name);

    /* Checks if the symbol already exists */
    if (symbol_duplicate != NULL) {
        /* if the symbol is an entry, update it's value to current IC */
        if (symbol_duplicate->type == SYMBOL_ENTRY) {
            return 0;
        }

        /* Checks if the symbol is of the same type */
        if (symbol_duplicate->type == type) {
            return 1;
        }
    }

    return 0;
}

/* Add a symbol to the table */
int add_symbol(char *name, int value, SymbolType type) {

    if (is_duplicate_symbol(name,value, type)) {
        return 0;
    }

    if (symbol_count >= symbol_capacity) {
        grow_symbol_table();
    }

    strncpy(symbol_table[symbol_count].name, name, MAX_LABEL_LENGTH);
    symbol_table[symbol_count].name[MAX_LABEL_LENGTH] = '\0'; /* Ensure null-termination */
    symbol_table[symbol_count].type = type;
    switch (type) {
    case SYMBOL_CODE:
        symbol_table[symbol_count].value = value;
        break;
    case SYMBOL_DATA:
        symbol_table[symbol_count].value = value;
        break;
    case SYMBOL_ENTRY:
        symbol_table[symbol_count].value = 0;
        break;
    case SYMBOL_EXTERNAL:
        symbol_table[symbol_count].value = 0;
        break;
    default:
        printf("Error: Unknown symbol type\n");
        return 0;
    }

    symbol_count++;
    return 1;
}

static void grow_symbol_table(void) {
    int new_capacity;
    symbol *new_table;

    new_capacity = symbol_capacity * 2;
    new_table = (symbol *) realloc(symbol_table, new_capacity * sizeof(symbol));
    if (new_table == NULL) {
        /* Handle reallocation error*/
        printf("Failed to reallocate memory for symbol_table\n");
        exit(1);
    }
    symbol_table = new_table;
    symbol_capacity = new_capacity;
}

symbol* get_symbol_table() {
    return symbol_table;
}

int get_symbol_count() {
    return symbol_count;
}

/* Look up a symbol in the table */
symbol *find_symbol(char *name) {
    int i;
    for (i = 0; i < symbol_count; i++) {
        if (strcmp(symbol_table[i].name, name) == 0) {
            return &symbol_table[i];
        }
    }
    return NULL;
}

void update_data_symbols(int IC) {
    int i;
    for (i = 0; i < symbol_count; i++) {
        if (symbol_table[i].type == SYMBOL_DATA) {
            symbol_table[i].value += IC;
        }
    }
}

static int compare_symbols(const void *a, const void *b) {
    return ((symbol*)a)->value - ((symbol*)b)->value;
}

int resolve_entrys() {
    int i;
    symbol *entry_refrence;
    for (i = 0; i < symbol_count; i++) {
        if (symbol_table[i].type == SYMBOL_ENTRY) {
            entry_refrence = find_entry_refrence(symbol_table[i].name);
            if (entry_refrence == NULL) {
                printf("Error: Entry symbol value not found\n");
                return 0;
            }
            symbol_table[i].value = entry_refrence->value;
        }
    }
}

/* Look up an entry symbol's refrence address in the table */
symbol *find_entry_refrence(char *name) {
    int i;
    for (i = 0; i < symbol_count; i++) {
        if ((strcmp(symbol_table[i].name, name) == 0) && (symbol_table[i].type != SYMBOL_ENTRY)) {
            return &symbol_table[i];
        }
    }
    return NULL;
}

void print_symbol_table() {
    int i;
    qsort(symbol_table, symbol_count, sizeof(symbol), compare_symbols);
    printf("\nSymbol Table, Sorted by value (Sorted only at printing, table itself is by order of insertion)\n");
    printf("%-32s %-10s %-10s\n", "Name", "Value", "Type");
    printf("-------------------------------- ---------- ----------\n");
    for (i = 0; i < symbol_count; i++) {
        printf("%-32s %-10d ", symbol_table[i].name, symbol_table[i].value);
        switch (symbol_table[i].type) {
        case SYMBOL_CODE:
            printf("CODE\n");
            break;
        case SYMBOL_DATA:
            printf("DATA\n");
            break;

        case SYMBOL_ENTRY:
            printf("ENTRY\n");
            break;

        case SYMBOL_EXTERNAL:
            printf("EXTERNAL\n");
            break;

        default:
            printf("UNKNOWN\n");
        }
    }
    printf("\nTotal symbols: %d\n", symbol_count);
}

/* Clean up the symbol table */
void free_symbol_table() {
    free(symbol_table);
    symbol_table = NULL;
    symbol_count = 0;
    symbol_capacity = 0;
}
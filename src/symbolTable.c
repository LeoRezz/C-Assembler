#include "symbolTable.h"
#include "macro_table.h"
#define MAX_LABEL_LENGTH 31
#define INITIAL_SYMBOL_TABLE_SIZE 20

static void grow_symbol_table(void);

/* Symbol table, current element number, and capacity */
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

/* Checks if a symbol is already exists in the table */
static int is_duplicate_symbol(char *name, int value, SymbolType origin_type) {
    symbol *symbol_duplicate = find_symbol(name);
    
    /* Checks if there is a duplicate symbol with the same name we want to add */
    if (symbol_duplicate != NULL) {
        /* If the duplicate names are of the same type, we return true */
        if (symbol_duplicate->type == origin_type) {
            return 1; /* TRUE, it's a dup */
        }
        switch (origin_type) {
        case SYMBOL_CODE:
        case SYMBOL_DATA:
            if (((symbol_duplicate->type == SYMBOL_CODE) || (symbol_duplicate->type == SYMBOL_DATA)) ) {
                return 1;
            }
            break;
        case SYMBOL_ENTRY:
            if (((symbol_duplicate->type == SYMBOL_ENTRY) || (symbol_duplicate->type == SYMBOL_EXTERNAL))) {
                return 1;
            }
            break;

        case SYMBOL_EXTERNAL:
            return 1;
            break;
        default:
            break;
        }
    }

    return 0; /*  FALSE: , not a dup */
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
    case SYMBOL_MACRO:
        symbol_table[symbol_count].value = 0;
        break;

    default:
        printf("Error: Unknown symbol type\n");
        return 0;
    }

    symbol_count++;
    return 1;
}


/* Reallocates memory to the symbol table to accommodate new symboles */
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

/* Getter for the symbol table */
symbol* get_symbol_table() {
    return symbol_table;
}
/* Getter for the total number of symbols */
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
/* Updates the symbol table at the end of the first pass to ensure data is after code in memory */
void update_data_symbols(int IC) {
    int i;
    for (i = 0; i < symbol_count; i++) {
        if (symbol_table[i].type == SYMBOL_DATA) {
            symbol_table[i].value += IC;
        }
    }
}
/* Resolves addresses of entry labels declered in the source code */
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
            if (entry_refrence->type == SYMBOL_EXTERNAL) {
                printf("Error: External symbol can't be used in entry directive\n");
                return 0;
            }
            symbol_table[i].value = entry_refrence->value;
        }
    }
    return 1;
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


/* Clean up the symbol table */
void free_symbol_table() {
    free(symbol_table);
    symbol_table = NULL;
    symbol_count = 0;
    symbol_capacity = 0;
}
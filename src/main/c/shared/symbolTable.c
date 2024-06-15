#include "symbolTable.h"

unsigned int hash(const char *str, int size) {
    unsigned int hash = 0;
    while (*str) {
        hash = (hash << 5) + *str++;
    }
    return hash % size;
}

void init_symbol_table(SymbolTable *table, int size) {
    table->table = (SymbolEntry **)malloc(size * sizeof(SymbolEntry *));
    table->size = size;
    table->current_scope = 0;
    for (int i = 0; i < size; i++) {
        table->table[i] = NULL;
    }
}

void free_symbol_table(SymbolTable *table) {
    for (int i = 0; i < table->size; i++) {
        SymbolEntry *entry = table->table[i];
        while (entry) {
            SymbolEntry *next = entry->next;
            free(entry->id);
            free(entry->attributes);
            free(entry);
            entry = next;
        }
    }
    free(table->table);
}

SymbolEntry *create_symbol_entry(SymbolType type, const char *id, int scope_level) {
    SymbolEntry *entry = (SymbolEntry *)malloc(sizeof(SymbolEntry));
    entry->type = type;
    entry->id = strdup(id);
    entry->attributes = NULL;
    entry->next = NULL;
    entry->scope_level = scope_level;
    return entry;
}

void insert_symbol(SymbolTable *table, SymbolEntry *entry) {
    unsigned int index = hash(entry->id, table->size);
    entry->next = table->table[index];
    table->table[index] = entry;
}

SymbolEntry *find_symbol(SymbolTable *table, const char *id, int scope_level) {
    unsigned int index = hash(id, table->size);
    SymbolEntry *entry = table->table[index];
    while (entry) {
        if (strcmp(entry->id, id) == 0 && entry->scope_level == scope_level) {
            return entry;
        }
        entry = entry->next;
    }
    if (scope_level > 0) {
        return find_symbol(table, id, scope_level - 1);
    }
    return NULL;
}

void enter_scope(SymbolTable *table) {
    table->current_scope++;
}

void exit_scope(SymbolTable *table) {
    table->current_scope--;
}

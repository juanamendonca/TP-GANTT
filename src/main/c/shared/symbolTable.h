#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

typedef enum {
    PROJECT_SYMBOL,
    TASK_SYMBOL,
    CATEGORY_SYMBOL
} SymbolType;

typedef struct {
    char *name;
    char *id;
    char *format;
    char *depends_on;
    char **with;
    int max_points;
    int max_tasks;
} ProjectAttributes;

typedef struct {
    char *id;
    char *name;
    int lengthStart;
    int lengthFinish;
    char *depends_on;
    char *start;
    char *finish;
    int points;
    char *category;
    char *unique;
} TaskAttributes;

typedef struct {
    char *name;
    char *id;
} CategoryAttributes;

typedef struct SymbolEntry {
    SymbolType type;
    char *id;
    void *attributes;
    struct SymbolEntry *next;
    int scope_level;
} SymbolEntry;

typedef struct {
    SymbolEntry **table;
    int size;
    int current_scope;
} SymbolTable;

void init_symbol_table(SymbolTable *table, int size);
void free_symbol_table(SymbolTable *table);
SymbolEntry *create_symbol_entry(SymbolType type, const char *id, int scope_level);
void insert_symbol(SymbolTable *table, SymbolEntry *entry);
SymbolEntry *find_symbol(SymbolTable *table, const char *id, int scope_level);
void enter_scope(SymbolTable *table);
void exit_scope(SymbolTable *table);

#endif // SYMBOL_TABLE_H

#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

typedef struct {
    char *name;
    char *format;
    char *depends_on;
    char **with;
    int max_points;
    int max_tasks;
} ProjectAttributes;

typedef struct {
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
} CategoryAttributes;

typedef struct{
    char *type;
    char *id;
    union {
        ProjectAttributes *projectAttrs;
        TaskAttributes *taskAttrs;
        CategoryAttributes *categoryAttrs;
    } attributes;
    int scopeLevel;
}Symbol;

typedef struct Node{
    Symbol symbol;
    struct Node * next;
}Node;

typedef struct List{ 
    Node * head;
    int length;
    int current_scope;
}List;


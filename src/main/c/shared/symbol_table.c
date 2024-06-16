#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "symbol_table.h"


Node * newNode(Symbol * symbol);
void destroyNode(Node * node);
void insert(List * list, Symbol * symbol);
Symbol * findType(List * list, char * type);
void update (List * list, char * symbol, ProjectAttributes *attributes);


Node * newNode(Symbol * symbol){
    Node * newNode = (Node*) malloc(sizeof(Node));
    strcpy(newNode->symbol.type, symbol->type);
    newNode->symbol.scopeLevel=0;
    newNode->next=NULL;
    return newNode;
}

void insert(List * list, Symbol * symbol){
    Node * node = newNode(symbol);
    node -> next = list->head;
    list->head = node;
    list->length++;
}

Symbol * findbyType(List * list, char * type){
    Node *aux = list -> head;
    while(aux)
    {
        if (strcmp(aux->symbol.type,type) == 0){
            return &aux->symbol;
        }
        aux = aux->next;
    }
    return NULL;
}
Symbol * findbyTypeandId(List * list, char * type, char * id){
    Node *aux = list -> head;
    while(aux)
    {
        if (strcmp(aux->symbol.type,type) == 0 && strcmp(aux->symbol.id,id)){
            return &aux->symbol;
        }
        aux = aux->next;
    }
    return NULL;
}

void updateSymbolProject (List * list, char * type, ProjectAttributes *attributes){
    Symbol * searchedSymbol = findbyType(list,type);
    if(searchedSymbol == NULL){
        //enter_scope();
    } 
    searchedSymbol->attributes.projectAttrs= attributes;

}


/* SymbolTable* insertSymbol(SymbolTable * symbolTable, char *type, char *id) {
    SymbolTable *newSymbol = (SymbolTable *)malloc(sizeof(SymbolTable));
    newSymbol->type = strdup(type);
    newSymbol->id = strdup(id);
    newSymbol->attributes.projectAttrs = NULL;
    newSymbol->attributes.taskAttrs = NULL;
    newSymbol->attributes.categoryAttrs = NULL;
    newSymbol->next = symbolTable;
    symbolTable = newSymbol;
    return newSymbol;
}
SymbolTable* findSymbol(SymbolTable * symbolTable, char *id) {
    SymbolTable *current = symbolTable;
    while (current != NULL) {
        if (strcmp(current->id, id) == 0) {
            return current;
        }
        current = current->next;
    }
    return NULL;
}
void updateSymbolProject(char *id, ProjectAttributes *attributes, SymbolTable * symbolTable) {
  //  SymbolTable *symbol = findSymbol(symbolTable, id);
/*     if (symbol != NULL && strcmp(symbol->type, "PROJECT") == 0) {
        symbol->attributes.projectAttrs = attributes;
    } 
}

void updateSymbolTask(char *id, TaskAttributes *attributes, SymbolTable * symbolTable) {
    SymbolTable *symbol = findSymbol(symbolTable,id);
    if (symbol != NULL && strcmp(symbol->type, "TASK") == 0) {
        symbol->attributes.taskAttrs = attributes;
    }
}

void updateSymbolCategory(char *id, CategoryAttributes *attributes, SymbolTable * symbolTable ){
    SymbolTable *symbol = findSymbol(symbolTable,id);
    if (symbol != NULL && strcmp(symbol->type, "CATEGORY") == 0) {
        symbol->attributes.categoryAttrs = attributes;
    }
}
 */
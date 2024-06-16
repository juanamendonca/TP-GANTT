#ifndef TYPE_HEADER
#define TYPE_HEADER
#include "uthash.h" 

typedef enum {
	false = 0,
	true = 1
} boolean;

typedef int Token;

struct category{
    char * categoryId;            /* we'll use this field as the key */
	char *name;
    UT_hash_handle hh; /* makes this structure hashable */
};

struct task{
    char * taskId;            /* we'll use this field as the key */
	char *name;
    int lengthStart;
    int lengthFinish;
    char *depends_on;
    char *start;
    char *finish;
    int points;
    char *category;
    char *unique;
    UT_hash_handle hh; /* makes this structure hashable */
};

// Main Hash Table
struct project{
    char * projectId;            /* we'll use this field as the key */
	struct task * tasks; // Hashtable de tareas (CLAVE es el id, VALOR)
	struct category * categories; // Hashtable de CATEGORIAS (CLAVE es el id, VALOR)
	char *name;
    char *format;
    char *depends_on;
    char **with;
    int max_points;
    int max_tasks;
    UT_hash_handle hh; /* makes this structure hashable */
};


#endif

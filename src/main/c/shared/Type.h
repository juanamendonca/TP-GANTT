#ifndef TYPE_HEADER
#define TYPE_HEADER
#include "uthash.h" 

#define MAX_LIST 100

typedef enum {
	false = 0,
	true = 1
} boolean;

typedef int Token;

struct Category{
    char * categoryId;            /* we'll use this field as the key */
	char *name;
    UT_hash_handle hh; /* makes this structure hashable */
};

struct Task{
    char * taskId;            /* we'll use this field as the key */
	char *name;
    int lengthStart;
    int lengthFinish;
    char *start;
    char *finish;
    int points;
    char *category;
    char *unique;
    int base;
    UT_hash_handle hh; /* makes this structure hashable */
    char *depends_on[MAX_LIST];
};

// Main Hash Table
struct Project{
    char * projectId;            /* we'll use this field as the key */
	char * name;
	struct Task * tasks; // Hashtable de tareas (CLAVE es el id, VALOR)
	struct Category * categories; // Hashtable de CATEGORIAS (CLAVE es el id, VALOR)
	int format;
    char * depends_on;
    int max_points;
    int max_tasks;
    int projectNumber;
    int baseFlag;
    UT_hash_handle hh; /* makes this structure hashable */
    char * with[MAX_LIST];
};


#endif

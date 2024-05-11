#ifndef ABSTRACT_SYNTAX_TREE_HEADER
#define ABSTRACT_SYNTAX_TREE_HEADER

#include "../../shared/Logger.h"
#include <stdlib.h>

/** Initialize module's internal state. */
void initializeAbstractSyntaxTreeModule();

/** Shutdown module's internal state. */
void shutdownAbstractSyntaxTreeModule();

/**
 * This typedefs allows self-referencing types.
 */

typedef enum RecursiveType RecursiveType;
typedef enum OptionalsType OptionalsType;
typedef enum ProjectOptionalsType ProjectOptionalsType;
typedef enum TaskLengthFormatType TaskLengthFormatType;
typedef enum TaskOptionalsType TaskOptionalsType;
typedef enum ProjectBodyOptionalsType ProjectBodyOptionalsType;

typedef struct Program Program;
typedef struct ProjectStructure ProjectStructure;
typedef struct ProjectBody ProjectBody;
typedef struct ProjectOptionals ProjectOptionals;
typedef struct ProjectUnion ProjectUnion;
typedef struct TaskList TaskList;
typedef struct TaskStructure TaskStructure;
typedef struct TaskLengthFormat TaskLengthFormat;
typedef struct TaskOptionals TaskOptionals;
typedef struct TaskOptionDependsOn TaskOptionDependsOn;
typedef struct ProjectBodyOptionals ProjectBodyOptionals;
typedef struct BodyCategoriesOption BodyCategoriesOption;

/**
 * Node types for the Abstract Syntax Tree (AST).
 */

enum RecursiveType {
	SINGLE,
	MULTIPLE
};

enum OptionalsType {
	BASIC,
	OPTIONALS
};

enum ProjectOptionalsType{
	DEPENDS_ON,
	WITH,
	BOTH
};

enum TaskLengthFormatType{
	TIME_PERIOD,
	DURATION
};

enum TaskOptionalsType{
	CATEGORY,
	POINTS,
	DEPENDS,
	UNIQUE,
	CATEGORY_POINTS,
	CATEGORY_DEPENDS,
	CATEGORY_UNIQUE,
	POINTS_DEPENDS,
	POINTS_UNIQUE,
	DEPENDS_UNIQUE,
	CATEGORY_POINTS_DEPENDS,
	CATEGORY_DEPENDS_UNIQUE,
	POINTS_DEPENDS_UNIQUE,
	CATEGORY_POINTS_DEPENDS_UNIQUE
};

enum ProjectBodyOptionalsType{
	TASKS,
	CATEGORIES,
	POINTS,
	START,
	TASKS_CATEGORIES,
	TASKS_POINTS,
	TASKS_START,
	CATEGORIES_POINTS,
	CATEGORIES_START,
	POINTS_START,
	TASKS_CATEGORIES_POINTS,
	TASKS_POINTS_START,
	CATEGORIES_POINTS_START,
	TASKS_CATEGORIES_POINTS_START,
};

struct BodyCategoriesOption{
	char * id;
	char * name;
};

struct ProjectBodyOptionals {
	int maxTasks;
	int maxPoints;
	char * startDate;
	char * id;
	char * name;
	BodyCategoriesOption * bodyCategoriesOption;
	ProjectBodyOptionalsType type;
};

struct TaskOptionDependsOn{
	char * id;
	char * id2;
};

struct TaskOptionals {
	char * id1;
	char * id2;
	char * id3;
	int points;
	TaskOptionDependsOn * taskOptionDependsOn;
	TaskOptionalsType type;
};

struct TaskLengthFormat {
	union {
		struct{
			char * startDate;
			char * finishDate;
		};
		char * interval;
	};
	TaskLengthFormatType type;
};

struct TaskStructure {
	char * id;
	char * name;
	TaskLengthFormat * taskLengthFormat;
	TaskOptionals * taskOptionals;
	OptionalsType type;
};

struct TaskList {
	TaskList * taskList;
	TaskStructure * taskStructure;
	RecursiveType type;
};

struct ProjectBody {
	ProjectBodyOptionals * projectBodyOptionals;
	TaskList * taskList;
	OptionalsType type;
};

struct ProjectUnion {
	ProjectUnion * projectUnion;
	char * id;
};

struct ProjectOptionals {
	char * id1;
	char * id2;
	ProjectUnion * projectUnion;
	ProjectOptionalsType type;
};

struct ProjectStructure {
	char * id;
	char * name;
	ProjectBody * projectBody;
	ProjectOptionals * projectOptionals;
	OptionalsType type;
};

struct Program {
	ProjectStructure * projectStructure;
	Program * program;
	RecursiveType type;
};

/**
 * Node recursive destructors.
 */
// void releaseConstant(Constant * constant);
// void releaseExpression(Expression * expression);
// void releaseFactor(Factor * factor);
// void releaseProgram(Program * program);

#endif

// struct BodyCategoriesOption{
// 	char * id;
// 	char * name;
// };

// struct ProjectBodyOptionals {
// 	union {
// 		int max;
// 		char * startDate;
// 		struct{
// 			char * id;
// 			char * name;
// 			BodyCategoriesOption * bodyCategoriesOption;
// 		};
// 		struct{
// 			int max1;
// 			char * id2;
// 			char * name1;
// 			BodyCategoriesOption * bodyCategoriesOption;
// 		};
// 		struct{
// 			int maxTasks;
// 			int maxPoints;
// 		};
// 		struct{
// 			int max2;
// 			char * startDate1;
// 		};
// 		struct{
// 			int max3;
// 			char * id3;
// 			char * name2;
// 			BodyCategoriesOption * bodyCategoriesOption;
// 			char * startDate2;
// 		};
// 		struct{
// 			char * id4;
// 			char * name3;
// 			BodyCategoriesOption * bodyCategoriesOption;
// 			char * startDate3;
// 		};
// 		struct{
// 			int maxTasks2;
// 			char * id5;
// 			char * name4;
// 			BodyCategoriesOption * bodyCategoriesOption;
// 			int maxPoints2;
// 		};
// 		struct{
// 			int maxTasks3;
// 			int maxPoints3;
// 			char * startDate4;
// 		};
// 		struct{
// 			int maxTasks4;
// 			int maxPoints4;
// 			char * startDate5;
// 			char * id6;
// 			char * name5;
// 			BodyCategoriesOption * bodyCategoriesOption;
// 		};
// 	};
// 	ProjectBodyOptionalsType type;
// };

// struct TaskOptionDependsOn{
// 	char * id;
// 	char * id2;
// };

// struct TaskOptionals {
// 	union {
// 		char * id;
// 		int points;
// 		struct{
// 			char * id1;
// 			char * id2;
// 			TaskOptionDependsOn * taskOptionDependsOn;
// 		};
// 		struct{
// 			char * id3;
// 			int points2;
// 		};
// 		struct{
// 			char * id4;
// 			char * id5;
// 			char * id6;
// 			TaskOptionDependsOn * taskOptionDependsOn;
// 		};
// 		struct{
// 			int points3;
// 			char * id7;
// 			char * id8;
// 			TaskOptionDependsOn * taskOptionDependsOn;
// 		};
// 		struct{
// 			char * id9;
// 			int points4;
// 			char * id10;
// 			char * id11;
// 			TaskOptionDependsOn * taskOptionDependsOn;
// 		};
// 	};
// 	TaskOptionalsType type;
// };

// struct TaskLengthFormat {
// 	union {
// 		struct{
// 			char * startDate;
// 			char * finishDate;
// 		};
// 		struct{
// 			char * interval;
// 		};
// 	};
// 	TaskLengthFormatType type;
// };

// struct TaskStructure {
// 	union {
// 		struct{
// 			char * id;
// 			char * name;
// 			TaskLengthFormat * taskLengthFormat;
// 		};
// 		struct{
// 			char * id;
// 			char * name;
// 			TaskLengthFormat * taskLengthFormat;
// 			TaskOptionals * taskOptionals;
// 		};
// 	};
// 	OptionalsType type;
// };

// struct TaskList {
// 	union {
// 		TaskStructure * taskStructure;
// 		struct{
// 			TaskList * taskList;
// 			TaskStructure * taskStructure;
// 		};
// 	};
// 	RecursiveType type;
// };

// struct ProjectBody {
// 	union {
// 		TaskList * taskList;
// 		struct{
// 			ProjectBodyOptionals * projectBodyOptionals;
// 			TaskList * taskList;
// 		};
// 	};
// 	OptionalsType type;
// };

// struct ProjectUnion {
// 	ProjectUnion * projectUnion;
// 	char * id;
// };

// struct ProjectOptionals {
// 	union {
// 		struct{
// 			char * id;
// 		};
// 		struct{
// 			char * id;
// 			ProjectUnion * projectUnion;
// 		};
// 		struct{
// 			char * id1;
// 			char * id2;
// 			ProjectUnion * projectUnion;
// 		};
// 	};
// 	ProjectOptionalsType type;
// };

// struct ProjectStructure {
// 	union {
// 		struct{
// 			char * id;
// 			char * name;
// 			ProjectBody * projectBody;
// 		};
// 		struct{
// 			char * id;
// 			char * name;
// 			ProjectOptionals * projectOptionals;
// 			ProjectBody * projectBody;
// 		};
// 	};
// 	OptionalsType type;
// };

// struct Program {
// 	union{
// 		ProjectStructure * projectStructure;
// 		struct {
// 			Program * program;
// 			ProjectStructure * projectStructure1;
// 		};
// 	};
// 	RecursiveType type;
// };
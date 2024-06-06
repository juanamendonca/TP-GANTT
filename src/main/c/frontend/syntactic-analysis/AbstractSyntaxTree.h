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
typedef enum ProjectOptionalsType ProjectOptionalsType;
typedef enum TaskLengthFormatType TaskLengthFormatType;

typedef struct Program Program;
typedef struct ProjectStructure ProjectStructure;
typedef struct ProjectStructureCommon ProjectStructureCommon;
typedef struct TimeUnit TimeUnit;
typedef struct CategoryId CategoryId;
typedef struct PointsInteger PointsInteger;
typedef struct DependsOnId DependsOnId;
typedef struct Unique Unique;
typedef struct ProjectStart ProjectStart;
typedef struct MaxTasks MaxTasks;
typedef struct CategoriesId CategoriesId;
typedef struct MaxPoints MaxPoints;
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

enum ProjectOptionalsType{
	DEPENDS_ON_P,
	WITH_P,
	BOTH,
	EMPTY
};

enum TaskLengthFormatType{
	TIME_PERIOD,
	DURATION
};

struct BodyCategoriesOption{
	char * id;
	char * name;
	BodyCategoriesOption * bodyCategoriesOption;
};

struct ProjectStart{
	char * date;
};

struct MaxPoints{
	int points;
};

struct CategoriesId{
	char * id;
	char * name;
	BodyCategoriesOption * bodyCategoriesOption;
};

struct MaxTasks{
	int tasks;
};

struct ProjectBodyOptionals {
	MaxTasks * maxTasks;
	CategoriesId * categoriesId;
	MaxPoints * maxPoints;
	ProjectStart * projectStart;
};

struct TaskOptionDependsOn{
	char * id;
	char * id2;
	TaskOptionDependsOn * taskOptionDependsOn;
};

struct DependsOnId {
	char * id1;
	char * id2;
	TaskOptionDependsOn * taskOptionDependsOn;
};

struct PointsInteger {
	int points;
};

struct CategoryId {
	char * id;
};

struct Unique {
	
};

struct TaskOptionals {
	CategoryId * categoryId;
	PointsInteger * pointsInteger;
	DependsOnId * dependsOnId;
	Unique * unique;
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
};

struct TaskList {
	union {
		TaskStructure * taskStructure1;
		struct{
			TaskList * taskList;
			TaskStructure * taskStructure2;
		};
	};
	RecursiveType type;
};

struct ProjectBody {
	ProjectBodyOptionals * projectBodyOptionals;
	TaskList * taskList;
};

struct ProjectUnion {
	ProjectUnion * projectUnion;
	char * id;
};

struct ProjectOptionals {
	union {
		struct{
			char * id1;
		};
		struct{
			char * id2;
			ProjectUnion * projectUnion1;
		};
		struct{
			char * id3;
			char * id4;
			ProjectUnion * projectUnion2;
		};
	};
	ProjectOptionalsType type;
};

struct TimeUnit{

};

struct ProjectStructureCommon{
	char * id;
	char * name;
	TimeUnit * timeUnit;
};

struct ProjectStructure {
	ProjectStructureCommon * projectStructureCommon;
	ProjectOptionals * projectOptionals;
	ProjectBody * projectBody;
};

struct Program {
	union{
		ProjectStructure * projectStructure;
		struct {
			Program * program;
			ProjectStructure * projectStructure1;
		};
	};
	RecursiveType type;
};

/**
 * Node recursive destructors.
 */
void releaseBodyCategoriesOption(BodyCategoriesOption * bodyCategoriesOption);
void releaseProjectStart(ProjectStart * projectStart);
void releaseMaxPoints(MaxPoints * maxPoints);
void releaseCategoriesId(CategoriesId * categoriesId);
void releaseMaxTasks(MaxTasks * maxTasks);
void releaseProjectBodyOptionals(ProjectBodyOptionals * projectBodyOptionals);
void releaseTaskOptionDependsOn(TaskOptionDependsOn * taskOptionDependsOn);
void releaseDependsOnId(DependsOnId * dependsOnId);
void releasePointsInteger(PointsInteger * pointsInteger);
void releaseCategoryId(CategoryId * categoryId);
void releaseUnique(Unique * unique);
void releaseTaskOptionals(TaskOptionals * taskOptionals);
void releaseTaskLengthFormat(TaskLengthFormat * taskLengthFormat);
void releaseTaskStructure(TaskStructure * taskStructure);
void releaseTaskList(TaskList * taskList);
void releaseProjectBody(ProjectBody * projectBody);
void releaseProjectUnion(ProjectUnion * projectUnion);
void releaseProjectOptionals(ProjectOptionals * projectOptionals);
void releaseProjectStructureCommon(ProjectStructureCommon * projectStructureCommon);
void releaseProjectStructure(ProjectStructure * projectStructure);
void releaseProgram(Program * program);
#endif
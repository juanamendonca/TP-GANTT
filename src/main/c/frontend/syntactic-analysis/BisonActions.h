#ifndef BISON_ACTIONS_HEADER
#define BISON_ACTIONS_HEADER

#include "../../shared/CompilerState.h"
#include "../../shared/Logger.h"
#include "../../shared/Type.h"
#include "AbstractSyntaxTree.h"
#include "SyntacticAnalyzer.h"
#include <stdlib.h>

extern struct project * projects; //Symbol table


/** Initialize module's internal state. */
void initializeBisonActionsModule();

/** Shutdown module's internal state. */
void shutdownBisonActionsModule();

/**
 * Bison semantic actions.
 */

DependsOnId * DependsOnIdSemanticAction(char * id1, char * id2, TaskOptionDependsOn * taskOptionDependsOn);
PointsInteger * PointsIntegerSemanticAction(int points);
CategoryId * CategoryIdSemanticAction(char * id);
TaskOptionals * TaskOptionalSemanticAction(CategoryId * categoryId, PointsInteger * pointsInteger, DependsOnId * dependsOnId, Unique * unique);
TaskLengthFormat * IntervalLengthFormatSemanticAction(int leftInterval, int rightInterval);
TaskLengthFormat * DateLengthFormatSemanticAction(char * startDate, char * finishDate);
TaskStructure * OptionalsStructureSemanticAction(char * id, char * name, TaskLengthFormat * taskLengthFormat, TaskOptionals * taskOptionals);
TaskList * StructureRecursiveListSemanticAction(TaskList * recursiveTaskList, TaskStructure * taskStructure);
TaskList * StructureListSemanticAction(TaskStructure * taskStructure);
ProjectBody * OptionalsProjectBodySemanticAction(ProjectBodyOptionals * projectBodyOptionals, TaskList * taskList);
ProjectUnion * IdProjectUnionSemanticAction(ProjectUnion * recursiveProjectUnion, char * id);
ProjectOptionals * BothOptionalsSemanticAction(char * id1, char * id2, ProjectUnion * projectUnion);
ProjectOptionals * ProjectUnionOptionalsSemanticAction(char * id, ProjectUnion * projectUnion);
ProjectOptionals * IdOptionalsSemanticAction(char * id);
ProjectStructure * OptionalsProjectStructureSemanticAction(ProjectStructureCommon  * projectStructureCommon, ProjectOptionals * projectOptionals, ProjectBody * projectBody);
Program * ProjectStructureRecursiveProgramSemanticAction(CompilerState * compilerState, Program * recursiveProgram, ProjectStructure * projectStructure);
Program * ProjectStructureProgramSemanticAction(CompilerState * compilerState, ProjectStructure * projectStructure);
BodyCategoriesOption * RecursiveCategoriesOptionSemanticAction(BodyCategoriesOption * recursiveBodyCategoriesOption, char * id, char * name);
TaskOptionDependsOn * TaskOptionDependsOnSemanticAction(TaskOptionDependsOn * recursiveTaskOptionDependsOn, char * id, char * id2);
ProjectStart * ProjectStartSemanticAction(char * date);
MaxPoints * MaxPointsSemanticAction(int points);
CategoriesId * CategoriesIdSemanticAction(char * id, char * name, BodyCategoriesOption * bodyCategoriesOption);
MaxTasks * MaxTasksSemanticAction(int tasks);
ProjectBodyOptionals * ProjectBodyOptionalsSemanticAction(MaxTasks * maxTasks, CategoriesId * categoriesId, MaxPoints * maxPoints, ProjectStart * projectStart);
ProjectStructureCommon * ProjectStructureCommonSemanticAction(char * id, char * name, TimeUnit * timeUnit);
TimeUnit * TimeUnitSemanticAction(TimeUnitType type);


#endif
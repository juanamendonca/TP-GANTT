#ifndef BISON_ACTIONS_HEADER
#define BISON_ACTIONS_HEADER

#include "../../shared/CompilerState.h"
#include "../../shared/Logger.h"
#include "../../shared/Type.h"
#include "AbstractSyntaxTree.h"
#include "SyntacticAnalyzer.h"
#include <stdlib.h>

/** Initialize module's internal state. */
void initializeBisonActionsModule();

/** Shutdown module's internal state. */
void shutdownBisonActionsModule();

/**
 * Bison semantic actions.
 */

TaskOptionals * sevenTaskOptionalsSemanticAction(char * id1, int points, char * id2, char * id3, TaskOptionDependsOn * taskOptionDependsOn, TaskOptionalsType type);
TaskOptionals * sixTaskOptionalsSemanticAction(int points, char * id1, char * id2, TaskOptionDependsOn * taskOptionDependsOn, TaskOptionalsType type);
TaskOptionals * fiveTaskOptionalsSemanticAction(char * id1, char * id2, char * id3, TaskOptionDependsOn * taskOptionDependsOn, TaskOptionalsType type);
TaskOptionals * fourTaskOptionalsSemanticAction(char * id, int points, TaskOptionalsType type);
TaskOptionals * threeTaskOptionalsSemanticAction(char * id1, char * id2, TaskOptionDependsOn * taskOptionDependsOn, TaskOptionalsType type);
TaskOptionals * twoTaskOptionalsSemanticAction(int points, TaskOptionalsType type);
TaskOptionals * oneTaskOptionalsSemanticAction(char * id, TaskOptionalsType type);
TaskLengthFormat * intervalLengthFormatSemanticAction(char * interval);
TaskLengthFormat * dateLengthFormatSemanticAction(char * startDate, char * finishDate);
TaskStructure * optionalsStructureSemanticAction(char * id, char * name, TaskLengthFormat * taskLengthFormat, TaskOptionals * taskOptionals);
TaskStructure * lengthFormatStructureSemanticAction(char * id, char * name, TaskLengthFormat * taskLengthFormat);
TaskList * structureRecursiveListSemanticAction(TaskList * recursiveTaskList, TaskStructure * taskStructure);
TaskList * structureListSemanticAction(TaskStructure * taskStructure);
ProjectBody * optionalsProjectBodySemanticAction(ProjectBodyOptionals * projectBodyOptionals, TaskList * taskList);
ProjectBody * taskListProjectBodySemanticAction(TaskList * taskList);
ProjectUnion * IdProjectUnionSemanticAction(ProjectUnion * recursiveProjectUnion, char * id);
ProjectOptionals * BothOptionalsSemanticAction(char * id1, char * id2, ProjectUnion * projectUnion);
ProjectOptionals * ProjectUnionOptionalsSemanticAction(char * id, ProjectUnion * projectUnion);
ProjectOptionals * IdOptionalsSemanticAction(char * id);
ProjectStructure * OptionalsProjectStructureSemanticAction(char * id, char * name, ProjectOptionals * projectOptionals, ProjectBody * projectBody);
ProjectStructure * ProjectBodyProjectStructureSemanticAction(char * id, char * name, ProjectBody * projectBody);
Program * ProjectStructureRecursiveProgramSemanticAction(Program * recursiveProgram, ProjectStructure * projectStructure);
Program * ProjectStructureProgramSemanticAction(CompilerState * compilerState, ProjectStructure * projectStructure);
BodyCategoriesOption * RecursiveCategoriesOptionSemanticAction(char * id, char * name);




TaskOptionDependsOn * TaskOptionDependsOnSemanticAction(char * id, char * id2);

ProjectBodyOptionals * OneBodyOptionalsSemanticAction(int maxTasks);
ProjectBodyOptionals * TwoBodyOptionalsSemanticAction( char * id, char * name, BodyCategoriesOption * bodyCategoriesOption);
ProjectBodyOptionals * ThreeBodyOptionalsSemanticAction(int maxPoints);
ProjectBodyOptionals * FourBodyOptionalsSemanticAction(char * startDate);
ProjectBodyOptionals * FiveBodyOptionalsSemanticAction(int maxTasks,  char * id, char * name, BodyCategoriesOption * bodyCategoriesOption);
ProjectBodyOptionals * SixBodyOptionalsSemanticAction(int maxTasks, int maxPoints);
ProjectBodyOptionals * SevenBodyOptionalsSemanticAction(int maxTasks,char * startDate);
ProjectBodyOptionals * EightBodyOptionalsSemanticAction(char * id, char * name, BodyCategoriesOption * bodyCategoriesOption, int maxPoints);
ProjectBodyOptionals * NineBodyOptionalsSemanticAction( char * id, char * name, BodyCategoriesOption * bodyCategoriesOption, char * startDate);
ProjectBodyOptionals * TenBodyOptionalsSemanticAction( int maxPoints, 	char * startDate );
ProjectBodyOptionals * ElevenBodyOptionalsSemanticAction(int maxTasks,char * id, char * name, BodyCategoriesOption * bodyCategoriesOption, int maxPoints);
ProjectBodyOptionals * TwelveBodyOptionalsSemanticAction(int maxTasks, int maxPoints, char * startDate );
ProjectBodyOptionals * ThirteenBodyOptionalsSemanticAction(char * id, char * name, BodyCategoriesOption * bodyCategoriesOption, int maxPoints, 	char * startDate);
ProjectBodyOptionals * FourteenBodyOptionalsSemanticAction(int maxTasks,  char * id, char * name, BodyCategoriesOption * bodyCategoriesOption, int maxPoints, 	char * startDate);
ProjectBodyOptionals * FifthteenBodyOptionalsSemanticAction(int maxTasks,  char * id, char * name, BodyCategoriesOption * bodyCategoriesOption,char * startDate);


#endif

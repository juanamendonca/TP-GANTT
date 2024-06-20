#ifndef TYPE_CHECKER_H
#define TYPE_CHECKER_H

#include "../frontend/syntactic-analysis/AbstractSyntaxTree.h"
#include <stdbool.h>
extern struct Project * projects; //Symbol table

typedef enum {
    PROJECT_T,
    TASK_T,
    CATEGORY_T,
    DATE_T,
    TIMEUNIT_T,
    INTEGER_T,
    INTERVAL_T,
    BOTTOM
} Type;

bool validateMaxTasks(ProjectBodyOptionals *optionals, struct Project *projectData);
bool validateMaxPoints(ProjectBodyOptionals *optionals, struct Project *projectData);
bool validateProjectReference(ProjectOptionals *projectOptionals);

// Declaraciones de funciones de type checking
bool typecheckProgram(Program *program);
Type typecheckProjectStructure(ProjectStructure *projectStructure);
Type typecheckProjectBody(ProjectBody *projectBody, struct Project *projectData);
Type typecheckProjectBodyOptionals(ProjectBodyOptionals *optionals, struct Project *projectData, Type projectType);
Type typecheckTaskList(TaskList *taskList, struct Project *projectData);
Type typecheckTaskStructure(TaskStructure *taskStructure, struct Project *projectData);
Type typecheckTaskLengthFormat(TaskLengthFormat *taskLengthFormat, TimeUnitType timeUnit);
Type typecheckTaskOptionals(TaskOptionals *taskOptionals, struct Project *projectData);
Type typecheckTaskOptionDependsOn(DependsOnId *dependsOn, struct Project *projectData);
Type typecheckProjectOptionals(ProjectOptionals *optionals);
Type typecheckProjectUnion(ProjectUnion *projectUnion);

// Función para reportar errores, se podría usar el logger
void reportError(const char *message, ...);

#endif // TYPE_CHECKER_H

#include "TypeChecker.h"
#include <stdio.h>
#include <stdarg.h>
#include <string.h>


// Validation of the entire program
bool typecheckProgram(Program *program) {
    if (program == NULL) {
        reportError("Error: Empty program.");
        exit(EXIT_FAILURE);
        return false;
    }

    if (program->type == SINGLE) {
        return typecheckProjectStructure(program->projectStructure) != BOTTOM;
    } else {
        return typecheckProgram(program->program) && typecheckProjectStructure(program->projectStructure1) != BOTTOM;
    }
}

// Validation of the project structure
Type typecheckProjectStructure(ProjectStructure *projectStructure) {
    if (projectStructure == NULL) {
        reportError("Error: Empty project structure.");
        exit(EXIT_FAILURE);
        return BOTTOM;
    }

    struct Project *projectData;
    HASH_FIND_STR(projects, projectStructure->projectStructureCommon->projectId->id, projectData);
    if (!projectData) {
        reportError("Error: Project '%s' not found.", projectStructure->projectStructureCommon->projectId->id);
        exit(EXIT_FAILURE);
        return BOTTOM;
    }

    //  Validation of the project body
    Type bodyType = typecheckProjectBody(projectStructure->projectBody, projectData);

   // Validation of the  project optionals
    Type optionalType = typecheckProjectOptionals(projectStructure->projectOptionals);

    // The project type is valid if both the body and the optionals are valid
    return (bodyType != BOTTOM && optionalType != BOTTOM) ? PROJECT_T : BOTTOM;
}

    //  Validation of the project body
Type typecheckProjectBody(ProjectBody *projectBody, struct Project *projectData) {
    if (projectBody == NULL) {
        reportError("Error: Empty body project.");
        exit(EXIT_FAILURE);
        return BOTTOM;
    }

    Type optionalsType = typecheckProjectBodyOptionals(projectBody->projectBodyOptionals, projectData, PROJECT_T);
    Type taskListType = typecheckTaskList(projectBody->taskList, projectData);

// The project body is valid if both the optionals and the task list are valid
    return (optionalsType != BOTTOM && taskListType != BOTTOM) ? PROJECT_T : BOTTOM;
}

// Validation of the project body optionals
Type typecheckProjectBodyOptionals(ProjectBodyOptionals *optionals, struct Project *projectData, Type projectType) {
    if (optionals == NULL) {
        return projectType; // If there are not optionals, the type remains the same.
    }

    bool success = true;

    if (optionals->maxTasks != NULL) {
        success = validateMaxTasks(optionals, projectData);
    }
    if (optionals->maxPoints != NULL) {
        success = validateMaxPoints(optionals, projectData);
    }
    if (optionals->categoriesId != NULL) {
        struct Category *category;
        // Search the category in the hash table of an specific project.
        HASH_FIND_STR(projectData->categories, optionals->categoriesId->id, category);
        if (category == NULL) {
            reportError("Error: Category '%s' not defined.", optionals->categoriesId->id);
            success = false;
            exit(EXIT_FAILURE);
        }
    }

    return success ? projectType : BOTTOM;
}

// Task list validation
Type typecheckTaskList(TaskList *taskList, struct Project *projectData) {
    if (taskList == NULL) {
        return BOTTOM; 
    }

    if (taskList->type == SINGLE) {
        return typecheckTaskStructure(taskList->taskStructure1, projectData);
    } else {
        Type taskListType = typecheckTaskList(taskList->taskList, projectData);
        Type taskType = typecheckTaskStructure(taskList->taskStructure2, projectData);
        return (taskListType != BOTTOM && taskType != BOTTOM) ? TASK_T : BOTTOM;
    }
}

// Validation of task structure
Type typecheckTaskStructure(TaskStructure *taskStructure, struct Project *projectData) {
    if (taskStructure == NULL) {
        reportError("Error: Task structure empty.");
        exit(EXIT_FAILURE);
        return BOTTOM;
    }

    struct Task *taskData;
    HASH_FIND_STR(projectData->tasks, taskStructure->taskId->id, taskData);
    if (!taskData) {
        reportError("Error: Task '%s' not found.", taskStructure->taskId->id);
        exit(EXIT_FAILURE);
        return BOTTOM;
    }

    Type lengthFormatType = typecheckTaskLengthFormat(taskStructure->taskLengthFormat, projectData->format);
    Type optionalsType = typecheckTaskOptionals(taskStructure->taskOptionals, projectData, taskData);

    return (lengthFormatType != BOTTOM && optionalsType != BOTTOM) ? TASK_T : BOTTOM;
}

// Validation of task duration format 
Type typecheckTaskLengthFormat(TaskLengthFormat *taskLengthFormat, TimeUnitType timeUnit) {
    if (taskLengthFormat == NULL) {
        reportError("Error: Format not specified.");
        exit(EXIT_FAILURE);
        return BOTTOM;
    }

    if (taskLengthFormat->type == TIME_PERIOD && strcmp(taskLengthFormat->startDate,taskLengthFormat->finishDate) > 0 ) {
        reportError("Error: The start date is later than the end date.");
        exit(EXIT_FAILURE);
    } else if (taskLengthFormat->type == DURATION && taskLengthFormat->rightInterval >= 0 && taskLengthFormat->leftInterval > taskLengthFormat->rightInterval)   {
        reportError("Error: The left interval bound is greater than the right interval bound.");
        exit(EXIT_FAILURE);
    }
    return INTERVAL_T;
}
// Validation of task optionals format 
Type typecheckTaskOptionals(TaskOptionals *taskOptionals, struct Project *projectData, struct Task *taskData) {
    if (taskOptionals == NULL) {
        return TASK_T;
    }

    bool success = true;

    if (taskOptionals->dependsOnId != NULL) {
        Type dependsOnType = typecheckDependsOnId(taskOptionals->dependsOnId, projectData, taskData);
        if (dependsOnType == BOTTOM) {
            success = false;
        }
    }
    if (taskOptionals->pointsInteger != NULL) {
        if (taskOptionals->pointsInteger->points < 0) {
            reportError("Error: Task points cannot be negative.");
            exit(EXIT_FAILURE);
            success = false;
        }
    }
    if (taskOptionals->categoryId != NULL) {
        struct Category *category;
        HASH_FIND_STR(projectData->categories, taskOptionals->categoryId->id, category);
        if (category == NULL) {
            reportError("Error: Category '%s' not defined.", taskOptionals->categoryId->id);
            exit(EXIT_FAILURE);
            success = false;
        }
    }
    return success ? TASK_T : BOTTOM;
}

// Validation of task dependencies
Type typecheckDependsOnId(DependsOnId *dependsOn, struct Project *projectData, struct Task *taskData) {
    if (dependsOn == NULL) {
        return TASK_T;
    }
    bool success = true;

    struct Project *project;
    HASH_FIND_STR(projects, dependsOn->id1, project);
    if (project == NULL) {
        reportError("Error: Project  '%s' has noot been defined.", dependsOn->id1);
        success = false;
        exit(EXIT_FAILURE);
    }

    // Verify that the projects have the same format
    if(project->format != projectData->format){
        reportError("Error: The Project '%s' does not have the same format as the project '%s'.", dependsOn->id1, projectData->projectId);
        success = false;
        exit(EXIT_FAILURE);
    }

    // Verify that the task does not depend on itself
    if(strcmp(dependsOn->id2, taskData->taskId) == 0 && strcmp(dependsOn->id1, projectData->projectId) == 0){
        reportError("Error: The task '%s' depends on itself", taskData->taskId);
        success = false;
        exit(EXIT_FAILURE);
    }

    // Verify that the referenced task exists in the project
    struct Task *task;
    HASH_FIND_STR(project->tasks, dependsOn->id2, task);
    if (task == NULL) {
        reportError("Error: Task '%s' not found in project '%s'.", dependsOn->id2, dependsOn->id1);
        success = false;
        exit(EXIT_FAILURE);
    }

    // Verify that the task starts after the completion of the task it depends on
    if(projectData->format == DATE_TYPE){
        if(strcmp(taskData->start, task->finish) < 0){
        reportError("Error: Task '%s' starts before task '%s' (which it depends on) ends.", taskData->taskId, dependsOn->id2);
        success = false;
        exit(EXIT_FAILURE);
        }
    }

    if (dependsOn->taskOptionDependsOn != NULL) {
        Type nextDependsOnType = typecheckTaskOptionDependsOn(dependsOn->taskOptionDependsOn, projectData, taskData);
        if (nextDependsOnType == BOTTOM) {
            success = false;
        }
    }
    return success ? TASK_T : BOTTOM;
}

// Validation of task dependencies
Type typecheckTaskOptionDependsOn(TaskOptionDependsOn *taskOptionDependsOn, struct Project *projectData, struct Task *taskData) {
    if (taskOptionDependsOn == NULL) {
        return TASK_T;
    }
    bool success = true;

    // Verify that the referenced project exists in the program
    struct Project *project;
    HASH_FIND_STR(projects, taskOptionDependsOn->id, project);
    if (project == NULL) {
        reportError("Error: Project '%s' has not been defined.", taskOptionDependsOn->id);
        success = false;
        exit(EXIT_FAILURE);
    }

    // Verify that the projects have the same format
    if(project->format != projectData->format){
        reportError("Error: The Project '%s' does not have the same format as the project '%s'.", taskOptionDependsOn->id, projectData->projectId);
        success = false;
        exit(EXIT_FAILURE);
    }

    // Verify that the task does not depend on itself
    if(strcmp(taskOptionDependsOn->id2, taskData->taskId) == 0 && strcmp(taskOptionDependsOn->id, projectData->projectId) == 0){
        reportError("Error: The task '%s' depends on itself", taskData->taskId);
        success = false;
        exit(EXIT_FAILURE);
    }

    // Verify that the referenced task exists in the project
    struct Task *task;
    HASH_FIND_STR(project->tasks, taskOptionDependsOn->id2, task);
    if (task == NULL) {
        reportError("Error: Task '%s' not found in project '%s'.", taskOptionDependsOn->id2, taskOptionDependsOn->id);
        success = false;
        exit(EXIT_FAILURE);
    }

    // Verify that the task starts after the completion of the task it depends on
    if(projectData->format == DATE_TYPE){
        if(strcmp(taskData->start, task->finish) < 0){
        reportError("Error: Task '%s' starts before task '%s' (which it depends on) ends.", taskData->taskId, taskOptionDependsOn->id2);
        success = false;
        exit(EXIT_FAILURE);
        }
    }

    if (taskOptionDependsOn->taskOptionDependsOn != NULL) {
        Type nextDependsOnType = typecheckTaskOptionDependsOn(taskOptionDependsOn->taskOptionDependsOn, projectData, taskData);
        if (nextDependsOnType == BOTTOM) {
            success = false;
        }
    }
    return success ? TASK_T : BOTTOM;
}

// Validation of project optionals
Type typecheckProjectOptionals(ProjectOptionals *optionals) {
    if (optionals == NULL) {
        return PROJECT_T;
    }

    bool success = true;
    switch (optionals->type) {
        case DEPENDS_ON_P:
            success = validateProjectReference(optionals);
            break;
        case WITH_P:
            if (typecheckProjectUnion(optionals->projectUnion1) == BOTTOM) {
                success = false;
            }
            break;
        case BOTH:
            success = validateProjectReference(optionals) && typecheckProjectUnion(optionals->projectUnion2) != BOTTOM;
            break;
    }
    return success ? PROJECT_T : BOTTOM;
}

// Validation of project union
Type typecheckProjectUnion(ProjectUnion *projectUnion) {
    if (projectUnion == NULL) {
        return PROJECT_T;
    }

    // Verify that the referenced project exists
    struct Project *project;
    HASH_FIND_STR(projects, projectUnion->id, project);
    if (!project) {
        reportError("Error: Project '%s' not found.", projectUnion->id);
        exit(EXIT_FAILURE);
        return BOTTOM;
    }

    // Recursively verify other projects in the union
    return typecheckProjectUnion(projectUnion->projectUnion);
}

// Validate maximum number of tasks in a project
bool validateMaxTasks(ProjectBodyOptionals *optionals, struct Project *projectData) {
    TaskList *taskList;
    int taskCount = HASH_COUNT(projectData->tasks);

    if (taskCount > optionals->maxTasks->tasks) {
        reportError("Error: The maximum number of tasks allowed in project '%s' is exceeded.", projectData->projectId);
        exit(EXIT_FAILURE);
        return false;
    }
    return true;
}

// Validate maximum points in a project
bool validateMaxPoints(ProjectBodyOptionals *optionals, struct Project *projectData) {
    int totalPoints = 0;
    struct Task *task;

    // Iterate over the task hash table
    for (task = projectData->tasks; task != NULL; task = (struct Task*)(task->hh.next)) {
        totalPoints += task->points;
    }

    if (totalPoints > optionals->maxPoints->points) {
        reportError("Error: The maximum points allowed in project '%s' is exceeded.", projectData->projectId);
        exit(EXIT_FAILURE);
        return false;
    }
    return true;
}

// Validate project reference
bool validateProjectReference(ProjectOptionals *projectOptionals) {
    struct Project *referencedProject;
    bool success = true;

    if (projectOptionals != NULL) {
        switch (projectOptionals->type) {
            case DEPENDS_ON_P:
                HASH_FIND_STR(projects, projectOptionals->id1, referencedProject);
                if (referencedProject == NULL) {
                    reportError("Error: Project '%s' not found.", projectOptionals->id1);
                    success = false;
                    exit(EXIT_FAILURE);

                }
                break;
            case WITH_P:
                // No validation for WITH_P as it's a project union validated elsewhere
                break;
            case BOTH:
                HASH_FIND_STR(projects, projectOptionals->id3, referencedProject);
                if (referencedProject == NULL) {
                    reportError("Error: Project '%s' not found.", projectOptionals->id3);
                    success = false;
                    exit(EXIT_FAILURE);

                }
                break;
            default: // EMPTY
                break;
        }
    }
    return success;
}

// Function to report errors
void reportError(const char *message, ...) {
    va_list args;
    va_start(args, message);
    vfprintf(stderr, message, args);
    va_end(args);
    fprintf(stderr, "\n");
}
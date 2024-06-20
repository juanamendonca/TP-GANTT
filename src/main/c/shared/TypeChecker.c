#include "TypeChecker.h"
#include <stdio.h>
#include <stdarg.h>
#include <string.h>


// Validación del programa completo
bool typecheckProgram(Program *program) {
    if (program == NULL) {
        reportError("Error: Programa vacío.");
        exit(EXIT_FAILURE);
        return false;
    }

    if (program->type == SINGLE) {
        return typecheckProjectStructure(program->projectStructure) != BOTTOM;
    } else {
        return typecheckProgram(program->program) && typecheckProjectStructure(program->projectStructure1) != BOTTOM;
    }
}

// Validación de la estructura de un proyecto
Type typecheckProjectStructure(ProjectStructure *projectStructure) {
    if (projectStructure == NULL) {
        reportError("Error: Estructura de proyecto vacía.");
        exit(EXIT_FAILURE);
        return BOTTOM;
    }

    struct Project *projectData;
    HASH_FIND_STR(projects, projectStructure->projectStructureCommon->projectId->id, projectData);
    if (!projectData) {
        reportError("Error: Proyecto '%s' no encontrado.", projectStructure->projectStructureCommon->projectId->id);
        exit(EXIT_FAILURE);
        return BOTTOM;
    }

    // Validación del cuerpo del proyecto
    Type bodyType = typecheckProjectBody(projectStructure->projectBody, projectData);

    // Validación de los opcionales del proyecto
    Type optionalType = typecheckProjectOptionals(projectStructure->projectOptionals);

    // El tipo del proyecto es válido si tanto el cuerpo como los opcionales son válidos
    return (bodyType != BOTTOM && optionalType != BOTTOM) ? PROJECT_T : BOTTOM;
}

// Validación del cuerpo de un proyecto
Type typecheckProjectBody(ProjectBody *projectBody, struct Project *projectData) {
    if (projectBody == NULL) {
        reportError("Error: Cuerpo del proyecto vacío.");
        exit(EXIT_FAILURE);
        return BOTTOM;
    }

    Type optionalsType = typecheckProjectBodyOptionals(projectBody->projectBodyOptionals, projectData, PROJECT_T);
    Type taskListType = typecheckTaskList(projectBody->taskList, projectData);

    // El cuerpo del proyecto es válido si tanto los opcionales como la lista de tareas son válidos
    return (optionalsType != BOTTOM && taskListType != BOTTOM) ? PROJECT_T : BOTTOM;
}

// Validación de los opcionales del cuerpo de un proyecto
Type typecheckProjectBodyOptionals(ProjectBodyOptionals *optionals, struct Project *projectData, Type projectType) {
    if (optionals == NULL) {
        return projectType; // Si no hay opcionales, el tipo del proyecto no cambia
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
        // Buscar la categoría en la tabla hash de categorías del proyecto
        HASH_FIND_STR(projectData->categories, optionals->categoriesId->id, category);
        if (category == NULL) {
            reportError("Error: Categoría '%s' no definida.", optionals->categoriesId->id);
            success = false;
            exit(EXIT_FAILURE);
        }
    }

/*     if (optionals->projectStart != NULL) {
        // ... (Validación de formato de fecha)
    } */

    return success ? projectType : BOTTOM;
}

// Validación de la lista de tareas
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

// Validación de la estructura de una tarea
Type typecheckTaskStructure(TaskStructure *taskStructure, struct Project *projectData) {
    if (taskStructure == NULL) {
        reportError("Error: Estructura de tarea vacía.");
        exit(EXIT_FAILURE);
        return BOTTOM;
    }

    struct Task *taskData;
    HASH_FIND_STR(projectData->tasks, taskStructure->taskId->id, taskData);
    if (!taskData) {
        reportError("Error: Tarea '%s' no encontrada.", taskStructure->taskId->id);
        exit(EXIT_FAILURE);
        return BOTTOM;
    }

    Type lengthFormatType = typecheckTaskLengthFormat(taskStructure->taskLengthFormat, projectData->format);
    Type optionalsType = typecheckTaskOptionals(taskStructure->taskOptionals, projectData, taskData);

    return (lengthFormatType != BOTTOM && optionalsType != BOTTOM) ? TASK_T : BOTTOM;
}

// Validación del formato de duración de una tarea
Type typecheckTaskLengthFormat(TaskLengthFormat *taskLengthFormat, TimeUnitType timeUnit) {
    if (taskLengthFormat == NULL) {
        reportError("Error: Formato de duración de tarea no especificado.");
        exit(EXIT_FAILURE);
        return BOTTOM;
    }

    if (taskLengthFormat->type == TIME_PERIOD && strcmp(taskLengthFormat->startDate,taskLengthFormat->finishDate) > 0 ) {
        reportError("Error: La fecha de comienzo es mayor que la fecha de fin.");
        exit(EXIT_FAILURE);
    } else if (taskLengthFormat->type == DURATION && taskLengthFormat->rightInterval >= 0 && taskLengthFormat->leftInterval > taskLengthFormat->rightInterval)   {
        reportError("Error: El extremo izquierdo del intervalo es mayor que el extremo derecho");
        exit(EXIT_FAILURE);
    }
    return INTERVAL_T;
}

// Validación de los opcionales de una tarea
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
            reportError("Error: Los puntos de la tarea no pueden ser negativos.");
            exit(EXIT_FAILURE);
            success = false;
        }
    }
    if (taskOptionals->categoryId != NULL) {
        struct Category *category;
        HASH_FIND_STR(projectData->categories, taskOptionals->categoryId->id, category);
        if (category == NULL) {
            reportError("Error: Categoría '%s' no definida.", taskOptionals->categoryId->id);
            exit(EXIT_FAILURE);
            success = false;
        }
    }
    return success ? TASK_T : BOTTOM;
}

// Validación de las dependencias de una tarea
Type typecheckDependsOnId(DependsOnId *dependsOn, struct Project *projectData, struct Task *taskData) {
    if (dependsOn == NULL) {
        return TASK_T;
    }
    bool success = true;

    // Verificar que el proyecto referenciado existe en el programa
    struct Project *project;
    HASH_FIND_STR(projects, dependsOn->id1, project);
    if (project == NULL) {
        reportError("Error: El Proyecto '%s' no ha sido definido.", dependsOn->id1);
        success = false;
        exit(EXIT_FAILURE);
    }

    //Verificar que los proyectos tengan el mismo formato
    if(project->format != projectData->format){
        reportError("Error: El Proyecto '%s' no tiene el mismo formato que el proyecto '%s'.", dependsOn->id1, projectData->projectId);
        success = false;
        exit(EXIT_FAILURE);
    }

    //verificar que la tarea no depende de si misma
    if(strcmp(dependsOn->id2, taskData->taskId) == 0 && strcmp(dependsOn->id1, projectData->projectId) == 0){
        reportError("Error: La tarea '%s' depende de si misma", taskData->taskId);
        success = false;
        exit(EXIT_FAILURE);
    }

    // Verificar que la tarea referenciada existe en el proyecto
    struct Task *task;
    HASH_FIND_STR(project->tasks, dependsOn->id2, task);
    if (task == NULL) {
        reportError("Error: Tarea '%s' no encontrada en el proyecto '%s'.", dependsOn->id2, dependsOn->id1);
        success = false;
        exit(EXIT_FAILURE);
    }

    //Verificar que la tarea empieza despues de la finalizacion de la tarea de la cual depende
    if(projectData->format == DATE_TYPE){
        if(strcmp(taskData->start, task->finish) < 0){
        reportError("Error: La tarea '%s' comienza antes que termine la tarea '%s' (de la cual depende)", taskData->taskId, dependsOn->id2);
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

// Validación de las dependencias de una tarea
Type typecheckTaskOptionDependsOn(TaskOptionDependsOn *taskOptionDependsOn, struct Project *projectData, struct Task *taskData) {
    if (taskOptionDependsOn == NULL) {
        return TASK_T;
    }
    bool success = true;

    // Verificar que el proyecto referenciado existe en el programa
    struct Project *project;
    HASH_FIND_STR(projects, taskOptionDependsOn->id, project);
    if (project == NULL) {
        reportError("Error: El Proyecto '%s' no ha sido definido.", taskOptionDependsOn->id);
        success = false;
        exit(EXIT_FAILURE);
    }

    //Verificar que los proyectos tengan el mismo formato
    if(project->format != projectData->format){
        reportError("Error: El Proyecto '%s' no tiene el mismo formato que el proyecto '%s", taskOptionDependsOn->id, projectData->projectId);
        success = false;
        exit(EXIT_FAILURE);
    }

    //verificar que la tarea no depende de si misma
    if(strcmp(taskOptionDependsOn->id2, taskData->taskId) == 0 && strcmp(taskOptionDependsOn->id, projectData->projectId) == 0){
        reportError("Error: La tarea '%s' depende de si misma", taskData->taskId);
        success = false;
        exit(EXIT_FAILURE);
    }

    // Verificar que la tarea referenciada existe en el proyecto
    struct Task *task;
    HASH_FIND_STR(project->tasks, taskOptionDependsOn->id2, task);
    if (task == NULL) {
        reportError("Error: Tarea '%s' no encontrada en el proyecto '%s'.", taskOptionDependsOn->id2, taskOptionDependsOn->id);
        success = false;
        exit(EXIT_FAILURE);
    }

    //Verificar que la tarea empieza despues de la finalizacion de la tarea de la cual depende
    if(projectData->format == DATE_TYPE){
        if(strcmp(taskData->start, task->finish) < 0){
        reportError("Error: La tarea '%s' comienza antes que la tarea '%s' (de la cual depende)", taskData->taskId, taskOptionDependsOn->id2);
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

// Validación de los opcionales de un proyecto
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

// Validación de la unión de proyectos
Type typecheckProjectUnion(ProjectUnion *projectUnion) {
    if (projectUnion == NULL) {
        return PROJECT_T;
    }

    // Verificar que el proyecto referenciado existe
    struct Project *project;
    HASH_FIND_STR(projects, projectUnion->id, project);
    if (!project) {
        reportError("Error: Proyecto '%s' no encontrado.", projectUnion->id);
        exit(EXIT_FAILURE);
        return BOTTOM;
    }

    // Verificar recursivamente otros proyectos en la unión
    return typecheckProjectUnion(projectUnion->projectUnion);
}

// Validate maximum number of tasks in a project
bool validateMaxTasks(ProjectBodyOptionals *optionals, struct Project *projectData) {
    TaskList *taskList;
    int taskCount = HASH_COUNT(projectData->tasks);

    if (taskCount > optionals->maxTasks->tasks) {
        reportError("Error: Se excede la cantidad de tareas máximas permitidas en el proyecto '%s'.", projectData->projectId);
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
        reportError("Error: Se excede la cantidad de puntos máximos permitidos en el proyecto '%s'.", projectData->projectId);
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
                    reportError("Error: Proyecto '%s' no encontrado.", projectOptionals->id1);
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
                    reportError("Error: Proyecto '%s' no encontrado.", projectOptionals->id3);
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

// Función para reportar errores
void reportError(const char *message, ...) {
    va_list args;
    va_start(args, message);
    vfprintf(stderr, message, args);
    va_end(args);
    fprintf(stderr, "\n");
}

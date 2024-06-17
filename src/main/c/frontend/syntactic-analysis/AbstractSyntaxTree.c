#include "AbstractSyntaxTree.h"

/* MODULE INTERNAL STATE */

static Logger * _logger = NULL;

void initializeAbstractSyntaxTreeModule() {
    _logger = createLogger("AbstractSyntaxTree");
}

void shutdownAbstractSyntaxTreeModule() {
    if (_logger != NULL) {
        destroyLogger(_logger);
    }
}

/** PUBLIC FUNCTIONS */

void releaseBodyCategoriesOption(BodyCategoriesOption * bodyCategoriesOption) {
    logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
    if (bodyCategoriesOption != NULL) {
        free(bodyCategoriesOption->id);
        free(bodyCategoriesOption->name);
        releaseBodyCategoriesOption(bodyCategoriesOption->bodyCategoriesOption);
        free(bodyCategoriesOption);
    }
}

void releaseProjectStart(ProjectStart * projectStart) {
    logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
    if (projectStart != NULL) {
        free(projectStart->date);
        free(projectStart);
    }
}

void releaseMaxPoints(MaxPoints * maxPoints) {
    logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
    if (maxPoints != NULL) {
        free(maxPoints);
    }
}

void releaseCategoriesId(CategoriesId * categoriesId) {
    logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
    if (categoriesId != NULL) {
        free(categoriesId->id);
        free(categoriesId->name);
        releaseBodyCategoriesOption(categoriesId->bodyCategoriesOption);
        free(categoriesId);
    }
}

void releaseMaxTasks(MaxTasks * maxTasks) {
    logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
    if (maxTasks != NULL) {
        free(maxTasks);
    }
}

void releaseProjectBodyOptionals(ProjectBodyOptionals * projectBodyOptionals) {
    logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
    if (projectBodyOptionals != NULL) {
        releaseMaxTasks(projectBodyOptionals->maxTasks);
        releaseCategoriesId(projectBodyOptionals->categoriesId);
        releaseMaxPoints(projectBodyOptionals->maxPoints);
        releaseProjectStart(projectBodyOptionals->projectStart);
        free(projectBodyOptionals);
    }
}

void releaseTaskOptionDependsOn(TaskOptionDependsOn * taskOptionDependsOn) {
    logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
    if (taskOptionDependsOn != NULL) {
        free(taskOptionDependsOn->id);
        free(taskOptionDependsOn->id2);
        releaseTaskOptionDependsOn(taskOptionDependsOn->taskOptionDependsOn);
        free(taskOptionDependsOn);
    }
}

void releaseDependsOnId(DependsOnId * dependsOnId) {
    logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
    if (dependsOnId != NULL) {
        free(dependsOnId->id1);
        free(dependsOnId->id2);
        releaseTaskOptionDependsOn(dependsOnId->taskOptionDependsOn);
        free(dependsOnId);
    }
}

void releasePointsInteger(PointsInteger * pointsInteger) {
    logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
    if (pointsInteger != NULL) {
        free(pointsInteger);
    }
}

void releaseCategoryId(CategoryId * categoryId) {
    logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
    if (categoryId != NULL) {
        free(categoryId->id);
        free(categoryId);
    }
}

void releaseUnique(Unique * unique) {
    logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
    if (unique != NULL) {
        free(unique);
    }
}

void releaseTaskOptionals(TaskOptionals * taskOptionals) {
    logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
    if (taskOptionals != NULL) {
        releaseCategoryId(taskOptionals->categoryId);
        releasePointsInteger(taskOptionals->pointsInteger);
        releaseDependsOnId(taskOptionals->dependsOnId);
        releaseUnique(taskOptionals->unique);
        free(taskOptionals);
    }
}

void releaseTaskLengthFormat(TaskLengthFormat * taskLengthFormat) {
    logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
    if (taskLengthFormat != NULL) {
        if (taskLengthFormat->type == TIME_PERIOD) {
            free(taskLengthFormat->startDate);
            free(taskLengthFormat->finishDate);
        }
        free(taskLengthFormat);
    }
}

void releaseTaskId(TaskId * taskId){
    logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
    if (taskId != NULL){
        free(taskId->id);
        free(taskId);
    }
}

void releaseTaskStructure(TaskStructure * taskStructure) {
    logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
    if (taskStructure != NULL) {
        releaseTaskId(taskStructure->taskId);
        free(taskStructure->name);
        releaseTaskLengthFormat(taskStructure->taskLengthFormat);
        releaseTaskOptionals(taskStructure->taskOptionals);
        free(taskStructure);
    }
}

void releaseTaskList(TaskList * taskList) {
    logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
    if (taskList != NULL) {
        if (taskList->type == SINGLE) {
            releaseTaskStructure(taskList->taskStructure1);
        } else {
            releaseTaskList(taskList->taskList);
            releaseTaskStructure(taskList->taskStructure2);
        }
        free(taskList);
    }
}

void releaseProjectBody(ProjectBody * projectBody) {
    logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
    if (projectBody != NULL) {
        releaseProjectBodyOptionals(projectBody->projectBodyOptionals);
        releaseTaskList(projectBody->taskList);
        free(projectBody);
    }
}

void releaseProjectUnion(ProjectUnion * projectUnion) {
    logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
    if (projectUnion != NULL) {
        free(projectUnion->id);
        releaseProjectUnion(projectUnion->projectUnion);
        free(projectUnion);
    }
}

void releaseProjectOptionals(ProjectOptionals * projectOptionals) {
    logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
    if (projectOptionals != NULL) {
        switch (projectOptionals->type) {
            case DEPENDS_ON_P:
                free(projectOptionals->id1);
                break;
            case WITH_P:
                free(projectOptionals->id2);
                releaseProjectUnion(projectOptionals->projectUnion1);
                break;
            case BOTH:
                free(projectOptionals->id3);
                free(projectOptionals->id4);
                releaseProjectUnion(projectOptionals->projectUnion2);
                break;
            default:
                break;
        }
        free(projectOptionals);
    }
}

void releaseProjectId(ProjectId * projectId){
    logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
    if(projectId != NULL){
        free(projectId->id);
        free(projectId);
    }
}

void releaseProjectStructureCommon(ProjectStructureCommon * projectStructureCommon) {
    logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
    if (projectStructureCommon != NULL) {
        releaseProjectId(projectStructureCommon->projectId);
        free(projectStructureCommon->name);
        free(projectStructureCommon->timeUnit);
        free(projectStructureCommon);
    }
}

void releaseProjectStructure(ProjectStructure * projectStructure) {
    logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
    if (projectStructure != NULL) {
        releaseProjectStructureCommon(projectStructure->projectStructureCommon);
        releaseProjectOptionals(projectStructure->projectOptionals);
        releaseProjectBody(projectStructure->projectBody);
        free(projectStructure);
    }
}

void releaseProgram(Program * program) {
    logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
    if (program != NULL) {
        if (program->type == SINGLE) {
            releaseProjectStructure(program->projectStructure);
        } else {
            releaseProgram(program->program);
            releaseProjectStructure(program->projectStructure1);
        }
        free(program);
    }
}

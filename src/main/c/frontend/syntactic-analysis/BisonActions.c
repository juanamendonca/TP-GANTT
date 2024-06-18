#include "BisonActions.h"

/* MODULE INTERNAL STATE */

static Logger * _logger = NULL;
static char * currentProjectId = NULL;
static char * currentTaskId = NULL;
static int withCount = 0;
static int dependsCount = 0;
static int projectCount = 0;

void initializeBisonActionsModule() {
	_logger = createLogger("BisonActions");
}

void shutdownBisonActionsModule() {
	if (_logger != NULL) {
		destroyLogger(_logger);
	}
}

/** IMPORTED FUNCTIONS */

extern unsigned int flexCurrentContext(void);

/* PRIVATE FUNCTIONS */

static void _logSyntacticAnalyzerAction(const char * functionName);

/**
 * Logs a syntactic-analyzer acnewProjecttion in DEBUGGING level.
 */
static void _logSyntacticAnalyzerAction(const char * functionName) {
	logDebugging(_logger, "%s", functionName);
}

/* PUBLIC FUNCTIONS */

DependsOnId * DependsOnIdSemanticAction(char * id1, char * id2, TaskOptionDependsOn * taskOptionDependsOn){
	_logSyntacticAnalyzerAction(__FUNCTION__);
	DependsOnId * dependsOnId = calloc(1,sizeof(DependsOnId));
	dependsOnId->id1 = id1;
	dependsOnId->id2 = id2;
	dependsOnId->taskOptionDependsOn = taskOptionDependsOn;

	struct Project *project;
	HASH_FIND_STR(projects, currentProjectId, project);

	if(project != NULL){
		dependsCount = 0;
		struct Task *task;
		HASH_FIND_STR(projects->tasks, currentTaskId, task);

		if(task != NULL){
			task->depends_on[dependsCount++] = id2;
			task->depends_on[dependsCount] = NULL;
		}
	}


	return dependsOnId;
}

PointsInteger * PointsIntegerSemanticAction(int points){
	_logSyntacticAnalyzerAction(__FUNCTION__);
	PointsInteger * pointsInteger = calloc(1,sizeof(PointsInteger));
	pointsInteger->points = points;

	struct Project *project;
	HASH_FIND_STR(projects, currentProjectId, project);

	if(project != NULL){
		struct Task *task;
		HASH_FIND_STR(projects->tasks, currentTaskId, task);

		if(task != NULL){
			task->points = points;
		}
	}

	return pointsInteger;
}

CategoryId * CategoryIdSemanticAction(char * id){
	_logSyntacticAnalyzerAction(__FUNCTION__);
	CategoryId * categoryId = calloc(1,sizeof(CategoryId));
	categoryId->id = id;

	struct Project *project;
	HASH_FIND_STR(projects, currentProjectId, project);

	if(project != NULL){
		struct Task *task;
		HASH_FIND_STR(projects->tasks, currentTaskId, task);

		if(task != NULL){
			task->category = id;
		}
	}

	return categoryId;
}

TaskOptionals * TaskOptionalSemanticAction(CategoryId * categoryId, PointsInteger * pointsInteger, DependsOnId * dependsOnId, Unique * unique){
	_logSyntacticAnalyzerAction(__FUNCTION__);
	TaskOptionals * taskOptionals = calloc(1,sizeof(TaskOptionals));
	taskOptionals->categoryId = categoryId;
	taskOptionals->pointsInteger = pointsInteger;
	taskOptionals->dependsOnId = dependsOnId;
	taskOptionals->unique = unique;
	return taskOptionals;
}

TaskLengthFormat * IntervalLengthFormatSemanticAction(int leftInterval, int rightInterval){
	_logSyntacticAnalyzerAction(__FUNCTION__);
	TaskLengthFormat * taskLengthFormat = calloc(1,sizeof(TaskLengthFormat));
	taskLengthFormat->leftInterval = leftInterval;
	taskLengthFormat->rightInterval = rightInterval;
	taskLengthFormat->type = DURATION;

	
	struct Project *project;
	HASH_FIND_STR(projects, currentProjectId, project);

	if(project != NULL){
		struct Task *newTask;
		HASH_FIND_STR(project->tasks, currentTaskId, newTask);

		if(newTask == NULL){
			newTask = malloc(sizeof(struct Task));
			newTask->lengthStart = leftInterval;
			newTask->lengthFinish = rightInterval;
			newTask->base = 0;
			newTask->taskId = currentTaskId;
			// Agregar la tarea a la tabla hash
			HASH_ADD_KEYPTR(hh, project->tasks, currentTaskId, strlen(currentTaskId), newTask);
		}
		else{
			logError(_logger, "Same ID's task definition", flexCurrentContext());
		}
	}

	return taskLengthFormat;
}

TaskLengthFormat * DateLengthFormatSemanticAction(char * startDate, char * finishDate){
	_logSyntacticAnalyzerAction(__FUNCTION__);
	TaskLengthFormat * taskLengthFormat = calloc(1,sizeof(TaskLengthFormat));
	taskLengthFormat->startDate = startDate;
	taskLengthFormat->finishDate = finishDate;
	taskLengthFormat->type = TIME_PERIOD;

	struct Project *project;
	HASH_FIND_STR(projects, currentProjectId, project);

	if(project != NULL){
		struct Task *newTask;
		HASH_FIND_STR(project->tasks, currentTaskId, newTask);

		if(newTask == NULL){
			newTask = malloc(sizeof(struct Task));
			newTask->start = startDate;
			newTask->finish = finishDate;
			newTask->base = 0;
			newTask->taskId = currentTaskId;
			// Agregar la tarea a la tabla hash
			HASH_ADD_KEYPTR(hh, project->tasks, currentTaskId, strlen(currentTaskId), newTask);
		}
		else{
			logError(_logger, "Same ID's task definition", flexCurrentContext());
		}
	}

	return taskLengthFormat;
}

TaskId * taskIdSemanticAction(char * id){
	_logSyntacticAnalyzerAction(__FUNCTION__);
	TaskId * taskId = calloc(1,sizeof(TaskId));
	taskId->id = id;

	currentTaskId = id;

	return taskId;
}

TaskStructure * OptionalsStructureSemanticAction(TaskId * taskId, char * name, TaskLengthFormat * taskLengthFormat, TaskOptionals * taskOptionals){
	_logSyntacticAnalyzerAction(__FUNCTION__);
	TaskStructure * taskStructure = calloc(1,sizeof(TaskStructure));
	taskStructure->taskId = taskId;
	taskStructure->name = name;
	taskStructure->taskLengthFormat = taskLengthFormat;
	taskStructure->taskOptionals = taskOptionals;

	struct Project *project;
	HASH_FIND_STR(projects, currentProjectId, project);

	if(project != NULL){
		struct Task *task;
		HASH_FIND_STR(project->tasks, currentTaskId, task);

		if(task != NULL){
			task->name = name;
		}
	}

	return taskStructure;
}

TaskList * StructureRecursiveListSemanticAction(TaskList * recursiveTaskList, TaskStructure * taskStructure){
	_logSyntacticAnalyzerAction(__FUNCTION__);
	TaskList * taskList = calloc(1,sizeof(TaskList));
	taskList->taskList = recursiveTaskList;
	taskList->taskStructure2 = taskStructure;
	taskList->type = MULTIPLE;
	return taskList;
}

TaskList * StructureListSemanticAction(TaskStructure * taskStructure){
	_logSyntacticAnalyzerAction(__FUNCTION__);
	TaskList * taskList = calloc(1,sizeof(TaskList));
	taskList->taskStructure1 = taskStructure;
	taskList->type = SINGLE;
	return taskList;
}

ProjectBody * OptionalsProjectBodySemanticAction(ProjectBodyOptionals * projectBodyOptionals, TaskList * taskList){
	_logSyntacticAnalyzerAction(__FUNCTION__);
	ProjectBody * projectBody = calloc(1,sizeof(ProjectBody));
	projectBody->projectBodyOptionals = projectBodyOptionals;
	projectBody->taskList = taskList;
	return projectBody;
}

ProjectUnion * IdProjectUnionSemanticAction(ProjectUnion * recursiveProjectUnion, char * id){
	_logSyntacticAnalyzerAction(__FUNCTION__);
	ProjectUnion * projectUnion = calloc(1,sizeof(ProjectUnion));
	projectUnion->projectUnion = recursiveProjectUnion;
	projectUnion->id = id;

	struct Project *project;
	HASH_FIND_STR(projects, currentProjectId, project);

	if(project != NULL){
		if(withCount < MAX_LIST){
			project->with[withCount++] = id;
		}
	}


	return projectUnion;
}

ProjectOptionals * BothOptionalsSemanticAction(char * id1, char * id2, ProjectUnion * projectUnion){
	_logSyntacticAnalyzerAction(__FUNCTION__);
	ProjectOptionals * projectOptionals = calloc(1,sizeof(ProjectOptionals));
	projectOptionals->id1 = id1;
	projectOptionals->id1 = id2;
	projectOptionals->projectUnion2 = projectUnion;
	projectOptionals->type = BOTH;

	withCount = 0;
	struct Project *project;
	HASH_FIND_STR(projects, currentProjectId, project);

	if(project != NULL){
		project->depends_on = id1;
		if(withCount < MAX_LIST){
			project->with[withCount++] = id2;
		}
	}

	return projectOptionals;
}

ProjectOptionals * ProjectUnionOptionalsSemanticAction(char * id, ProjectUnion * projectUnion){
	_logSyntacticAnalyzerAction(__FUNCTION__);
	ProjectOptionals * projectOptionals = calloc(1,sizeof(ProjectOptionals));
	projectOptionals->id1 = id;
	projectOptionals->projectUnion1 = projectUnion;
	projectOptionals->type = WITH_P;

	withCount = 0;
	struct Project *project;
	HASH_FIND_STR(projects, currentProjectId, project);

	if(project != NULL){
		if(withCount < MAX_LIST){
			project->with[withCount++] = id;
		}
	}

	return projectOptionals;
}

ProjectOptionals * IdOptionalsSemanticAction(char * id){
	_logSyntacticAnalyzerAction(__FUNCTION__);
	ProjectOptionals * projectOptionals = calloc(1,sizeof(ProjectOptionals));
	projectOptionals->id1 = id;
	projectOptionals->type = DEPENDS_ON_P;

	struct Project *project;
	HASH_FIND_STR(projects, currentProjectId, project);

	if(project != NULL){
		project->depends_on = id;
	}

	return projectOptionals;
}

ProjectStructure * OptionalsProjectStructureSemanticAction(ProjectStructureCommon  * projectStructureCommon, ProjectOptionals * projectOptionals, ProjectBody * projectBody){
	_logSyntacticAnalyzerAction(__FUNCTION__);
	ProjectStructure * projectStructure = calloc(1,sizeof(ProjectStructure));
	projectStructure->projectStructureCommon = projectStructureCommon;
	projectStructure->projectBody = projectBody;
	projectStructure->projectOptionals = projectOptionals;
	
	return projectStructure;

}

ProjectStructureCommon * ProjectStructureCommonSemanticAction(ProjectId * projectId, char * name, TimeUnit * timeUnit){
	_logSyntacticAnalyzerAction(__FUNCTION__);
	ProjectStructureCommon * projectStructureCommon = calloc(1,sizeof(ProjectStructureCommon));
	projectStructureCommon->projectId = projectId;
	projectStructureCommon->name = name;
	projectStructureCommon->timeUnit = timeUnit;

	
    struct Project *project;
	HASH_FIND_STR(projects, currentProjectId, project);

	if(project != NULL){
		project->projectId = projectId->id;
    	project->name = name;
	}
    
	return projectStructureCommon;
}

ProjectId * projectIdSemanticAction(char * id){
	_logSyntacticAnalyzerAction(__FUNCTION__);
	ProjectId * projectId = calloc(1,sizeof(ProjectId));
	projectId->id = id;

	currentProjectId = id;

	return projectId;
}

TimeUnit * TimeUnitSemanticAction(TimeUnitType type){
	_logSyntacticAnalyzerAction(__FUNCTION__);
	TimeUnit * timeUnit = calloc(1,sizeof(TimeUnit));
	timeUnit->type = type;

	struct Project *newProject;
	HASH_FIND_STR(projects, currentProjectId, newProject);

	if(newProject == NULL){
		newProject = malloc(sizeof(struct Project));
		newProject->format = (int) type;
		newProject->tasks = NULL;
		newProject->categories = NULL;
		newProject->projectNumber = ++projectCount;
		newProject->baseFlag = 0;
		// Agregar el proyecto a la tabla hash
		HASH_ADD_KEYPTR(hh, projects, currentProjectId, strlen(currentProjectId), newProject);
	}
	else{
		logError(_logger, "Same ID's project definition", flexCurrentContext());
	}

	return timeUnit;
}

Program * ProjectStructureRecursiveProgramSemanticAction(CompilerState * compilerState, Program * recursiveProgram, ProjectStructure * projectStructure){
	_logSyntacticAnalyzerAction(__FUNCTION__);
	Program * program = calloc(1, sizeof(Program));
	program->projectStructure1 = projectStructure;
	program->program = recursiveProgram;
	program->type = MULTIPLE;
	compilerState->abstractSyntaxtTree = program;
	if (0 < flexCurrentContext()) {
		logError(_logger, "The final context is not the default (0): %d", flexCurrentContext());
		compilerState->succeed = false;
	}
	else {
		compilerState->succeed = true;
	}
	return program;
}

Program * ProjectStructureProgramSemanticAction(CompilerState * compilerState, ProjectStructure * projectStructure) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	Program * program = calloc(1, sizeof(Program));
	program->projectStructure = projectStructure;
	program->type = SINGLE;
	compilerState->abstractSyntaxtTree = program;
	if (0 < flexCurrentContext()) {
		logError(_logger, "The final context is not the default (0): %d", flexCurrentContext());
		compilerState->succeed = false;
	}
	else {
		compilerState->succeed = true;
	}
	return program;
}

TaskOptionDependsOn * TaskOptionDependsOnSemanticAction(TaskOptionDependsOn * recursiveTaskOptionDependsOn, char * id, char * id2){
	_logSyntacticAnalyzerAction(__FUNCTION__);
	TaskOptionDependsOn * taskOptionDependsOn = calloc(1, sizeof(TaskOptionDependsOn));
	taskOptionDependsOn->id=id;
	taskOptionDependsOn->id2=id2;
	taskOptionDependsOn->taskOptionDependsOn = recursiveTaskOptionDependsOn;

	struct Project *project;
	HASH_FIND_STR(projects, currentProjectId, project);

	if(project != NULL){
		struct Task *task;
		HASH_FIND_STR(projects->tasks, currentTaskId, task);

		if(task != NULL){
			task->depends_on[dependsCount++] = id2;
		}
	}

	return taskOptionDependsOn;
}

ProjectBodyOptionals * ProjectBodyOptionalsSemanticAction(MaxTasks * maxTasks, CategoriesId * categoriesId, MaxPoints * maxPoints, ProjectStart * projectStart){
	_logSyntacticAnalyzerAction(__FUNCTION__);
	ProjectBodyOptionals * projectBodyOptionals = calloc(1, sizeof(ProjectBodyOptionals));
	projectBodyOptionals->maxTasks = maxTasks;
	projectBodyOptionals->categoriesId = categoriesId;
	projectBodyOptionals->maxPoints = maxPoints;
	projectBodyOptionals->projectStart = projectStart;
	return projectBodyOptionals;
}

MaxTasks * MaxTasksSemanticAction(int tasks){
	_logSyntacticAnalyzerAction(__FUNCTION__);
	MaxTasks * maxTasks = calloc(1, sizeof(MaxTasks));
	maxTasks->tasks = tasks;

	struct Project *project;
	HASH_FIND_STR(projects, currentProjectId, project);

	if(project != NULL){
		project->max_tasks = tasks;
	}

	return maxTasks;
}

CategoriesId * CategoriesIdSemanticAction(char * id, char * name, BodyCategoriesOption * bodyCategoriesOption){
	_logSyntacticAnalyzerAction(__FUNCTION__);
	CategoriesId * categoriesId = calloc(1, sizeof(CategoriesId));
	categoriesId->id = id;
	categoriesId->name = name;
	categoriesId->bodyCategoriesOption = bodyCategoriesOption;

	struct Project *project;
	HASH_FIND_STR(projects, currentProjectId, project);

	if(project != NULL){
		struct Category *newCategory;
		HASH_FIND_STR(project->categories, id, newCategory);

		if(newCategory == NULL){
			newCategory = malloc(sizeof(struct Category));
			newCategory->name = name;
			// Agregar la tarea a la tabla hash
			HASH_ADD_KEYPTR(hh, projects->categories, id, strlen(id), newCategory);
		}
		else{
			logError(_logger, "Same ID's Category definition", flexCurrentContext());
		}
	}

	return categoriesId;
}

MaxPoints * MaxPointsSemanticAction(int points){
	_logSyntacticAnalyzerAction(__FUNCTION__);
	MaxPoints * maxPoints = calloc(1, sizeof(MaxPoints));
	maxPoints->points = points;
	
	struct Project *project;
	HASH_FIND_STR(projects, currentProjectId, project);

	if(project != NULL){
		project->max_points = points;
	}

	return maxPoints;
}

ProjectStart * ProjectStartSemanticAction(char * date){
	_logSyntacticAnalyzerAction(__FUNCTION__);
	ProjectStart * projectStart = calloc(1, sizeof(ProjectStart));
	projectStart->date = date;
	return projectStart;
}


BodyCategoriesOption * RecursiveCategoriesOptionSemanticAction(BodyCategoriesOption * recursiveBodyCategoriesOption, char * id, char * name){
	_logSyntacticAnalyzerAction(__FUNCTION__);
	BodyCategoriesOption * bodyCategoriesOption = calloc(1,sizeof(BodyCategoriesOption));
	bodyCategoriesOption->id=id;
	bodyCategoriesOption->name=name;
	bodyCategoriesOption->bodyCategoriesOption = recursiveBodyCategoriesOption;

	struct Project *project;
	HASH_FIND_STR(projects, currentProjectId, project);

	if(project != NULL){
		struct Category *newCategory;
		HASH_FIND_STR(project->categories, id, newCategory);

		if(newCategory == NULL){
			newCategory = malloc(sizeof(struct Category));
			newCategory->name = name;
			// Agregar la tarea a la tabla hash
			HASH_ADD_KEYPTR(hh, projects->categories, id, strlen(id), newCategory);
		}
		else{
			logError(_logger, "Same ID's Category definition", flexCurrentContext());
		}
	}


	return bodyCategoriesOption;
}

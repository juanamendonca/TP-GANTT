#include "BisonActions.h"

/* MODULE INTERNAL STATE */

static Logger * _logger = NULL;

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
 * Logs a syntactic-analyzer action in DEBUGGING level.
 */
static void _logSyntacticAnalyzerAction(const char * functionName) {
	logDebugging(_logger, "%s", functionName);
}

/* PUBLIC FUNCTIONS */

TaskOptionals * sevenTaskOptionalsSemanticAction(char * id, int points, char * id2, char * id3, TaskOptionDependsOn * taskOptionDependsOn, CATEGORY_POINTS_DEPENDS){
	_logSyntacticAnalyzerAction(__FUNCTION__);
	TaskOptionals * taskOptionals = calloc(1,sizeof(TaskOptionals));
	taskOptionals->id1 = id1;
	taskOptionals->id2 = id2;
	taskOptionals->id3 = id3;
	taskOptionals->points = points;
	taskOptionals->taskOptionDependsOn = taskOptionDependsOn;
	taskOptionals->type = type;
	return taskOptionals;
}

TaskOptionals * sixTaskOptionalsSemanticAction(int points, char * id1, char * id2, TaskOptionDependsOn * taskOptionDependsOn, TaskOptionalsType type){
	_logSyntacticAnalyzerAction(__FUNCTION__);
	TaskOptionals * taskOptionals = calloc(1,sizeof(TaskOptionals));
	taskOptionals->id1 = id1;
	taskOptionals->id2 = id2;
	taskOptionals->points = points;
	taskOptionals->taskOptionDependsOn = taskOptionDependsOn;
	taskOptionals->type = type;
	return taskOptionals;
}

TaskOptionals * fiveTaskOptionalsSemanticAction(char * id1, char * id2, char * id3, TaskOptionDependsOn * taskOptionDependsOn, TaskOptionalsType type){
	_logSyntacticAnalyzerAction(__FUNCTION__);
	TaskOptionals * taskOptionals = calloc(1,sizeof(TaskOptionals));
	taskOptionals->id1 = id1;
	taskOptionals->id2 = id2;
	taskOptionals->id3 = id3;
	taskOptionals->taskOptionDependsOn = taskOptionDependsOn;
	taskOptionals->type = type;
	return taskOptionals;
}

TaskOptionals * fourTaskOptionalsSemanticAction(char * id, int points, TaskOptionalsType type){
	_logSyntacticAnalyzerAction(__FUNCTION__);
	TaskOptionals * taskOptionals = calloc(1,sizeof(TaskOptionals));
	taskOptionals->id1 = id;
	taskOptionals->points = points;
	taskOptionals->type = type;
	return taskOptionals;
}

TaskOptionals * threeTaskOptionalsSemanticAction(char * id1, char * id2, TaskOptionDependsOn * taskOptionDependsOn, TaskOptionalsType type){
	_logSyntacticAnalyzerAction(__FUNCTION__);
	TaskOptionals * taskOptionals = calloc(1,sizeof(TaskOptionals));
	taskOptionals->id1 = id1;
	taskOptionals->id2 = id2;
	taskOptionals->taskOptionDependsOn = taskOptionDependsOn;
	taskOptionals->type = type;
	return taskOptionals;
}

TaskOptionals * twoTaskOptionalsSemanticAction(int points, TaskOptionalsType type){
	_logSyntacticAnalyzerAction(__FUNCTION__);
	TaskOptionals * taskOptionals = calloc(1,sizeof(TaskOptionals));
	taskOptionals->points = points;
	taskOptionals->type = type;
	return taskOptionals;
}

TaskOptionals * oneTaskOptionalsSemanticAction(char * id, TaskOptionalsType type){
	_logSyntacticAnalyzerAction(__FUNCTION__);
	TaskOptionals * taskOptionals = calloc(1,sizeof(TaskOptionals));
	taskOptionals->id1 = id;
	taskOptionals->type = type;
	return taskOptionals;
}

TaskLengthFormat * intervalLengthFormatSemanticAction(char * interval){
	_logSyntacticAnalyzerAction(__FUNCTION__);
	TaskLengthFormat * taskLengthFormat = calloc(1,sizeof(TaskLengthFormat));
	taskLengthFormat->interval = interval;
	taskLengthFormat->type = DURATION;
	return taskLengthFormat;
}

TaskLengthFormat * dateLengthFormatSemanticAction(char * startDate, char * finishDate){
	_logSyntacticAnalyzerAction(__FUNCTION__);
	TaskLengthFormat * taskLengthFormat = calloc(1,sizeof(TaskLengthFormat));
	taskLengthFormat->startDate = startDate;
	taskLengthFormat->finishDate = finishDate;
	taskLengthFormat->type = TIME_PERIOD;
	return taskLengthFormat;
}

TaskStructure * optionalsStructureSemanticAction(char * id, char * name, TaskLengthFormat * taskLengthFormat, TaskOptionals * taskOptionals){
	_logSyntacticAnalyzerAction(__FUNCTION__);
	TaskStructure * taskStructure = calloc(1,sizeof(TaskStructure));
	taskStructure->id = id;
	taskStructure->name = name;
	taskStructure->taskLengthFormat = taskLengthFormat;
	taskStructure->taskOptionals = taskOptionals;
	taskStructure->type = OPTIONALS;
	return taskStructure;
}

TaskStructure * lengthFormatStructureSemanticAction(char * id, char * name, TaskLengthFormat * taskLengthFormat){
	_logSyntacticAnalyzerAction(__FUNCTION__);
	TaskStructure * taskStructure = calloc(1,sizeof(TaskStructure));
	taskStructure->id = id;
	taskStructure->name = name;
	taskStructure->taskLengthFormat = taskLengthFormat;
	taskStructure->type = BASIC;
	return taskStructure;
}

TaskList * structureRecursiveListSemanticAction(TaskList * recursiveTaskList, TaskStructure * taskStructure){
	_logSyntacticAnalyzerAction(__FUNCTION__);
	TaskList * taskList = calloc(1,sizeof(TaskList));
	taskList->taskList = recursiveTaskList;
	taskList->taskStructure = taskStructure;
	taskList->type = SINGLE;
	return taskList;
}

TaskList * structureListSemanticAction(TaskStructure * taskStructure){
	_logSyntacticAnalyzerAction(__FUNCTION__);
	TaskList * taskList = calloc(1,sizeof(TaskList));
	taskList->taskStructure = taskStructure;
	taskList->type = SINGLE;
	return taskList;
}

ProjectBody * optionalsProjectBodySemanticAction(ProjectBodyOptionals * projectBodyOptionals, TaskList * taskList){
	_logSyntacticAnalyzerAction(__FUNCTION__);
	ProjectBody * projectBody = calloc(1,sizeof(ProjectBody));
	projectBody->projectBodyOptionals = projectBodyOptionals;
	projectBody->taskList = taskList;
	projectBody->type = OPTIONALS;
	return projectBody;
}

ProjectBody * taskListProjectBodySemanticAction(TaskList * taskList){
	_logSyntacticAnalyzerAction(__FUNCTION__);
	ProjectBody * projectBody = calloc(1,sizeof(ProjectBody));
	projectBody->taskList = taskList;
	projectBody->type = BASIC;
	return projectBody;
}

ProjectUnion * IdProjectUnionSemanticAction(ProjectUnion * recursiveProjectUnion, char * id){
	_logSyntacticAnalyzerAction(__FUNCTION__);
	ProjectUnion * projectUnion = calloc(1,sizeof(ProjectUnion));
	projectUnion->projectUnion = recursiveProjectUnion;
	projectUnion->id = id;
	return projectUnion;
}

ProjectOptionals * BothOptionalsSemanticAction(char * id1, char * id2, ProjectUnion * projectUnion){
	_logSyntacticAnalyzerAction(__FUNCTION__);
	ProjectOptionals * projectOptionals = calloc(1,sizeof(ProjectOptionals));
	projectOptionals->id1 = id1;
	projectOptionals->id1 = id2;
	projectOptionals->projectUnion = projectUnion;
	projectOptionals->type = BOTH;
	return projectOptionals;
}

ProjectOptionals * ProjectUnionOptionalsSemanticAction(char * id, ProjectUnion * projectUnion){
	_logSyntacticAnalyzerAction(__FUNCTION__);
	ProjectOptionals * projectOptionals = calloc(1,sizeof(ProjectOptionals));
	projectOptionals->id1 = id;
	projectOptionals->projectUnion = projectUnion;
	projectOptionals->type = WITH;
	return projectOptionals;
}

ProjectOptionals * IdOptionalsSemanticAction(char * id){
	_logSyntacticAnalyzerAction(__FUNCTION__);
	ProjectOptionals * projectOptionals = calloc(1,sizeof(ProjectOptionals));
	projectOptionals->id1 = id;
	projectOptionals->type = DEPENDS_ON;
	return projectOptionals;
}

ProjectStructure * OptionalsProjectStructureSemanticAction(char * id, char * name, ProjectOptionals * projectOptionals, ProjectBody * projectBody){
	_logSyntacticAnalyzerAction(__FUNCTION__);
	ProjectStructure * projectStructure = calloc(1,sizeof(ProjectStructure));
	projectStructure->id = id;
	projectStructure->name = name;
	projectStructure->projectBody = projectBody;
	projectStructure->projectOptionals = projectOptionals;
	projectStructure->type = OPTIONALS;
	return projectStructure;
}

ProjectStructure * ProjectBodyProjectStructureSemanticAction(char * id, char * name, ProjectBody * projectBody){
	_logSyntacticAnalyzerAction(__FUNCTION__);
	ProjectStructure * projectStructure = calloc(1,sizeof(ProjectStructure));
	projectStructure->id = id;
	projectStructure->name = name;
	projectStructure->projectBody = projectBody;
	projectStructure->type = BASIC;
	return projectStructure;
}

Program * ProjectStructureRecursiveProgramSemanticAction(Program * recursiveProgram, ProjectStructure * projectStructure){
	_logSyntacticAnalyzerAction(__FUNCTION__);
	Program * program = calloc(1, sizeof(Program));
	program->projectStructure = projectStructure;
	program->program = recursiveProgram;
	program->type = MULTIPLE;
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

























TaskOptionDependsOn * TaskOptionDependsOnSemanticAction(char * id, char * id2){
	_logSyntacticAnalyzerAction(__FUNCTION__);
	TaskOptionDependsOn * taskOptionDependsOn = calloc(1, sizeOf(TaskOptionDependsOn));
	taskOptionDependsOn->id=id;
	taskOptionDependsOn->id2=id2;
	return taskOptionDependsOn;
}

ProjectBodyOptionals * OneBodyOptionalsSemanticAction(int maxTasks){
	_logSyntacticAnalyzerAction(__FUNCTION__);
	ProjectBodyOptionals * projectBodyOptionals = calloc(1, sizeof(ProjectBodyOptionals));
	projectBodyOptionals->maxTasks = maxTasks;
	projectBodyOptionals->type=TASKS;
	return projectBodyOptionals;
}

ProjectBodyOptionals * TwoBodyOptionalsSemanticAction( char * id, char * name, BodyCategoriesOption * bodyCategoriesOption){
	_logSyntacticAnalyzerAction(__FUNCTION__);
	ProjectBodyOptionals * projectBodyOptionals = calloc(1, sizeof(ProjectBodyOptionals));
	projectBodyOptionals->id = id;
	projectBodyOptionals->name = name;
	projectBodyOptionals->bodyCategoriesOption = bodyCategoriesOption;
	projectBodyOptionals->type=CATEGORIES;
	return projectBodyOptionals;
}

ProjectBodyOptionals * ThreeBodyOptionalsSemanticAction(int maxPoints){
	_logSyntacticAnalyzerAction(__FUNCTION__);
	ProjectBodyOptionals * projectBodyOptionals = calloc(1, sizeof(ProjectBodyOptionals));
	projectBodyOptionals->maxPoints = maxPoints;
	projectBodyOptionals->type=POINTS;
	return projectBodyOptionals;
}

ProjectBodyOptionals * FourBodyOptionalsSemanticAction(char * startDate){
	_logSyntacticAnalyzerAction(__FUNCTION__);
	ProjectBodyOptionals * projectBodyOptionals = calloc(1, sizeof(ProjectBodyOptionals));
	projectBodyOptionals->startDate=startDate;
	projectBodyOptionals->type=START;
	return projectBodyOptionals;
}

ProjectBodyOptionals * FiveBodyOptionalsSemanticAction(int maxTasks,  char * id, char * name, BodyCategoriesOption * bodyCategoriesOption){
	_logSyntacticAnalyzerAction(__FUNCTION__);
	ProjectBodyOptionals * projectBodyOptionals = calloc(1, sizeof(ProjectBodyOptionals));
	projectBodyOptionals->maxTasks = maxTasks;
	projectBodyOptionals->id = id;
	projectBodyOptionals->name = name;
	projectBodyOptionals->bodyCategoriesOption = bodyCategoriesOption;
	projectBodyOptionals->type=TASKS_CATEGORIES;
	return projectBodyOptionals;
}

ProjectBodyOptionals * SixBodyOptionalsSemanticAction(int maxTasks, int maxPoints){
	_logSyntacticAnalyzerAction(__FUNCTION__);
	ProjectBodyOptionals * projectBodyOptionals = calloc(1, sizeof(ProjectBodyOptionals));
	projectBodyOptionals->maxTasks = maxTasks;
	projectBodyOptionals->maxPoints = maxPoints;
	projectBodyOptionals->type=TASKS_POINTS;
	return projectBodyOptionals;
}



ProjectBodyOptionals * SevenBodyOptionalsSemanticAction(int maxTasks,char * startDate){
	_logSyntacticAnalyzerAction(__FUNCTION__);
	ProjectBodyOptionals * projectBodyOptionals = calloc(1, sizeof(ProjectBodyOptionals));
	projectBodyOptionals->maxTasks = maxTasks;
	projectBodyOptionals->startDate=startDate;
	projectBodyOptionals->type=TASKS_START;
	return projectBodyOptionals;
}


ProjectBodyOptionals * EightBodyOptionalsSemanticAction(char * id, char * name, BodyCategoriesOption * bodyCategoriesOption, int maxPoints){
	_logSyntacticAnalyzerAction(__FUNCTION__);
	ProjectBodyOptionals * projectBodyOptionals = calloc(1, sizeof(ProjectBodyOptionals));
	projectBodyOptionals->id = id;
	projectBodyOptionals->name = name;
	projectBodyOptionals->bodyCategoriesOption = bodyCategoriesOption;
	projectBodyOptionals->maxPoints = maxPoints;
	projectBodyOptionals->type=CATEGORIES_POINTS;
	return projectBodyOptionals;
}



ProjectBodyOptionals * NineBodyOptionalsSemanticAction( char * id, char * name, BodyCategoriesOption * bodyCategoriesOption, char * startDate){
	_logSyntacticAnalyzerAction(__FUNCTION__);
	ProjectBodyOptionals * projectBodyOptionals = calloc(1, sizeof(ProjectBodyOptionals));
	projectBodyOptionals->id = id;
	projectBodyOptionals->name = name;
	projectBodyOptionals->bodyCategoriesOption = bodyCategoriesOption;
	projectBodyOptionals->startDate=startDate;
	projectBodyOptionals->type=CATEGORIES_START;
	return projectBodyOptionals;

}


ProjectBodyOptionals * TenBodyOptionalsSemanticAction( int maxPoints, 	char * startDate){
	_logSyntacticAnalyzerAction(__FUNCTION__);
	ProjectBodyOptionals * projectBodyOptionals = calloc(1, sizeof(ProjectBodyOptionals));
	projectBodyOptionals->maxPoints = maxPoints;
	projectBodyOptionals->startDate=startDate;
	projectBodyOptionals->type=POINTS_START;
	return projectBodyOptionals;
}


ProjectBodyOptionals * ElevenBodyOptionalsSemanticAction(int maxTasks,char * id, char * name, BodyCategoriesOption * bodyCategoriesOption, int maxPoints){
	_logSyntacticAnalyzerAction(__FUNCTION__);
	ProjectBodyOptionals * projectBodyOptionals = calloc(1, sizeof(ProjectBodyOptionals));
	projectBodyOptionals->maxTasks=maxTasks;
	projectBodyOptionals->id=id;
	projectBodyOptionals->name=name;
	projectBodyOptionals->bodyCategoriesOption=bodyCategoriesOption;
	projectBodyOptionals->maxPoints=maxPoints;
	projectBodyOptionals->type=	TASKS_CATEGORIES_POINTS;

	return projectBodyOptionals;
}



ProjectBodyOptionals * TwelveBodyOptionalsSemanticAction(int maxTasks, int maxPoints, char * startDate){
	_logSyntacticAnalyzerAction(__FUNCTION__);
	ProjectBodyOptionals * projectBodyOptionals = calloc(1, sizeof(ProjectBodyOptionals));
	projectBodyOptionals->maxTasks = maxTasks;
	projectBodyOptionals->maxPoints = maxPoints;
	projectBodyOptionals->startDate = startDate;
	projectBodyOptionals->type = TASKS_POINTS_START;
	return projectBodyOptionals;
}


ProjectBodyOptionals * ThirteenBodyOptionalsSemanticAction(char * id, char * name, BodyCategoriesOption * bodyCategoriesOption, int maxPoints, 	char * startDate){
	_logSyntacticAnalyzerAction(__FUNCTION__);
	ProjectBodyOptionals * projectBodyOptionals = calloc(1, sizeof(ProjectBodyOptionals));
	projectBodyOptionals->id = id;
	projectBodyOptionals->name = name;
	projectBodyOptionals->bodyCategoriesOption = bodyCategoriesOption;
	projectBodyOptionals->maxPoints = maxPoints;
	projectBodyOptionals->startDate = startDate;
	projectBodyOptionals->type = CATEGORIES_POINTS_START;
	return projectBodyOptionals;
}

ProjectBodyOptionals * FourteenBodyOptionalsSemanticAction(int maxTasks,  char * id, char * name, BodyCategoriesOption * bodyCategoriesOption, int maxPoints, 	char * startDate){
	_logSyntacticAnalyzerAction(__FUNCTION__);
	ProjectBodyOptionals * projectBodyOptionals = calloc(1, sizeof(ProjectBodyOptionals));
	projectBodyOptionals->maxTasks = maxTasks;
	projectBodyOptionals->id = id;
	projectBodyOptionals->name = name;
	projectBodyOptionals->bodyCategoriesOption = bodyCategoriesOption;
	projectBodyOptionals->maxPoints = maxPoints;
	projectBodyOptionals->startDate=startDate;
	projectBodyOptionals->type=TASKS_CATEGORIES_POINTS_START;
	return projectBodyOptionals;
}

ProjectBodyOptionals * FifthteenBodyOptionalsSemanticAction(int maxTasks,  char * id, char * name, BodyCategoriesOption * bodyCategoriesOption,char * startDate){
		_logSyntacticAnalyzerAction(__FUNCTION__);
	ProjectBodyOptionals * projectBodyOptionals = calloc(1, sizeof(ProjectBodyOptionals));
	projectBodyOptionals->maxTasks = maxTasks;
	projectBodyOptionals->id = id;
	projectBodyOptionals->name = name;
	projectBodyOptionals->bodyCategoriesOption = bodyCategoriesOption;
	projectBodyOptionals->startDate=startDate;
	projectBodyOptionals->type=TASK_CATEGORIES_START;
	return projectBodyOptionals;
}


BodyCategoriesOption * RecursiveCategoriesOptionSemanticAction(char * id, char * name){
	_logSyntacticAnalyzerAction(__FUNCTION__);
	BodyCategoriesOption * bodyCategoriesOption = calloc(1,sizeOf(BodyCategoriesOption));
	bodyCategoriesOption->id=id;
	bodyCategoriesOption->name=name;
	return bodyCategoriesOption;
}
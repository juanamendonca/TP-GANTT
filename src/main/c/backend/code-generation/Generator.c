#include "Generator.h"

/* MODULE INTERNAL STATE */
const char _indentationCharacter = ' ';
const char _indentationSize = 4;
static Logger * _logger = NULL;
static FILE *file = NULL;
extern struct Project * projects;

void initializeGeneratorModule() {
	_logger = createLogger("Generator");
}

void shutdownGeneratorModule() {
	if (_logger != NULL) {
		destroyLogger(_logger);
	}
}

/** PRIVATE FUNCTIONS */

static void _generateEpilogue();
static void _generateTaskOptionDependsOnText(const unsigned int indentationLevel, TaskOptionDependsOn * taskOptionDependsOn, char * projectId);
static int _generateTaskOptionDependsOn(const unsigned int indentationLevel, TaskOptionDependsOn * taskOptionDependsOn, char * projectId);
static void _generateDependsOnIdText(const unsigned int indentationLevel, DependsOnId * dependsOnId, char * projectId);
static void _generateDependsOnId(const unsigned int indentationLevel, DependsOnId * dependsOnId, char * projectId, char * taskId);
static void _generateTaskStructure(const unsigned int indentationLevel, TaskStructure * taskStructure, char * projectId);
static void _generateTaskList(const unsigned int indentationLevel, TaskList * taskList, char * projectId);
static void _generateProjectUnion(const unsigned int indentationLevel, ProjectUnion * projectUnion);
static void _generateProjectStructure(const unsigned int indentationLevel, ProjectStructure * projectStructure);
static void _generateProgram(Program * program);
static void _generatePrologue(void);
static char * _indentation(const unsigned int indentationLevel);
static void _output(const unsigned int indentationLevel, const char * const format, ...);
static void openFile();
static void closeFile();


/**
 * Creates the epilogue of the generated output, that is, the final lines that
 * completes a valid Latex document.
 */
static void _generateEpilogue() {
	int count = HASH_COUNT(projects);
	if(count == 1){
		_output(0, "%s", "fig1.write_html(\'gantt.html\')");
	} else {
		_output(0, "%s", "with open(\'gantt.html\', \'w\') as f:\n");
		for(int i = 1; i <= count; i++){
			_output(1, "%s%d%s", "f.write(fig", i, ".to_html(full_html=False, include_plotlyjs=\'cdn\'))\n");
		}
	}
}

static void _generateTaskOptionDependsOnText(const unsigned int indentationLevel, TaskOptionDependsOn * taskOptionDependsOn, char * projectId){
	struct Project *project;
	struct Project *otherProject;
	struct Task * task;
	
	if(strcmp(projectId, taskOptionDependsOn->id) != 0){
		HASH_FIND_STR(projects, taskOptionDependsOn->id, otherProject);
		HASH_FIND_STR(otherProject->tasks, taskOptionDependsOn->id2, task);
		_output(indentationLevel, "%s%s%s%s%s", "<br>(Depends on \\\"", task->name, "\\\"<br>From \\\"", otherProject->name, "\\\")");
	} else {
		HASH_FIND_STR(projects, projectId, project);
		HASH_FIND_STR(project->tasks, taskOptionDependsOn->id2, task);
		_output(indentationLevel, "%s%s%s", "<br>(Depends on \\\"", task->name, "\\\")");
	}

	if(taskOptionDependsOn->taskOptionDependsOn != NULL){
		_generateTaskOptionDependsOnText(indentationLevel, taskOptionDependsOn->taskOptionDependsOn, projectId);
	}
}

static void _generateDependsOnIdText(const unsigned int indentationLevel, DependsOnId * dependsOnId, char * projectId){
	struct Project *project;
	struct Project *otherProject;
	struct Task * task;

	if(strcmp(projectId, dependsOnId->id1) != 0){
		HASH_FIND_STR(projects, dependsOnId->id1, otherProject);
		HASH_FIND_STR(otherProject->tasks, dependsOnId->id2, task);
		_output(indentationLevel, "%s%s%s%s%s", "<br>(Depends on \\\"", task->name, "\\\"<br>From \\\"", otherProject->name, "\\\")");
	} else {
		HASH_FIND_STR(projects, projectId, project);
		HASH_FIND_STR(project->tasks, dependsOnId->id2, task);
		_output(indentationLevel, "%s%s%s", "<br>(Depends on \\\"", task->name, "\\\")");
	}
	
	if(dependsOnId->taskOptionDependsOn != NULL){
		_generateTaskOptionDependsOnText(indentationLevel, dependsOnId->taskOptionDependsOn, projectId);
	}
}

static int _generateTaskOptionDependsOn(const unsigned int indentationLevel, TaskOptionDependsOn * taskOptionDependsOn, char * projectId){
	int previousEndPoint = 0;
	int currentEndPoint;
	
	struct Project * project;
	struct Task * task;
	HASH_FIND_STR(projects, taskOptionDependsOn->id, project);
	HASH_FIND_STR(project->tasks, taskOptionDependsOn->id2, task);
	currentEndPoint = task->lengthFinish == -1 ? task->lengthStart + task->base : task->lengthFinish + task->base;

	if(taskOptionDependsOn->taskOptionDependsOn != NULL){
		previousEndPoint = _generateTaskOptionDependsOn(indentationLevel, taskOptionDependsOn->taskOptionDependsOn, projectId);
	}

	if(previousEndPoint > currentEndPoint){
		return previousEndPoint;
	} else {
		return currentEndPoint;
	}
}

static void _generateDependsOnId(const unsigned int indentationLevel, DependsOnId * dependsOnId, char * projectId, char * taskId){
	int previousEndPoint = 0;
	int currentEndPoint;
	int max;
	
	struct Project * project;
	struct Task * task;
	HASH_FIND_STR(projects, dependsOnId->id1, project);
	HASH_FIND_STR(project->tasks, dependsOnId->id2, task);
	currentEndPoint = task->lengthFinish == -1 ? task->lengthStart + task->base : task->lengthFinish + task->base;
	
	if(dependsOnId->taskOptionDependsOn != NULL){
		previousEndPoint = _generateTaskOptionDependsOn(indentationLevel, dependsOnId->taskOptionDependsOn, projectId);
	}
	
	max = previousEndPoint > currentEndPoint ? previousEndPoint : currentEndPoint;

	_output(indentationLevel, "%s%d", ", Base=", max);
	
	struct Project * myProject;
	struct Task * myTask;
	HASH_FIND_STR(projects, projectId, myProject);
	
	HASH_FIND_STR(myProject->tasks, taskId, myTask);
	
	myTask->base = max;
	myProject->baseFlag = 1;
}

static void _generateTaskStructure(const unsigned int indentationLevel, TaskStructure * taskStructure, char * projectId){
	struct Project *project;
	HASH_FIND_STR(projects, projectId, project);
	int length;

	_output(indentationLevel, "%s%s", "dict(Task=\"", taskStructure->name);
	if(taskStructure->taskOptionals != NULL){
		if(taskStructure->taskOptionals->unique != NULL){
			_output(0, "%s", " (Unique)");
		}
		if(taskStructure->taskOptionals->pointsInteger != NULL){
			_output(0, "%s%d%s", " (", taskStructure->taskOptionals->pointsInteger->points, " points)");
		}
	}

	if(taskStructure->taskLengthFormat->type != TIME_PERIOD){
		if(taskStructure->taskLengthFormat->rightInterval != -1){
			_output(0, "%s%d%s", "<br>(Minimum length: ", taskStructure->taskLengthFormat->leftInterval, ")");
		}
	}

	if(taskStructure->taskOptionals != NULL && taskStructure->taskOptionals->dependsOnId != NULL){
		_generateDependsOnIdText(0, taskStructure->taskOptionals->dependsOnId, projectId);
	}

	_output(0, "%s", "\", ");

	if(taskStructure->taskLengthFormat->rightInterval != -1){
		length = taskStructure->taskLengthFormat->rightInterval;
	} else {
		length = taskStructure->taskLengthFormat->leftInterval;
	}

	switch (project->format){
		case HOUR_TYPE: _output(0, "%s%d", "Hours=", length); break;
		case DAY_TYPE: _output(0, "%s%d", "Days=", length); break;
		case WEEK_TYPE: _output(0, "%s%d", "Weeks=", length); break;
		case MONTH_TYPE: _output(0, "%s%d", "Months=", length); break;
		case DATE_TYPE: _output(0, "%s%s%s%s%s", "Start=\'", taskStructure->taskLengthFormat->startDate, "\', Finish=\'", taskStructure->taskLengthFormat->finishDate, "\'"); break;
	}


	if(taskStructure->taskOptionals != NULL && taskStructure->taskOptionals->categoryId != NULL){
		struct Category * category;
		HASH_FIND_STR(project->categories, taskStructure->taskOptionals->categoryId->id, category);
		_output(0, "%s%s%s", ", Category=\"", category->name, "\"");
	}

	if(project->format != DATE_TYPE && taskStructure->taskOptionals != NULL && taskStructure->taskOptionals->dependsOnId != NULL){
		_generateDependsOnId(0, taskStructure->taskOptionals->dependsOnId, projectId, taskStructure->taskId->id);
	}

	if(taskStructure->taskOptionals != NULL && taskStructure->taskOptionals->unique != NULL){
		struct Task * task;
		int max = 0;
		int current;

		for (task = project->tasks; task != NULL; task = task->hh.next){
			if(strcmp(task->taskId, taskStructure->taskId->id) != 0){
				current = task->lengthFinish == -1 ? task->lengthStart + task->base : task->lengthFinish + task->base;
				max = current > max ? current : max;
			}
		}
		_output(0, "%s%d", ", Base=", max);
		project->baseFlag = 1; 
	}

	_output(0, "%s", "),\n");
}

static void _generateTaskList(const unsigned int indentationLevel, TaskList * taskList, char * projectId){
	if (taskList->type == SINGLE){
        _generateTaskStructure(indentationLevel, taskList->taskStructure1, projectId);
    } else {
        _generateTaskList(indentationLevel, taskList->taskList, projectId);
		_generateTaskStructure(indentationLevel, taskList->taskStructure2, projectId);
    }
}

static void _generateProjectUnion(const unsigned int indentationLevel, ProjectUnion * projectUnion){
	struct Project * dependsOnProject;
	HASH_FIND_STR(projects, projectUnion->id, dependsOnProject);
	_output(indentationLevel, "%s%s%s", ", \\\"", dependsOnProject->name, "\\\"");
	if(projectUnion->projectUnion != NULL){
		_generateProjectUnion(indentationLevel, projectUnion->projectUnion);
	}
}

static void _generateProjectStructure(const unsigned int indentationLevel, ProjectStructure * projectStructure){
	struct Project *project;
	struct Project * dependsOnProject;
	struct Project * withProject;
	HASH_FIND_STR(projects, projectStructure->projectStructureCommon->projectId->id, project);
	
	if(project != NULL){
		_output(indentationLevel, "%s%d%s", "df", project->projectNumber, " = pd.DataFrame([\n");
	}
	
	if(projectStructure->projectBody != NULL && projectStructure->projectBody->taskList != NULL){
		_generateTaskList(indentationLevel + 1, projectStructure->projectBody->taskList, projectStructure->projectStructureCommon->projectId->id);
	}
	
	_output(indentationLevel, "%s%d%s", "])\n\nfig", project->projectNumber, " = ");

	switch (project->format){
		case HOUR_TYPE: _output(indentationLevel, "%s%d%s", "px.bar(df", project->projectNumber, ", x=\"Hours\", orientation=\"h\", "); break;
		case DAY_TYPE: _output(indentationLevel, "%s%d%s", "px.bar(df", project->projectNumber, ", x=\"Days\", orientation=\"h\", "); break;
		case WEEK_TYPE: _output(indentationLevel, "%s%d%s", "px.bar(df", project->projectNumber, ", x=\"Weeks\", orientation=\"h\", "); break;
		case MONTH_TYPE: _output(indentationLevel, "%s%d%s", "px.bar(df", project->projectNumber, ", x=\"Months\", orientation=\"h\", "); break;
		case DATE_TYPE: _output(indentationLevel, "%s%d%s", "px.timeline(df", project->projectNumber, ", x_start=\"Start\", x_end=\"Finish\", "); break;
	}
	_output(indentationLevel, "%s%s", "y=\"Task\", title=\"", projectStructure->projectStructureCommon->name);
	
	if(projectStructure->projectBody != NULL && projectStructure->projectBody->projectBodyOptionals != NULL && projectStructure->projectBody->projectBodyOptionals->projectStart != NULL){
		_output(indentationLevel, "%s%s%s", " (Start:", projectStructure->projectBody->projectBodyOptionals->projectStart->date, ")");
	}
	
	if(projectStructure->projectBody != NULL && projectStructure->projectBody->projectBodyOptionals != NULL && projectStructure->projectBody->projectBodyOptionals->maxPoints != NULL){
		_output(indentationLevel, "%s%d%s", " (Maximum points:", projectStructure->projectBody->projectBodyOptionals->maxPoints->points, ")");
	}
	
	if(projectStructure->projectBody != NULL && projectStructure->projectBody->projectBodyOptionals != NULL && projectStructure->projectBody->projectBodyOptionals->maxTasks != NULL){
		_output(indentationLevel, "%s%d%s", " (Maximum Tasks:", projectStructure->projectBody->projectBodyOptionals->maxTasks->tasks, ")");
	}
	
	if(projectStructure->projectOptionals != NULL && projectStructure->projectOptionals->type == DEPENDS_ON_P){
		HASH_FIND_STR(projects, projectStructure->projectOptionals->id1, dependsOnProject);
		_output(indentationLevel, "%s%s%s", " (Depends on \\\"", dependsOnProject->name, "\\\")");
	}
	

	if(projectStructure->projectOptionals != NULL && projectStructure->projectOptionals->type == WITH_P){
		HASH_FIND_STR(projects, projectStructure->projectOptionals->id2, withProject);
		_output(indentationLevel, "%s%s%s", " (With \\\"", withProject->name, "\\\"");
		if(projectStructure->projectOptionals->projectUnion1 != NULL){
			_generateProjectUnion(indentationLevel, projectStructure->projectOptionals->projectUnion1);
		}
		_output(indentationLevel, "%s", ")");
	}
	
	if(projectStructure->projectOptionals != NULL && projectStructure->projectOptionals->type == BOTH){
		HASH_FIND_STR(projects, projectStructure->projectOptionals->id1, dependsOnProject);
		_output(indentationLevel, "%s%s%s", " (Depends on \\\"", dependsOnProject->name, "\\\")");

		HASH_FIND_STR(projects, projectStructure->projectOptionals->id2, withProject);
		_output(indentationLevel, "%s%s%s", " (With \\\"", withProject->name, "\\\"");
		if(projectStructure->projectOptionals->projectUnion1 != NULL){
			_generateProjectUnion(indentationLevel, projectStructure->projectOptionals->projectUnion1);
		}
		_output(indentationLevel, "%s", ")");
	}
	
	_output(indentationLevel, "%s", "\"");

	if(projectStructure->projectBody != NULL && projectStructure->projectBody->projectBodyOptionals != NULL && projectStructure->projectBody->projectBodyOptionals->categoriesId != NULL){
		_output(indentationLevel, "%s", ", color=\'Category\'");
	}
	
	if(project->baseFlag){
		_output(indentationLevel, "%s", ", base=\"Base\"");
	}
	
	_output(indentationLevel, "%s%d%s", ")\nfig", project->projectNumber, ".update_yaxes(autorange=\"reversed\")\n");
}

/**
 * Generates the output of the program.
 */
static void _generateProgram(Program * program) {
    if (program->type == SINGLE){
        _generateProjectStructure(0, program->projectStructure);
    } else {
        _generateProgram(program->program);
		_generateProjectStructure(0, program->projectStructure1);
    }
}

/**
 * Creates the prologue of the generated output, a Latex document that renders
 * a tree thanks to the Forest package.
 *
 * @see https://ctan.dcc.uchile.cl/graphics/pgf/contrib/forest/forest-doc.pdf
 */
static void _generatePrologue(void) {
	_output(0, "%s",
		"import plotly.express as px\n"
        "import pandas as pd\n\n"
	);
}

/**
 * Generates an indentation string for the specified level.
 */
static char * _indentation(const unsigned int level) {
	return indentation(_indentationCharacter, level, _indentationSize);
}

/**
 * Outputs a formatted string to standard output.
 */
static void _output(const unsigned int indentationLevel, const char * const format, ...) {
	va_list arguments;
	va_start(arguments, format);
	char * indentation = _indentation(indentationLevel);
	char * effectiveFormat = concatenate(2, indentation, format);
	vfprintf(file, effectiveFormat, arguments);
	free(effectiveFormat);
	free(indentation);
	va_end(arguments);
}

static void openFile() {
    file = fopen("gantt.py", "w");
    if (file) {
        fclose(file);
    }
    file = fopen("gantt.py", "a");
}

static void closeFile() {
    if (file) {
        fclose(file);
        file = NULL;
    }
}

/** PUBLIC FUNCTIONS */

void generate(CompilerState * compilerState) {
	logDebugging(_logger, "Generating final output...");
    openFile();
	_generatePrologue();
	_generateProgram(compilerState->abstractSyntaxtTree);
	_generateEpilogue();
    closeFile();
	logDebugging(_logger, "Generation is done.");
}

#include "Generator.h"

/* MODULE INTERNAL STATE */
const char _indentationCharacter = ' ';
const char _indentationSize = 4;
static Logger * _logger = NULL;
static FILE *file = NULL;

void initializeGeneratorModule() {
	_logger = createLogger("Generator");
}

void shutdownGeneratorModule() {
	if (_logger != NULL) {
		destroyLogger(_logger);
	}
}

/** PRIVATE FUNCTIONS */

static void _generateEpilogue(const int value);
static void _generateBodyCategoriesOption(const unsigned int indentationLevel, BodyCategoriesOption * bodyCategoriesOption);
static void _generateProjectStart(const unsigned int indentationLevel, ProjectStart * projectStart);
static void _generateMaxPoints(const unsigned int indentationLevel, MaxPoints * maxPoints);
static void _generateCategoriesId(const unsigned int indentationLevel, CategoriesId * categoriesId);
static void _generateMaxTasks(const unsigned int indentationLevel, MaxTasks * maxTasks);
static void _generateProjectBodyOptionals(const unsigned int indentationLevel, ProjectBodyOptionals * projectBodyOptionals);
static void _generateTaskOptionDependsOn(const unsigned int indentationLevel, TaskOptionDependsOn * taskOptionDependsOn);
static void _generateDependsOnId(const unsigned int indentationLevel, DependsOnId * dependsOnId);
static void _generatePointsInteger(const unsigned int indentationLevel, PointsInteger * pointsInteger);
static void _generateCategoryId(const unsigned int indentationLevel, CategoryId * categoryId);
static void _generateUnique(const unsigned int indentationLevel, Unique * unique);
static void _generateTaskOptionals(const unsigned int indentationLevel, TaskOptionals * taskOptionals);
static void _generateTaskLengthFormat(const unsigned int indentationLevel, TaskLengthFormat * taskLengthFormat);
static void _generateTaskStructure(const unsigned int indentationLevel, TaskStructure * taskStructure);
static void _generateTaskList(const unsigned int indentationLevel, TaskList * taskList);
static void _generateProjectUnion(const unsigned int indentationLevel, ProjectUnion * projectUnion);
static void _generateTimeUnit(const unsigned int indentationLevel, TimeUnit * timeUnit);
static void _generateProjectBody(const unsigned int indentationLevel, ProjectBody * projectBody);
static void _generateProjectOptionals(const unsigned int indentationLevel, ProjectOptionals * projectOptionals);
static void _generateProjectStructureCommon(const unsigned int indentationLevel, ProjectStructureCommon * projectStructureCommon);
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
static void _generateEpilogue(const int value) {
	_output(0, "%s%d%s",
		"            [ $", value, "$, circle, draw, blue ]\n"
		"        ]\n"
		"    \\end{forest}\n"
		"\\end{document}\n\n"
	);
}

static void _generateProjectStructure(const unsigned int indentationLevel, ProjectStructure * projectStructure){

}

/**
 * Generates the output of the program.
 */
static void _generateProgram(Program * program) {
    if (program->type == SINGLE){
        _generateProjectStructure(1, program->projectStructure);
    } else {
        _generateProjectStructure(1, program->projectStructure1);
        _generateProgram(program->program);
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
        "df = pd.DataFrame([\n"
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
	_generateEpilogue(compilerState->value);
    closeFile();
	logDebugging(_logger, "Generation is done.");
}

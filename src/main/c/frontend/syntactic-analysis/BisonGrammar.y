%{

#include "BisonActions.h"

%}

%define api.value.union.name SemanticValue

%union {
	/** Terminals. */

	int integer;
	Token token;
	char *string;

	/** Non-terminals. */

	ProjectStructure * projectStructure;
	FormatType * formatType;
	ProjectOptionals * projectOptionals;
	ProjectDependencies * projectDependencies;
	ProjectUnion * projectUnion;
	ProjectDependenciesUnion * projectDependenciesUnion;
	ProjectBody * projectBody;
	TaskStructure * taskStructure;
	ProjectBodyOptionals * projectBodyOptionals;
	BodyMaxTasksOption * bodyMaxTasksOption;
	BodyCategoriesOption * bodyCategoriesOption;
	BodyMaxPointsOption * bodyMaxPointsOption;
	BodyProjectStart * bodyProjectStart;
	TaskList * taskList;
	TaskLengthFormat * taskLengthFormat;
	TaskLengthStartEnd * taskLengthStartEnd;
	TaskLengthInterval * taskLengthInterval;
	TaskOptionals * taskOptionals;
	TaskOptionCategories * taskOptionCategiores;
	TaskOptionPoints * taskOptionPoints;
	TaskOptionDependsOn * taskOptionDependsOn;
	TaskOptionDependsOnInternalTask * taskOptionDependsOnInternalTask;
	TaskOptionDependsOnExternalTask * taskOptionDependsOnExternalTask;
	Program * program;
}

/**
 * Destructors. This functions are executed after the parsing ends, so if the
 * AST must be used in the following phases of the compiler you shouldn't used
 * this approach.
 *
 * @see https://www.gnu.org/software/bison/manual/html_node/Destructor-Decl.html
 */
/*
%destructor { releaseConstant($$); } <constant>
%destructor { releaseExpression($$); } <expression>
%destructor { releaseFactor($$); } <factor>
%destructor { releaseProgram($$); } <program>
*/

/** Terminals. */
%token <integer> INTEGER
%token <string> NAME
%token <string> ID
%token <string> INTERVAL
%token <string> SPECIFIC_DATE

%token <token> PROJECT
%token <token> FORMAT
%token <token> DATE
%token <token> HOUR
%token <token> DAY
%token <token> WEEK
%token <token> MONTH
%token <token> OPEN_BRACKET
%token <token> CLOSE_BRACKET
%token <token> TASK
%token <token> LENGTH
%token <token> START
%token <token> FINISH
%token <token> CATEGORIES
%token <token> CATEGORY
%token <token> DEPENDS_ON
%token <token> DOT
%token <token> MAX_POINTS
%token <token> POINTS 
%token <token> UNIQUE 
%token <token> MAX_TASKS
%token <token> PROJECT_START 
%token <token> COMMA
%token <token> DEPENDS_ON_PROJECT
%token <token> WITH

%token <token> UNKNOWN

/** Non-terminals. */

%type <projectStructure> projectStructure
%type <formatType> formatType
%type <projectOptionals> projectOptionals
%type <projectDependencies> projectDependencies
%type <projectUnion> projectUnion
%type <projectDependenciesUnion> projectDependenciesUnion
%type <projectBody> projectBody
%type <taskStructure> taskStructure
%type <projectBodyOptionals> projectBodyOptionals
%type <bodyMaxTasksOption> bodyMaxTasksOption
%type <bodyCategoriesOption> bodyCategoriesOption
%type <bodyMaxPointsOption> bodyMaxPointsOption
%type <bodyProjectStart> bodyProjectStart
%type <taskList> taskList
%type <taskLengthFormat> taskLengthFormat
%type <taskLengthStartEnd> taskLengthStartEnd
%type <taskLengthInterval> taskLengthInterval
%type <taskOptionals> taskOptionals
%type <taskOptionCategiores> taskOptionCategiores
%type <taskOptionPoints> taskOptionPoints
%type <taskOptionDependsOn> taskOptionDependsOn
%type <taskOptionDependsOnInternalTask>  taskOptionDependsOnInternalTask
%type <taskOptionDependsOnExternalTask>  taskOptionDependsOnExternalTask
%type <program> program




/**
 * Precedence and associativity.
 *
 * @see https://www.gnu.org/software/bison/manual/html_node/Precedence.html
 */


%%

program: projectStructure
	| program projectStructure
	;

projectStructure: PROJECT ID NAME FORMAT formatType projectBody
	| PROJECT ID NAME FORMAT formatType projectOptionals projectBody
	;

formatType: HOUR
	| DAY
	| WEEK
	| MONTH
	| DATE
	;

projectOptionals: DEPENDS_ON_PROJECT ID
	| WITH ID projectUnion
	| DEPENDS_ON_PROJECT ID WITH ID projectUnion
	;

projectUnion: 
	| projectUnion COMMA ID
	;

projectBody: OPEN_BRACKET taskList CLOSE_BRACKET
	| OPEN_BRACKET projectBodyOptionals taskList CLOSE_BRACKET
	;

taskList: taskStructure
	| taskList taskStructure
	;

taskStructure: TASK ID NAME taskLengthFormat
	| TASK ID NAME taskLengthFormat taskOptionals
	;

taskLengthFormat: START SPECIFIC_DATE FINISH SPECIFIC_DATE
	| LENGTH INTERVAL
	;

taskOptionals: CATEGORY ID
	| POINTS INTEGER
	| DEPENDS_ON ID DOT ID taskOptionDependsOn
	| UNIQUE
	| CATEGORY ID POINTS INTEGER
	| CATEGORY ID DEPENDS_ON ID DOT ID taskOptionDependsOn
	| CATEGORY ID UNIQUE
	| POINTS INTEGER DEPENDS_ON ID DOT ID taskOptionDependsOn
	| POINTS INTEGER UNIQUE
	| DEPENDS_ON ID DOT ID taskOptionDependsOn UNIQUE
	| CATEGORY ID POINTS INTEGER DEPENDS_ON ID DOT ID taskOptionDependsOn
	| CATEGORY ID DEPENDS_ON ID DOT ID taskOptionDependsOn UNIQUE
	| POINTS INTEGER DEPENDS_ON ID DOT ID taskOptionDependsOn UNIQUE
	| CATEGORY ID POINTS INTEGER DEPENDS_ON ID DOT ID taskOptionDependsOn UNIQUE
	;

taskOptionDependsOn:
	| taskOptionDependsOn COMMA ID DOT ID
	;

projectBodyOptionals: MAX_TASKS INTEGER
	| CATEGORIES ID NAME bodyCategoriesOption
	| MAX_POINTS INTEGER
	| PROJECT_START SPECIFIC_DATE
	| MAX_TASKS INTEGER CATEGORIES ID NAME bodyCategoriesOption
	| MAX_TASKS INTEGER MAX_POINTS INTEGER
	| MAX_TASKS INTEGER PROJECT_START SPECIFIC_DATE
	| CATEGORIES ID NAME bodyCategoriesOption MAX_POINTS INTEGER
	| CATEGORIES ID NAME bodyCategoriesOption PROJECT_START SPECIFIC_DATE
	| MAX_POINTS INTEGER PROJECT_START SPECIFIC_DATE
	| MAX_TASKS INTEGER CATEGORIES ID NAME bodyCategoriesOption MAX_POINTS INTEGER
	| MAX_TASKS INTEGER MAX_POINTS INTEGER PROJECT_START SPECIFIC_DATE
	| CATEGORIES ID NAME bodyCategoriesOption MAX_POINTS INTEGER PROJECT_START SPECIFIC_DATE
	| MAX_TASKS INTEGER CATEGORIES ID NAME bodyCategoriesOption MAX_POINTS INTEGER PROJECT_START SPECIFIC_DATE
	;

bodyCategoriesOption:
	| COMMA ID NAME
	;


%%


/* program: expression													{ $$ = ExpressionProgramSemanticAction(currentCompilerState(), $1); }
	;

expression: expression[left] ADD expression[right]					{ $$ = ArithmeticExpressionSemanticAction($left, $right, ADDITION); }
	| expression[left] DIV expression[right]						{ $$ = ArithmeticExpressionSemanticAction($left, $right, DIVISION); }
	| expression[left] MUL expression[right]						{ $$ = ArithmeticExpressionSemanticAction($left, $right, MULTIPLICATION); }
	| expression[left] SUB expression[right]						{ $$ = ArithmeticExpressionSemanticAction($left, $right, SUBTRACTION); }
	| factor														{ $$ = FactorExpressionSemanticAction($1); }
	;

factor: OPEN_PARENTHESIS expression CLOSE_PARENTHESIS				{ $$ = ExpressionFactorSemanticAction($2); }
	| constant														{ $$ = ConstantFactorSemanticAction($1); }
	;

constant: INTEGER													{ $$ = IntegerConstantSemanticAction($1); }
	; */
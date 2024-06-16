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

	ProjectStart * projectStart;
	MaxPoints * maxPoints;
	CategoriesId * categoriesId;
	MaxTasks * maxTasks;
	CategoryId * categoryId;
	PointsInteger * pointsInteger;
	DependsOnId * dependsOnId;
	Unique * unique;
	TimeUnit * timeUnit;
	ProjectStructureCommon * projectStructureCommon;
	ProjectStructure * projectStructure;
	ProjectOptionals * projectOptionals;
	ProjectUnion * projectUnion;
	ProjectBody * projectBody;
	TaskStructure * taskStructure;
	ProjectBodyOptionals * projectBodyOptionals;
	BodyCategoriesOption * bodyCategoriesOption;
	TaskList * taskList;
	TaskLengthFormat * taskLengthFormat;
	TaskOptionals * taskOptionals;
	TaskOptionDependsOn * taskOptionDependsOn;
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
%token <integer> LEFT_INTERVAL
%token <integer> RIGHT_INTERVAL
%token <string> SPECIFIC_DATE

%token <token> PROJECT
%token <token> FORMAT
%token <token> DATE
%token <token> HOUR
%token <token> DAY
%token <token> WEEK
%token <token> MONTH
%token <token> OPEN_BRACES
%token <token> CLOSE_BRACES
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
%type <projectOptionals> projectOptionals
%type <projectUnion> projectUnion
%type <projectBody> projectBody
%type <taskStructure> taskStructure
%type <projectBodyOptionals> projectBodyOptionals
%type <bodyCategoriesOption> bodyCategoriesOption
%type <taskList> taskList
%type <taskLengthFormat> taskLengthFormat
%type <taskOptionals> taskOptionals
%type <taskOptionDependsOn> taskOptionDependsOn
%type <program> program
%type <projectStructureCommon> projectStructureCommon
%type <timeUnit> timeUnit
%type <categoryId> categoryId
%type <pointsInteger> pointsInteger
%type <dependsOnId> dependsOnId
%type <unique> unique
%type <projectStart> projectStart
%type <maxPoints> maxPoints
%type <categoriesId> categoriesId
%type <maxTasks> maxTasks




/**
 * Precedence and associativity.
 *
 * @see https://www.gnu.org/software/bison/manual/html_node/Precedence.html
 */


%%

program: projectStructure												{ $$ = ProjectStructureProgramSemanticAction(currentCompilerState(), $1); }
	| program projectStructure											{ $$ = ProjectStructureRecursiveProgramSemanticAction(currentCompilerState(), $1, $2); }
	;

projectStructure: projectStructureCommon projectOptionals projectBody   { $$ = OptionalsProjectStructureSemanticAction($1, $2, $3); } 
	;

projectStructureCommon: PROJECT ID NAME FORMAT timeUnit					{ $$ = ProjectStructureCommonSemanticAction($2, $3, $5); }
	;

timeUnit: HOUR															{ $$ = TimeUnitSemanticAction(HOUR_TYPE); }
	| DAY																{ $$ = TimeUnitSemanticAction(DAY_TYPE); }
	| WEEK																{ $$ = TimeUnitSemanticAction(WEEK_TYPE); }
	| MONTH																{ $$ = TimeUnitSemanticAction(MONTH_TYPE); }
	| DATE																{ $$ = TimeUnitSemanticAction(DATE_TYPE); }
	;

projectOptionals:														{ $$ = NULL; }
	| DEPENDS_ON_PROJECT ID												{ $$ = IdOptionalsSemanticAction($2); } 
	| WITH ID projectUnion												{ $$ = ProjectUnionOptionalsSemanticAction($2, $3); } 
	| DEPENDS_ON_PROJECT ID WITH ID projectUnion						{ $$ = BothOptionalsSemanticAction($2, $4, $5); } 
	;

projectUnion:															{ $$ = NULL; }
	| projectUnion COMMA ID												{ $$ = IdProjectUnionSemanticAction($1, $3); } 
	;

projectBody: OPEN_BRACES CLOSE_BRACES									{ $$ = NULL; }
	| OPEN_BRACES projectBodyOptionals taskList CLOSE_BRACES			{ $$ = OptionalsProjectBodySemanticAction($2, $3); }
	;

taskList: taskStructure													{ $$ = StructureListSemanticAction($1); }
	| taskList taskStructure											{ $$ = StructureRecursiveListSemanticAction($1, $2); }
	;

taskStructure: TASK ID NAME taskLengthFormat taskOptionals				{ $$ = OptionalsStructureSemanticAction($2, $3, $4, $5); }
	;

taskLengthFormat: START SPECIFIC_DATE FINISH SPECIFIC_DATE				{ $$ = DateLengthFormatSemanticAction($2, $4); }
	| LENGTH LEFT_INTERVAL RIGHT_INTERVAL								{ $$ = IntervalLengthFormatSemanticAction($2, $3); }
	| LENGTH LEFT_INTERVAL												{ $$ = IntervalLengthFormatSemanticAction($2, -1); }
	;

taskOptionals: categoryId pointsInteger dependsOnId unique				{ $$ = TaskOptionalSemanticAction($1, $2, $3, $4); }
	;

categoryId:																{ $$ = NULL; }
	| CATEGORY ID														{ $$ = CategoryIdSemanticAction($2); }
	;

pointsInteger:															{ $$ = NULL; }
	| POINTS INTEGER													{ $$ = PointsIntegerSemanticAction($2); }
	;

dependsOnId:															{ $$ = NULL; }
	| DEPENDS_ON ID DOT ID taskOptionDependsOn							{ $$ = DependsOnIdSemanticAction($2, $4, $5); }
	;

unique:																	{ $$ = NULL; }
	| UNIQUE															{ $$ = NULL; }
	;

taskOptionDependsOn:													{ $$ = NULL; }
	| taskOptionDependsOn COMMA ID DOT ID								{ $$ = TaskOptionDependsOnSemanticAction($1,$3,$5); }
	;

projectBodyOptionals: maxTasks categoriesId maxPoints projectStart 		{ $$ = ProjectBodyOptionalsSemanticAction($1, $2, $3, $4); }
	;

maxTasks:																{ $$ = NULL; }
	| MAX_TASKS INTEGER													{ $$ = MaxTasksSemanticAction($2); }	
	;

categoriesId:															{ $$ = NULL; }
	| CATEGORIES ID NAME bodyCategoriesOption							{ $$ = CategoriesIdSemanticAction($2,$3,$4); }	
	;	

maxPoints:																{ $$ = NULL; }
	| MAX_POINTS INTEGER												{ $$ = MaxPointsSemanticAction($2); }	
	;									

projectStart:															{ $$ = NULL; }
	| PROJECT_START SPECIFIC_DATE										{ $$ = ProjectStartSemanticAction($2); }	
	;	

bodyCategoriesOption: 													{ $$ = NULL; }
	| bodyCategoriesOption COMMA ID NAME 								{ $$ = RecursiveCategoriesOptionSemanticAction($1, $3, $4); }
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
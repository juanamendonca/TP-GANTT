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




/**
 * Precedence and associativity.
 *
 * @see https://www.gnu.org/software/bison/manual/html_node/Precedence.html
 */


%%

program: projectStructure												{ $$ = ProjectStructureProgramSemanticAction(currentCompilerState(), $1); }
	| program projectStructure											{ $$ = ProjectStructureRecursiveProgramSemanticAction($1, $2); }
	;

projectStructure: PROJECT ID NAME FORMAT HOUR projectBody			{ $$ = ProjectBodyProjectStructureSemanticAction($2, $3, $6); }
	| PROJECT ID NAME FORMAT HOUR projectOptionals projectBody      { $$ = OptionalsProjectStructureSemanticAction($2, $3, $6, $7); } 
	| PROJECT ID NAME FORMAT DAY projectBody						{ $$ = ProjectBodyProjectStructureSemanticAction($2, $3, $6); }
	| PROJECT ID NAME FORMAT DAY projectOptionals projectBody		{ $$ = OptionalsProjectStructureSemanticAction($2, $3, $6, $7); } 
	| PROJECT ID NAME FORMAT WEEK projectBody						{ $$ = ProjectBodyProjectStructureSemanticAction($2, $3, $6); }
	| PROJECT ID NAME FORMAT WEEK projectOptionals projectBody		{ $$ = OptionalsProjectStructureSemanticAction($2, $3, $6, $7); } 
	| PROJECT ID NAME FORMAT MONTH projectBody						{ $$ = ProjectBodyProjectStructureSemanticAction($2, $3, $6); }
	| PROJECT ID NAME FORMAT MONTH projectOptionals projectBody		{ $$ = OptionalsProjectStructureSemanticAction($2, $3, $6, $7); } 
	| PROJECT ID NAME FORMAT DATE projectBody						{ $$ = ProjectBodyProjectStructureSemanticAction($2, $3, $6); }
	| PROJECT ID NAME FORMAT DATE projectOptionals projectBody		{ $$ = OptionalsProjectStructureSemanticAction($2, $3, $6, $7); } 
	
	;

projectOptionals: DEPENDS_ON_PROJECT ID									{ $$ = IdOptionalsSemanticAction($2); } 
	| WITH ID projectUnion												{ $$ = ProjectUnionOptionalsSemanticAction($2, $3); } 
	| DEPENDS_ON_PROJECT ID WITH ID projectUnion						{ $$ = BothOptionalsSemanticAction($2, $4, $5); } 
	;

projectUnion:															{ $$ = NULL; }
	| projectUnion COMMA ID												{ $$ = IdProjectUnionSemanticAction($1, $3); } 
	;

projectBody: OPEN_BRACKET CLOSE_BRACKET									{ $$ = NULL; }
	| OPEN_BRACKET taskList CLOSE_BRACKET								{ $$ = taskListProjectBodySemanticAction($2); }
	| OPEN_BRACKET projectBodyOptionals taskList CLOSE_BRACKET			{ $$ = optionalsProjectBodySemanticAction($2, $3); }
	;

taskList: taskStructure													{ $$ = structureListSemanticAction($1); }
	| taskList taskStructure											{ $$ = structureRecursiveListSemanticAction($1, $2); }
	;

taskStructure: TASK ID NAME taskLengthFormat							{ $$ = lengthFormatStructureSemanticAction($2, $3, $4); }
	| TASK ID NAME taskLengthFormat taskOptionals						{ $$ = optionalsStructureSemanticAction($2, $3, $4, $5); }
	;

taskLengthFormat: START SPECIFIC_DATE FINISH SPECIFIC_DATE				{ $$ = dateLengthFormatSemanticAction($2, $4); }
	| LENGTH INTERVAL													{ $$ = intervalLengthFormatSemanticAction($2); }
	;

taskOptionals: CATEGORY ID												{ $$ = oneTaskOptionalsSemanticAction($2, CATEGORYTASK); }
	| POINTS INTEGER													{ $$ = twoTaskOptionalsSemanticAction($2, POINTSTASK); }
	| DEPENDS_ON ID DOT ID taskOptionDependsOn							{ $$ = threeTaskOptionalsSemanticAction($2, $4, $5, DEPENDS); }
	| UNIQUE															{ $$ = NULL; }
	| CATEGORY ID POINTS INTEGER										{ $$ = fourTaskOptionalsSemanticAction($2, $4, CATEGORY_POINTS); }
	| CATEGORY ID DEPENDS_ON ID DOT ID taskOptionDependsOn				{ $$ = fiveTaskOptionalsSemanticAction($2, $4, $6, $7, CATEGORY_DEPENDS); }
	| CATEGORY ID UNIQUE												{ $$ = oneTaskOptionalsSemanticAction($2, CATEGORY_UNIQUE); }
	| POINTS INTEGER DEPENDS_ON ID DOT ID taskOptionDependsOn			{ $$ = sixTaskOptionalsSemanticAction($2, $4, $6, $7, POINTS_DEPENDS); }
	| POINTS INTEGER UNIQUE												{ $$ = twoTaskOptionalsSemanticAction($2, POINTS_UNIQUE); }
	| DEPENDS_ON ID DOT ID taskOptionDependsOn UNIQUE					{ $$ = threeTaskOptionalsSemanticAction($2, $4, $5, DEPENDS_UNIQUE); }
	| CATEGORY ID POINTS INTEGER UNIQUE									{ $$ = fourTaskOptionalsSemanticAction($2, $4, CATEGORY_POINTS_UNIQUE); }
	| CATEGORY ID POINTS INTEGER DEPENDS_ON ID DOT ID taskOptionDependsOn	{ $$ = sevenTaskOptionalsSemanticAction($2, $4, $6, $8, $9,CATEGORY_POINTS_DEPENDS); }
	| CATEGORY ID DEPENDS_ON ID DOT ID taskOptionDependsOn UNIQUE		{ $$ = fiveTaskOptionalsSemanticAction($2, $4, $6, $7, CATEGORY_DEPENDS_UNIQUE); }
	| POINTS INTEGER DEPENDS_ON ID DOT ID taskOptionDependsOn UNIQUE	{ $$ = sixTaskOptionalsSemanticAction($2, $4, $6, $7, POINTS_DEPENDS_UNIQUE); }
	| CATEGORY ID POINTS INTEGER DEPENDS_ON ID DOT ID taskOptionDependsOn UNIQUE	{ $$ = sevenTaskOptionalsSemanticAction($2, $4, $6, $8, $9,CATEGORY_POINTS_DEPENDS_UNIQUE); }                             
	;

taskOptionDependsOn:																							{ $$ = NULL; }
	| taskOptionDependsOn COMMA ID DOT ID																		{ $$ = TaskOptionDependsOnSemanticAction($3,$5); }
	;

projectBodyOptionals: MAX_TASKS INTEGER																			{ $$ = OneBodyOptionalsSemanticAction($2); }	
	| CATEGORIES ID NAME bodyCategoriesOption																	{ $$ = TwoBodyOptionalsSemanticAction($2,$3,$4); }	
	| MAX_POINTS INTEGER																						{ $$ = ThreeBodyOptionalsSemanticAction($2); }	
	| PROJECT_START SPECIFIC_DATE																				{ $$ = FourBodyOptionalsSemanticAction($2); }	
	| MAX_TASKS INTEGER CATEGORIES ID NAME bodyCategoriesOption													{ $$ = FiveBodyOptionalsSemanticAction($2,$4,$5,$6); }	
	| MAX_TASKS INTEGER MAX_POINTS INTEGER																		{ $$ = SixBodyOptionalsSemanticAction($2,$4); }			
	| MAX_TASKS INTEGER PROJECT_START SPECIFIC_DATE																{ $$ = SevenBodyOptionalsSemanticAction($2,$4); }
	| CATEGORIES ID NAME bodyCategoriesOption MAX_POINTS INTEGER												{ $$ = EightBodyOptionalsSemanticAction($2,$3,$4,$5); }
	| CATEGORIES ID NAME bodyCategoriesOption PROJECT_START SPECIFIC_DATE										{ $$ = NineBodyOptionalsSemanticAction($2,$3,$4,$6); }
	| MAX_POINTS INTEGER PROJECT_START SPECIFIC_DATE															{ $$ = TenBodyOptionalsSemanticAction($2,$4); }
	| MAX_TASKS INTEGER CATEGORIES ID NAME bodyCategoriesOption MAX_POINTS INTEGER								{ $$ = ElevenBodyOptionalsSemanticAction($2,$4,$5,$6,$7); }
	| MAX_TASKS INTEGER MAX_POINTS INTEGER PROJECT_START SPECIFIC_DATE											{ $$ = TwelveBodyOptionalsSemanticAction($2,$4,$6); }
	| CATEGORIES ID NAME bodyCategoriesOption MAX_POINTS INTEGER PROJECT_START SPECIFIC_DATE 					{ $$ = ThirteenBodyOptionalsSemanticAction($2,$3,$4,$6,$8); }
	| MAX_TASKS INTEGER CATEGORIES ID NAME bodyCategoriesOption MAX_POINTS INTEGER PROJECT_START SPECIFIC_DATE  { $$ = FourteenBodyOptionalsSemanticAction($2,$4,$5,$6,$8,$10); }
	| MAX_TASKS INTEGER CATEGORIES ID NAME bodyCategoriesOption PROJECT_START SPECIFIC_DATE						{ $$ = FifthteenBodyOptionalsSemanticAction($2,$4,$5,$6,$8);}
	;
bodyCategoriesOption: 																							{ $$ = NULL; }
	| bodyCategoriesOption COMMA ID NAME 																		{ $$ = RecursiveCategoriesOptionSemanticAction($3, $4); }
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
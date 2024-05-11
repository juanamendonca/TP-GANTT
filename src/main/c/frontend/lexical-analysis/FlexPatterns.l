%{

#include "FlexActions.h"

%}

/**
 * Enable start condition manipulation functions.
 */
%option stack

/**
 * Flex contexts (a.k.a. start conditions).
 *
 * @see https://westes.github.io/flex/manual/Start-Conditions.html
 */
%x MULTILINE_COMMENT

/**
 * Reusable patterns.
 *
 * @see https://westes.github.io/flex/manual/Matching.html
 * @see https://westes.github.io/flex/manual/Patterns.html
 */

%%

"/*"								{ BEGIN(MULTILINE_COMMENT); BeginMultilineCommentLexemeAction(createLexicalAnalyzerContext()); }
<MULTILINE_COMMENT>"*/"				{ EndMultilineCommentLexemeAction(createLexicalAnalyzerContext()); BEGIN(INITIAL); }
<MULTILINE_COMMENT>[[:space:]]+		{ IgnoredLexemeAction(createLexicalAnalyzerContext()); }
<MULTILINE_COMMENT>[^*]+			{ IgnoredLexemeAction(createLexicalAnalyzerContext()); }
<MULTILINE_COMMENT>.				{ IgnoredLexemeAction(createLexicalAnalyzerContext()); }

[[:digit:]]+						{ return IntegerLexemeAction(createLexicalAnalyzerContext()); }



\"[[:alnum:]]+\"                    { return NameLexemeAction(createLexicalAnalyzerContext())}

[[:alnum:]]+                        { return IdLexemeAction(createLexicalAnalyzerContext())}

\[[[:digit:]]+"-"[[:digit:]]+\]|\[[[:digit:]]+\] {return IntervalLexemeAction(createLexicalAnalyzerContext())}

/* [[:digit:]]{4}"-"((0[1-9])|(1[0-2]))"-"((0[1-9])|([12][0-9])|(3[01])) {return } */
[[:digit:]]{4}\-((0[1-9])|(1[0-2]))\-((0[1-9])|([12][0-9])|(3[01])) {return SpecificDateLexemeAction(createLexicalAnalyzerContext())}


"project"                           { return ProjectLexemeAction(createLexicalAnalyzerContext())}
"format"                            { return FormatLexemeAction(createLexicalAnalyzerContext())}
"date"                              { return DateLexemeAction(createLexicalAnalyzerContext())}
"hour"                              { return HourLexemeAction(createLexicalAnalyzerContext())}                           
"day"                               { return DayLexemeAction(createLexicalAnalyzerContext())}
"week"                              { return WeekLexemeAction(createLexicalAnalyzerContext())}
"month"                             { return MonthLexemeAction(createLexicalAnalyzerContext())}
"task"                              { return TaskLexemeAction(createLexicalAnalyzerContext())}
"length"                            { return LengthLexemeAction(createLexicalAnalyzerContext())}
"start"                             { return StartLexemeAction(createLexicalAnalyzerContext())}
"finish"                            { return FinishLexemeAction(createLexicalAnalyzerContext())}
"categories"                        { return CategoriesLexemeAction(createLexicalAnalyzerContext())}
"category"                          { return CategoryLexemeAction(createLexicalAnalyzerContext())}
"depends_on"                        { return DependsOnLexemeAction(createLexicalAnalyzerContext())}
"depens_on_project"                 { return DependsOnProjectLexemeAction(createLexicalAnalyzerContext())}
"max_points"                        { return MaxPointsLexemeAction(createLexicalAnalyzerContext())}
"points"                            { return PointsLexemeAction(createLexicalAnalyzerContext())}
"unique"                            { return UniqueLexemeAction(createLexicalAnalyzerContext())}
"max_tasks"                         { return MaxTaskLexemeAction(createLexicalAnalyzerContext())}
"project_start"                     { return ProjectStartLexemeAction(createLexicalAnalyzerContext())}
"with"                              { return WithLexemeAction(createLexicalAnalyzerContext())}


","                                 { return CommaLexemeAction(createLexicalAnalyzerContext())}
"."                                 { return DotLexemeAction(createLexicalAnalyzerContext())}
"{"                                 { return BracketsLexemeAction(createLexicalAnalyzerContext())}
"}"                                 { return BracketsLexemeAction(createLexicalAnalyzerContext())}


.									{ return UnknownLexemeAction(createLexicalAnalyzerContext()); }






%%

#include "FlexExport.h"

/* "-"									{ return ArithmeticOperatorLexemeAction(createLexicalAnalyzerContext()); }
"*"									{ return ArithmeticOperatorLexemeAction(createLexicalAnalyzerContext()); }
"/"									{ return ArithmeticOperatorLexemeAction(createLexicalAnalyzerContext()); }
"+"									{ return ArithmeticOperatorLexemeAction(createLexicalAnalyzerContext()); }

"("									{ return ParenthesisLexemeAction(createLexicalAnalyzerContext()); }
")"									{ return ParenthesisLexemeAction(createLexicalAnalyzerContext()); }



[[:space:]]+						{ IgnoredLexemeAction(createLexicalAnalyzerContext()); } */

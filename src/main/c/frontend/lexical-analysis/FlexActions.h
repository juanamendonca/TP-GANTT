#ifndef FLEX_ACTIONS_HEADER
#define FLEX_ACTIONS_HEADER

#include "../../shared/Environment.h"
#include "../../shared/Logger.h"
#include "../../shared/String.h"
#include "../../shared/Type.h"
#include "../syntactic-analysis/AbstractSyntaxTree.h"
#include "../syntactic-analysis/BisonParser.h"
#include "LexicalAnalyzerContext.h"
#include <stdio.h>
#include <stdlib.h>

/** Initialize module's internal state. */
void initializeFlexActionsModule();

/** Shutdown module's internal state. */
void shutdownFlexActionsModule();

/**
 * Flex lexeme processing actions.
 */

void BeginMultilineCommentLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext);
void EndMultilineCommentLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext);
void IgnoredLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext);

Token IntegerLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext);
Token NameLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext);
Token IdLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext);
Token LeftIntervalLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext);
Token RightIntervalLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext);
Token SpecificDateLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext);

Token ProjectLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext);
Token FormatLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext);
Token DateLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext);
Token HourLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext);
Token DayLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext);
Token MonthLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext);
Token WeekLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext);
Token BracesLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext);
Token TaskLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext);
Token LengthLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext);
Token StartLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext);
Token FinishLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext);
Token CategoriesLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext);
Token CategoryLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext);
Token DependsOnLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext);
Token DependsOnProjectLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext);
Token MaxPointsLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext);
Token PointsLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext);
Token UniqueLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext);
Token MaxTasksLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext);
Token WithLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext);
Token ProjectStartLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext);
Token CommaLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext);
Token DotLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext);




Token ArithmeticOperatorLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext);
Token ParenthesisLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext);

Token UnknownLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext);

#endif

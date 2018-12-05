/**
 * @file expression.h
 * @author
 * @date 24.11. 2018
 * @brief
 */
#ifndef EXPRESSION_H
#define EXPRESSION_H

#include <stdbool.h>
#include <string.h>
#include "stack.h"
#include "token_stack.h"
#include "ass.h"

#define PRECEDENT_TABLE_SIZE 14

typedef enum {
  E = 100,  // =
  R = 101,  // >
  S = 102,  // <
  N = 103   // NONE
} p_table_sign;

typedef enum {
  P_PLUS,           // 0
  P_MINUS,          // 1
  P_MULTIPLY,       // 2
  P_DIVIDE,         // 3
  P_LOWER,          // 4
  P_LOWEREQ,        // 5
  P_GREATER,        // 6
  P_GREATEREQ,      // 7
  P_EQ,             // 8
  P_NOTEQ,          // 9
  P_LEFT_BRACKET,   // 10
  P_RIGHT_BRACKET,  // 11
  P_ID,             // 12
  P_BOTTOM,         // 13
  P_E               // 14 - non-terminal, expression
} p_table_terms;

#define NUMBER_OF_RULES 12
#define MAX_RULE_LENGHT 3

/**
@brief Find first terminal closest to the top of the stack
*/
int stack_terminal_top(tStack *searched_stack, tStack *aux_stack);
int prec_table(tToken *token, tSymPtr sym, bool isGlobal);
int evaluate_rule (tStack *stack_temp, tStack *stack_pushdown, tStack *stack_rules);

#endif

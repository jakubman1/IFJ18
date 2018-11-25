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
#include "lib/stack.h"
#include "lib/token_stack.h"
#include "lib/ass.h"

#define PRECEDENT_TABLE_SIZE 14

typedef enum {
  E, // =
  R, // >
  S, // <
  N // NONE
} p_table_sign;

static int precedent_table[PRECEDENT_TABLE_SIZE][PRECEDENT_TABLE_SIZE] =
{
  //+ |- |* |/ |< |<=|> |>=|==|!=|( |) |i |$ |
   {R, R, S, S, R, R, R, R, R, R, S, R, S, R}, // +
   {R, R, S, S, R, R, R, R, R, R, S, R, S, R}, // -
   {R, R, R, R, R, R, R, R, R, R, S, R, S, R}, // *
   {R, R, R, R, R, R, R, R, R, R, S, R, S, R}, // /
   {S, S, S, S, N, N, N, N, R, R, S, R, S, R}, // <
   {S, S, S, S, N, N, N, N, R, R, S, R, S, R}, // <=
   {S, S, S, S, N, N, N, N, R, R, S, R, S, R}, // >
   {S, S, S, S, N, N, N, N, R, R, S, R, S, R}, // >=
   {S, S, S, S, S, S, S, S, N, N, S, R, S, R}, // ==
   {S, S, S, S, S, S, S, S, N, N, S, R, S, R}, // !=
   {S, S, S, S, S, S, S, S, S, S, S, E, S, N}, // (
   {R, R, R, R, R, R, R, R, R, R, N, R, N, R}, // )
   {R, R, R, R, R, R, R, R, R, R, N, R, N, R}, // i
   {S, S, S, S, S, S, S, S, S, S, S, N, S, N}  // $
};


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

static int rule_table[NUMBER_OF_RULES][MAX_RULE_LENGHT] =
{
  {P_E, P_PLUS, P_E},                       // E → E + E
  {P_E, P_MINUS, P_E},                      // E → E - E
  {P_E, P_MULTIPLY, P_E},                   // E → E * E
  {P_E, P_DIVIDE, P_E},                     // E → E / E
  {P_E, P_LOWER, P_E},                      // E → E < E
  {P_E, P_LOWEREQ, P_E},                    // E → E <= E
  {P_E, P_GREATER, P_E},                    // E → E > E
  {P_E, P_GREATEREQ, P_E},                  // E → E >= E
  {P_E, P_EQ, P_E},                         // E → E == E
  {P_E, P_NOTEQ, P_E},                      // E → E != E
  {P_LEFT_BRACKET, P_E, P_RIGHT_BRACKET},   // E → (E)
  {P_ID, -1, -1}                            // E → i
};

/**
@brief Find first terminal closest to the top of the stack
*/
int stack_terminal_top(tStack *searched_stack, tStack *aux_stack);
void furfil_stack(tStack *searched_stack, tStack *aux_stack);
int prec_table(tToken *token);


#endif

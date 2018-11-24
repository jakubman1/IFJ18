/**
 * @file expression.h
 * @author
 * @date 24.11. 2018
 * @brief
 */
 #ifndef EXPRESION_H
 #define EXPRESION_H

 #include <stdbool.h>
 #include <string.h>
 #include "lib/stack.h"

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
  P_BOTTOM          // 13
} p_table_terms;

/*
typedef enum {
  E_PLUS_E,             // E → E + E
  E_MINUS_E,            // E → E - E
  E_MULTIPLY_E,         // E → E * E
  E_DIVIDE_E,           // E → E / E
  BRACKET_E_BRACKET,    // E → (E)
  ID,                   // E → i
  INT,                  // E → int
  FLOAT,                // E → float
  STRING,               // E → string
  E_LOWER_E,            // E → E < E
  E_LOWEREQ_E,          // E → E <= E
  E_GREATER_E,          // E → E > E
  E_QREATEREQ_E,        // E → E >= E
  E_EQ_E,               // E → E == E
  E_NOTEQ_E             // E → E != E
  EXP,                  // E
} expression_rules;*/

/* #define PREC_RULE_01 s_pop(stack); s_push(stack, ); s_push(stack, ); s_push(stack, ) */

#define PREC_RULE_01 s_pop(stack); s_push(stack, ); s_push(stack, ); s_push(stack, )
#define PREC_RULE_01 s_pop(stack); s_push(stack, ); s_push(stack, ); s_push(stack, )
#define PREC_RULE_01 s_pop(stack); s_push(stack, ); s_push(stack, ); s_push(stack, )
#define PREC_RULE_01 s_pop(stack); s_push(stack, ); s_push(stack, ); s_push(stack, )
#define PREC_RULE_01 s_pop(stack); s_push(stack, ); s_push(stack, ); s_push(stack, )
#define PREC_RULE_01 s_pop(stack); s_push(stack, ); s_push(stack, ); s_push(stack, )
#define PREC_RULE_01 s_pop(stack); s_push(stack, ); s_push(stack, ); s_push(stack, )
#define PREC_RULE_01 s_pop(stack); s_push(stack, ); s_push(stack, ); s_push(stack, )
#define PREC_RULE_01 s_pop(stack); s_push(stack, ); s_push(stack, ); s_push(stack, )
#define PREC_RULE_01 s_pop(stack); s_push(stack, ); s_push(stack, ); s_push(stack, )
#define PREC_RULE_01 s_pop(stack); s_push(stack, ); s_push(stack, ); s_push(stack, )
#define PREC_RULE_01 s_pop(stack); s_push(stack, ); s_push(stack, ); s_push(stack, )

#endif

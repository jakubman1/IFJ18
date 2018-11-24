/**
 * @file expression.h
 * @author
 * @date 24.11. 2018
 * @brief
 */
 #ifndef EXPRESION_H
 #define EXPRESION_H

 #define TABLE_S 9

typedef enum {
  E, // =
  R, // >
  S, // <
  N // NONE
} p_table_sign;

static int precedent_table[TABLE_S][TABLE_S] =
{
  //+-|*/| r| e| (| )| i| $|
   {R, S, R, R, S, R, S, R}, // +-
   {R, R, R, R, S, R, S, R}, // */
   {S, S, N, R, S, R, S, R}, // r
   {S, S, S, N, S, R, S, R}, // e
   {S, S, S, S, S, E, S, N}, // (
   {R, R, R, R, N, R, N, R}, // )
   {R, R, R, R, N, R, N, R}, // i
   {S, S, S, S, S, N, S, N}  // $
};

/*
E → E + E
E → E - E
E → E * E
E → E / E
E → (E)
E → i
E → int
E → float
E → string
E → E < E
E → E <= E
E → E > E
E → E >= E
E → E == E
E → E != E
*/

typedef enum {
  EXP,
  E_PLUS_E,
  E_MINUS_E,
  E_MULTIPLY_E,
  E_DIVIDE_E,
  BRACKET_E_BRACKET,
  ID,
  INT,
  FLOAT,
  STRING,
  E_LOWER_E,
  E_LOWEREQ_E,
  E_GREATER_E,
  E_QREATEREQ_E,
  E_EQ_E,
  E_NOTEQ_E
} expression_rules;

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

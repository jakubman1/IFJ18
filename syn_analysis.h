/**
 * @file syn_analysis.h
 * @author Jiri Tykva
 * @author Jakub Man
 * @author Adam Melichar
 * @author Jan Martinák
 * @date 19.11. 2018
 * @brief Syntactic analysis header file
 */

#ifndef SYN_ANALYSYS_H
#define SYN_ANALYSYS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h> // strchr
#include "lib/errcodes.h"
#include "lib/types.h"
#include "lib/stack.h"
#include "scanner.h"
#include "symtable.h"
#include "expression.h"
#include "single_link_list.h"


#define PUSH_START s_push(stack, LL_BOTTOM); s_push(stack, LL_PROG)

#define PUSH_RULE_1 s_pop(stack); s_push(stack, LL_PROG); s_push(stack, LL_EOL); s_push(stack, LL_END); s_push(stack, LL_STATEMENT_N); s_push(stack, LL_EOL); s_push(stack, LL_BRACKET_RIGHT); s_push(stack, LL_PARAMS); s_push(stack, LL_BRACKET_LEFT); s_push(stack, LL_ID); s_push(stack, LL_DEF)
#define PUSH_RULE_2 s_pop(stack); s_push(stack, LL_PROG); s_push(stack, LL_EOL); s_push(stack, LL_STATEMENT)
#define PUSH_RULE_3 s_pop(stack)
#define PUSH_RULE_4 s_pop(stack); s_push(stack, LL_PARAMS_N); s_push(stack, LL_ID)
#define PUSH_RULE_5 s_pop(stack)
#define PUSH_RULE_6 s_pop(stack); s_push(stack, LL_PARAMS_N); s_push(stack, LL_ID); s_push(stack, LL_COMMA)
#define PUSH_RULE_7 s_pop(stack)
#define PUSH_RULE_8 s_pop(stack); s_push(stack, LL_END); s_push(stack, LL_STATEMENT_N); s_push(stack, LL_EOL); s_push(stack, LL_ELSE); s_push(stack, LL_STATEMENT_N); s_push(stack, LL_EOL); s_push(stack, LL_THEN); s_push(stack, LL_EXPRESSION); s_push(stack, LL_IF)
#define PUSH_RULE_9 s_pop(stack); s_push(stack, LL_END); s_push(stack, LL_STATEMENT_N); s_push(stack, LL_EOL); s_push(stack, LL_DO); s_push(stack, LL_EXPRESSION); s_push(stack, LL_WHILE)
#define PUSH_RULE_10 s_pop(stack); s_push(stack, LL_DEF_ARGS); s_push(stack, LL_ID)
#define PUSH_RULE_11 s_pop(stack)
#define PUSH_RULE_12 s_pop(stack); s_push(stack, LL_PROG); s_push(stack, LL_EOL); s_push(stack, LL_STATEMENT)
#define PUSH_RULE_13 s_pop(stack)
#define PUSH_RULE_14 s_pop(stack); s_push(stack, LL_DEFINE); s_push(stack, LL_SET)
#define PUSH_RULE_15 s_pop(stack); s_push(stack, LL_ARGS)
#define PUSH_RULE_16 s_pop(stack); s_push(stack, LL_ARGS); s_push(stack, LL_ID)
#define PUSH_RULE_17 s_pop(stack); s_push(stack, LL_EXPRESSION)
#define PUSH_RULE_18 s_pop(stack)
#define PUSH_RULE_19 s_pop(stack)
#define PUSH_RULE_20 s_pop(stack); s_push(stack, LL_BRACKET_RIGHT); s_push(stack, LL_ARGS); s_push(stack, LL_BRACKET_LEFT)
#define PUSH_RULE_21 s_pop(stack); s_push(stack, LL_ARGS_N); s_push(stack, LL_VALUE)
#define PUSH_RULE_22 s_pop(stack); s_push(stack, LL_ARGS_N); s_push(stack, LL_VALUE); s_push(stack, LL_COMMA)
#define PUSH_RULE_23 s_pop(stack)
#define PUSH_RULE_24 s_pop(stack); s_push(stack, LL_INT)
#define PUSH_RULE_25 s_pop(stack); s_push(stack, LL_FLOAT)
#define PUSH_RULE_26 s_pop(stack); s_push(stack, LL_ID)
#define PUSH_RULE_27 s_pop(stack); s_push(stack, LL_STRING)


 /**
  * @brief Main function of parser
  */
 int parser();

 int ll_predict(tToken *token, tStack *stack, tSymPtr *globalTree);
 int add_to_symtable(tToken *token, tToken *prev_token, tSymPtr *globalTree);
 int create_local_symtable(tList *symtable_list, tToken *token);
 int fill_symtable (tSymPtr *symtable_ptr, tToken *token);
 int insert_built_in_functions (tSymPtr *root);

 #endif

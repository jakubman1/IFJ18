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
#include "errcodes.h"
#include "types.h"
#include "stack.h"
#include "scanner.h"
#include "symtable.h"
#include "expression.h"
#include "single_link_list.h"
#include "generator.h"

#define BUILT_IN_FUNCTION (strcmp(id_func_name, "print") == 0 || strcmp(id_func_name, "length") == 0 || strcmp(id_func_name, "substr") == 0 || strcmp(id_func_name, "ord") == 0 || strcmp(id_func_name, "chr") == 0)

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
#define PUSH_RULE_28 s_pop(stack); s_push(stack, LL_NIL)


 /**
  * @brief Main function of parser
  */
 int parser();

int ll_predict(tToken *token, tStack *stack, tList *symtable_list, bool isGlobal, char *id_name, char *id_func_name);

 /**
 Add built in functions into the symtable
 @param root Symtable where the functions will be inserted
 @return 0 on success, 99 on memory allocation errors.
 */
 int insert_built_in_functions (tSymPtr *root);

 /**
 Inserts unknown variables or modifies already existing ones. Auxiliary function for add_to_symtable. Function has no effect for tokens that shouldn't stored be in symtables.
 @param symtable_list List of symtables
 @param token Current token
 @return 0 on success, VARIABLE_ERR if "<id>! = ..." or "<id>? = ..." or if name is a function, 99 on memory allocation errors.
 */
int fill_global_symtable (tList *symtable_list, tToken *token);

 /**
 Inserts unknown variables or modifies already existing ones. Auxiliary function for add_to_symtable. Function has no effect for tokens that shouldn't stored be in symtables.
 @param symtable_list List of symtables
 @param token Current token
 @param isParam Whether is a param of defined function or param inside the body of defined function
 @return 0 on success, VARIABLE_ERR if "<id>! = ..." or "<id>? = ..." or if name is a function, 99 on memory allocation errors.
 */
int fill_local_symtable (tList *symtable_list, tToken *token, bool isParam);

 /**
 Creates symtables if needed and fills them with data obtained from tokens. Function has no effect for tokens that shouldn't be stored in symtables.
 @param symtable_list List of symtables
 @param token Current token
 @return 0 on success, VARIABLE_ERR if "<id>! = ..." or "<id>? = ..." or if name is a function (when inserting variable) or if name is a variable (when inserting function), 99 on memory allocation errors.
 @pre Head of the list should be global symtable
 */
 int add_to_symtable(tList *symtable_list, tToken *token);

 #endif

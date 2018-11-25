/**
 * @file stack.h
 * @author Jakub Man
 * @date 22.11. 2018
 * @brief Token stack library
 */

#ifndef TOKEN_STACK_H
#define TOKEN_STACK_H
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include "types.h"
#include "errcodes.h"

typedef struct tokenStackNode {
  tToken data;
  struct tokenStackNode *next;
} tTStackNode;

typedef struct tokenStack {
  tTStackNode *head;
} tTStack;

// USAGE:
// tToken token = st_pop(&stack);
// st_push(&stack, *token);

void st_init(tTStack *stack);
void st_push(tTStack *stack, tToken data);
tToken st_pop(tTStack *stack);
tToken st_top(tTStack *stack);
bool st_empty(tTStack *stack);
void st_free(tTStack *stack);

#endif

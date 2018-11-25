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

typedef struct stack {
  tTStackNode *head;
} tTStack;

// USAGE:
// tToken token = st_pop(&stack);
// st_push(&stack, *token);

void st_init(tPStack *stack);
void st_push(tPStack *stack, tToken data);
void *st_pop(tPStack *stack);
void *st_top(tPStack *stack);
bool st_empty(tPStack *stack);
int st_free(tPStack *stack);

#endif

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

typedef struct pointerStackNode {
  void *data;
  struct pointerStackNode *next;
} tPStackNode;

typedef struct stack {
  tStackNode *head;
} tPStack;

// USAGE:
// tToken *token = (tToken *) sp_pop(&stack)

void sp_init(tPStack *stack);
void sp_push(tPStack *stack, void *data);
void *sp_pop(tPStack *stack);
void *sp_top(tPStack *stack);
bool sp_empty(tPStack *stack);
int sp_free(tPStack *stack);

#endif

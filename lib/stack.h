/**
 * @file stack.h
 * @author Jakub Man
 * @date 22.11. 2018
 * @brief Stack library
 */

#ifndef STACK_H
#define STACK_H
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include "types.h"
#include "errcodes.h"

typedef struct stackNode {
  int data;
  struct stackNode *next;
} tStackNode;

typedef struct stack {
  tStackNode *head;
} tStack;

void s_init(tStack *stack);
void s_push(tStack *stack, int data);
int s_pop(tStack *stack);
int s_top(tStack *stack);
bool s_empty(tStack *stack);

#endif

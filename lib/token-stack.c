#include "token-stack.h"

void st_init(tTStack *stack)
{
  stack->head = NULL;
}

void st_push(tTStack *stack, tToken data)
{
  tTStackNode *new = malloc(sizeof(tTStackNode));
  if(new != NULL) {
    new->data = data;
    new->next = stack->head;
    stack->head = new;
  }
  else {
    fprintf(stderr, "Internal Error: No memory left.\n");
    exit(INTERNAL_ERR);
  }

}

tToken st_pop(tTStack *stack)
{
  if(!st_empty(stack)) {
    void *data = stack->head->data;
    tTStackNode *tmp = stack->head;
    stack->head = stack->head->next;
    free(tmp);
    return data;
  }
  else {
    tToken empty = {"", ERROR};
    return empty;
  }
}

tToken st_top(tTStack *stack)
{
  if(!st_empty(stack)) {
    return stack->head->data;
  }
  else {
    tToken empty = {"", ERROR};
    return empty;
  }

}

bool s_empty(tTStack *stack)
{
  return stack->head == NULL;
}

void s_free(tTStack *stack)
{
  tTStackNode *tmp;
  for(tTStackNode *current = stack->head; current != NULL; current = tmp) {
    tmp = current->next;
    free(current);
    current = NULL;
  }
}

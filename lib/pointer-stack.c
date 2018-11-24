#include "pointer-stack.h"

void s_init(tPStack *stack)
{
  stack->head = NULL;
}

void s_push(tPStack *stack, tToken *data)
{
  tPStackNode *new = malloc(sizeof(tPStackNode));
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

void *s_pop(tPStack *stack)
{
  if(!s_empty(stack)) {
    void *data = stack->head->data;
    tPStackNode *tmp = stack->head;
    stack->head = stack->head->next;
    free(tmp);
    return data;
  }
  else {
    return -1;
  }
}

void *s_top(tPStack *stack)
{
  if(!s_empty(stack)) {
    return stack->head->data;
  }
  else {
    return NULL;
  }

}

bool s_empty(tPStack *stack)
{
  return stack->head == NULL;
}

void s_free(tPStack *stack)
{
  tPStackNode *tmp;
  for(tPStackNode *current = stack->head; current != NULL; current = tmp) {
    tmp = current->next;
    free(current);
    current = NULL;
  }
}

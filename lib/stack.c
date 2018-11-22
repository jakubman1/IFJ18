#include "stack.h"

void s_init(tStack *stack)
{
  stack->head = NULL;
}

void s_push(tStack *stack, int data)
{
  tStackNode *new = malloc(sizeof(tStackNode));
  if(new != NULL) {
    new->data = data;
    new->next = stack->head;
    stack->head = new;
  }
  else {
    fprintf(stderr, "Internal Error: No memory left.\n");
    exit(99);
  }

}

int s_pop(tStack *stack)
{
  if(!s_empty(stack)) {
    int data = stack->head->data;
    tStackNode *tmp = stack->head;
    stack->head = stack->head->next;
    free(tmp);
    return data;
  }
  else {
    return -1;
  }
}

int s_top(tStack *stack)
{
  if(!s_empty(stack)) {
    return stack->head->data;
  }
  else {
    return -1;
  }

}

bool s_empty(tStack *stack)
{
  return stack->head == NULL;
}

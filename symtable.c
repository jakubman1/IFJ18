/**
 * @file symtable.c
 * @author Jakub Man
 * @date 24.11. 2018
 * @brief Data types and functions for symbol table.
 */


#include "symtable.h"

void symtable_init(tSymPtr *root)
{
  if(root != NULL) {
    *root = NULL;
  }
}

int symtable_insert_unknown(tSymPtr *root, char *name)
{
  tSymPtr new = NULL;
  symtable_search(*root, name, &new);

  if(new == NULL)
  {
    //fprintf(stderr, "Allocating space for a new node. \n");
    new = malloc(sizeof(struct symNode));
    //fprintf(stderr, "Allocated. \n");
    if(new != NULL)
    {
      int len = strlen(name) + 1;
      new->name = malloc(sizeof(char) * len);
      //fprintf(stderr, "Allocating %d chars for string %s\n", len, name );
      if(new->name != NULL)
      {
        strcpy(new->name, name);
      }
      else
      {
        free(new);
        return INTERNAL_ERR;
      }
      new->type = UNKNOWN;
      new->lptr = NULL;
      new->rptr = NULL;
      symtable_insert(root, new);
    }
    else
    {
      return INTERNAL_ERR;
    }
  }
  return SUCCESS;
}

int symtable_insert_variable(tSymPtr *root, char *name, data_type type, bool define)
{

  tSymPtr new = NULL;
  symtable_search(*root, name, &new);
  if(new == NULL) {
    // Create a new item in the tree
    new = malloc(sizeof(struct symNode));
    if(new != NULL) {
      new->name = malloc(sizeof(char) * (strlen(name) + 1));
      if(new->name != NULL) {
        strcpy(new->name, name);
      }
      else {
        free(new);
        return INTERNAL_ERR;
      }
      new->type = VARIABLE;
      new->data.varData.defined = define;
      new->lptr = NULL;
      new->rptr = NULL;
      symtable_insert(root, new);
    }
    else {
      return INTERNAL_ERR;
    }

  }
  else {
    // Check if symbol is a variable, otherwise its a semantic error
    if(new->type == FUNCTION ) {
      return VARIABLE_ERR;
    }
    if(new->type == UNKNOWN) {
      new->type = VARIABLE;
    }
  }
  // Update existing (or newly created) item
  new->data.varData.type = type;
  if(define) {
    new->data.varData.defined = true;
  }

  return SUCCESS;
}

int symtable_insert_function(tSymPtr *root, char *name, data_type returnType, int paramCount, tFuncParam *params, bool define)
{
  tSymPtr new = NULL;
  symtable_search(*root, name, &new);
  if(new == NULL) {
    // Create a new item in the tree
    new = malloc(sizeof(struct symNode));
    if(new != NULL) {
      new->name = malloc(sizeof(char) * (strlen(name) + 1));
      if(new->name != NULL) {
        strcpy(new->name, name);
      }
      else {
        free(new);
        return INTERNAL_ERR;
      }
      new->type = FUNCTION;
      new->data.funData.defined = define;
      new->lptr = NULL;
      new->rptr = NULL;
      symtable_insert(root, new);
    }
    else {
      return INTERNAL_ERR;
    }
  }
  else {
    // Check if symbol is a function, otherwise its a semantic error
    if(new->type == VARIABLE) {
      return VARIABLE_ERR;
    }
    if(new->type == UNKNOWN) {
      new->type = FUNCTION;
    }
  }
  // Update existing (or newly created) item
  if(define) {
    new->data.funData.defined = true;
  }
  new->data.funData.returnType = returnType;
  new->data.funData.paramCount = paramCount;
  new->data.funData.params = params;

  return SUCCESS;
}

void symtable_insert(tSymPtr *root, tSymPtr node)
{
  int cmpval = 0;
  if(root != NULL) {
    if(*root == NULL) {
      // Insert into root
      *root = node;
    }
    else {
      // cmpval should never be 0, when strcmp returns 0, strings are equal.
      // This situation is handled by symtable_insert_function() or symtable_insert_variable()
      if(node->name != NULL) {
        cmpval = strcmp((*root)->name, node->name);
        if(cmpval > 0) {
          symtable_insert(&(((*root)->lptr)), node);
        }
        else if(cmpval < 0) {
          symtable_insert(&(((*root)->rptr)), node);
        }
      }
      else {
        fprintf(stderr, "Node name was NULL!\n");
      }
    }
  }
}

tSymPtr symtable_search(tSymPtr root, char *name, tSymPtr *result)
{
  int cmpval = 0;
  if(root != NULL && root->name != NULL) {
    cmpval = strcmp(root->name, name);
    if(cmpval == 0) {
      *result = root;
      //return root;
    }
    else if(cmpval > 0) {
      symtable_search(root->lptr, name, result);
    }
    else {
      symtable_search(root->rptr, name, result);
    }
  }
  // Not found
  return NULL;
}

void symtable_clear(tSymPtr *root)
{
  if(root != NULL && *root != NULL) {
    symtable_clear(&((*root)->lptr)); // Remove left subtree
    symtable_clear(&((*root)->rptr)); // Remove right subtree
    if((*root)->name != NULL) {
      free((*root)->name);
      (*root)->name = NULL;
    }
    if((*root)->type == FUNCTION) {
      // We need to free param list
      tFuncParam *tmp;
      for(tFuncParam *current = (*root)->data.funData.params; current != NULL; current = tmp) {
        tmp = current->next;
        free(current);
      }
    }
    free(*root); // Remove current node
    *root = NULL;
  }
}

int add_param(tFuncParam **head, data_type type, char *name)
{
  tFuncParam *new = NULL;

  if (*head == NULL) {
    new = malloc(sizeof(tFuncParam));
    if (new == NULL) {
      return INTERNAL_ERR;
    }
    new->type = type;
    new->name = malloc((strlen(name) + 1) * sizeof(char));
    if(new->name != NULL) {
      strcpy(new->name, name);
    }
    else {
      free(new);
      return INTERNAL_ERR;
    }
    new->next = NULL;

    *head = new;
  }
  else {
    tFuncParam *last = *head;
    while (last->next != NULL) {
      last = last->next;
    }

    new = malloc(sizeof(tFuncParam));
    if (new == NULL) {
      return INTERNAL_ERR;
    }
    last->next = new;
    new->type = type;
    new->next = NULL;

    new->name = malloc((strlen(name) + 1) * sizeof(char));
    if(new->name != NULL) {
      strcpy(new->name, name);
    }
    else {
      free(new);
      return INTERNAL_ERR;
    }

  }
  return SUCCESS;
}

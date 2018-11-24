/**
 * @file ass.h
 * @author Jakub Man
 * @date 22.11. 2018
 * @brief Abstract syntax tree library (A.S.S. -> abstraktni syntakticky strom)
 */

#ifndef ASS_H
#define ASS_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "errcodes.h"

typedef struct assNode {
  int type; // Type of node (for internal use only), remove?
  char *text; // Token text (NULL for non-terminals, token text for terminals)
  struct assNode *lptr;
  struct assNode *rptr;
  struct assNode *fptr;
} *tAssPtr; // Node for abstract syntax tree

/**
@brief Initialize a new abstract syntax tree.
@param tree Pointer to a tree variable.
*/
void ass_init(tAssPtr *root);
/**
@brief Insert a new node into abstract syntax tree.
@returns 0 on success, 99 if memory allocation failed.
*/
int ass_insert(tAssPtr *root, int type, char *text);
/**
@brief Remove tree and free all allocated resources.
@param tree Pointer to a tree that should be removed. After this function, tree will be empty.
*/
void ass_empty(tAssPtr *root);

tAssPtr ass_make_tree(); //TODO

tAssPtr ass_make_leaf(); //TODO


#endif

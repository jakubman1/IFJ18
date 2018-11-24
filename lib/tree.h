/**
 * @file tree.h
 * @author Jakub Man
 * @date 22.11. 2018
 * @brief Binary tree library
 */

#ifndef TREE_H
#define TREE_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "errcodes.h"

typedef struct der_tree_node {
  int type; // Type of node
  char *text; // Token text (NULL for non-terminals, token text for terminals)
  struct der_tree_node *lptr;
  struct der_tree_node *rptr;
} *tDerTreePtr; // Node for derivation tree

/**
@brief Initialize a new derivation tree.
@param tree Pointer to a tree variable.
*/
void dtree_init(tDerTreePtr *root);
/**
@brief Insert a new node into derivation tree.
@returns 0 on success, 99 if memory allocation failed.
*/
int dtree_insert(tDerTreePtr root, int type, char *text);
/**
@brief Remove tree and free all allocated resources.
@param tree Pointer to a tree that should be removed. After this function, tree will be empty.
*/
void dtree_empty(tDerTreePtr *root);



#endif

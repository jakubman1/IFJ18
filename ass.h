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
#include "symtable.h"
#include "types.h"

typedef struct assNode {
  int type; // Type of node (for internal use only)
  char *text; // Token text (NULL for non-terminals, token text for terminals)
  tSymPtr symtable; // Pointer to a symtable.
  struct assNode *lptr;
  struct assNode *rptr;
} *tAssPtr; // Node for abstract syntax tree

/**
@brief Initialize a new abstract syntax tree.
@param tree Pointer to a tree variable.
*/
void ass_init(tAssPtr *root);

/**
@brief Insert a new node into abstract syntax tree.
@returns 0 on success, 99 if memory allocation failed.
@note currently not implemented nor used.
*/
// int ass_insert(tAssPtr *root, int type, char *text);

/**
@brief Remove tree and free all allocated resources.
@param tree Pointer to a tree that should be removed. After this function, tree will be empty.
*/
void ass_empty(tAssPtr *root);

/**
Create a new element from type and text, add children to it.
@param type Type of a new node
@param text Text of a new node
@param symtable Pointer to the symtable for a new node. Can be NULL, if node is a operator.
@param left Left child of a new node
@param right Right child of a new node
@returns Pointer to the new node or NULL if memory allocation failed
*/
tAssPtr ass_make_tree(int type, char *text, tSymPtr symtable, tAssPtr left, tAssPtr right);

/**
Create a new leaf node
@param type Type of node
@param text Text contained in a node
@param symtable Pointer to the symtable for a new node.
@returns Pointer to the new node or NULL if memory allocation failed
*/
tAssPtr ass_make_leaf(int type, char *text, tSymPtr symtable);

/**
Find a father of a node
@param root Root of the Ass
@param current_node Child of the searched Father
@param father_node Variable where the father node is stored when found. If tree has only 1 node the root is also it's own father.
@pre root != NULL, current_node != NULL, root and current_node must be part of the same tree.
*/
void ass_find_father(tAssPtr root, tAssPtr current_node, tAssPtr *father_node);

/**
Evaluate data types of node in the tree.
@param root Root of the evaluated tree.
@returns Final data type, TYPE_ERR if data types don't match, DIVISION_BY_ZERO if division by zero.
@pre root != NULL
*/
int ass_check_data_types(tAssPtr root, tSymPtr sym);

int ass_evaluate_types(tAssPtr left_node, tAssPtr right_node, char *operator, tSymPtr sym);



#endif

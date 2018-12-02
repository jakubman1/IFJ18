/**
 * @file ass.c
 * @author Jakub Man
 * @date 22.11. 2018
 * @brief Abstract syntax tree library (A.S.S. -> abstraktni syntakticky strom)
 */

#include "ass.h"


void ass_init(tAssPtr *root)
{
  if(root != NULL) {
    *root = NULL;
  }
}

/*int ass_insert(tAssPtr *root, int type, char *text)
{
  if(root != NULL) {
    // Create a new element
    tAssPtr new = malloc(sizeof(struct assNode));
    if(new != NULL) {
      new->type = type;
      if(text != NULL) {
        new->text = malloc(sizeof(char) * (strlen(text) + 1));
        if(new->text != NULL) {
          strcpy(new->text, text);
        }
        else {
          free(new);
          return INTERNAL_ERR;
        }
      }
      else {
        new->text = NULL;
      }
      // We have a new element, lets insert it
      if(*root == NULL) {
        // The tree was empty
        new->lptr = NULL;
        new->rptr = NULL;
        *root = new;
      }
      // TODO: Insert cases when tree is not empty
    }
    else {
      return INTERNAL_ERR;
    }
  }

  return SUCCESS;
}*/

void ass_empty(tAssPtr *root)
{
  if(root != NULL && *root != NULL) {
    ass_empty(&((*root)->lptr)); // Remove left subtree
    ass_empty(&((*root)->rptr)); // Remove right subtree
    if((*root)->text != NULL) {
      free((*root)->text);
      (*root)->text = NULL;
    }
    free(*root); // Remove current node
    *root = NULL;
  }
}

tAssPtr ass_make_tree(int type, char *text, tSymPtr symtable, tAssPtr left, tAssPtr right)
{
  fprintf(stderr, "ZACATEK MAKE TREE\n");
  tAssPtr new = ass_make_leaf(type, text, symtable);
  if(new != NULL) {
    new->lptr = left;
    new->rptr = right;

  }
  fprintf(stderr, "KONEC MAKE TREE\n");
  return new;
}

tAssPtr ass_make_leaf(int type, char *text, tSymPtr symtable)
{
  fprintf(stderr, "ZACATEK MAKE LEAF\n");
  tAssPtr new = malloc(sizeof(struct assNode));
  if(new != NULL) {
    new->type = type;
    new->symtable = symtable;
    if(text != NULL) {
      new->text = malloc(sizeof(char) * (strlen(text) + 1));
      if(new->text != NULL) {
        strcpy(new->text, text);
      }
      else {
        // Text memory allocation failed.
        free(new);
        return NULL;
      }
    }
    else {
      new->text = NULL;
    }
    // We have a new element
    new->lptr = NULL;
    new->rptr = NULL;
    fprintf(stderr, "KONEC MAKE LEAF\n");
    return new;
  }
  else {
    return NULL;
  }
}

tAssPtr ass_find_father(tAssPtr root, tAssPtr current_node)
{
  fprintf(stderr, "ZACATEK FIND_FATHER\n");
  if (root == NULL) {
    fprintf(stderr, "KONEC FIND_FATHER\n");
    return NULL;
  }

  if (ass_find_father(root->rptr, current_node) == current_node) {
    fprintf(stderr, "KONEC FIND_FATHER\n");
    return root;
  }

  if (ass_find_father(root->lptr, current_node) == current_node) {
    fprintf(stderr, "KONEC FIND_FATHER\n");
    return root;
  }
  fprintf(stderr, "KONEC FIND_FATHER\n");
  return NULL;
}

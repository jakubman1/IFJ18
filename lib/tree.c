#include "tree.h"


void dtree_init(tDerTreePtr *root)
{
  if(*root != NULL) {
    *root = NULL;
  }

}

int dtree_insert(tDerTreePtr *root, int type, char *text)
{
  if(root != NULL) {
    tDerTreePtr new = malloc(sizeof(struct der_tree_node));
    if(new != NULL) {
      new->type = type;
      if(text != NULL) {
        new->text = malloc(sizeof(char) * strlen(text));
        if(new->text != NULL) {
          strcpy(new->text, text);
        }
        else {
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
    }
    else {
      return INTERNAL_ERR;
    }
  }

  return SUCCESS;
}

void dtree_empty(tDerTreePtr *root)
{
  if(root != NULL && *root != NULL) {
    dtree_empty(&((*root)->lptr)); // Remove left subtree
    dtree_empty(&((*root)->rptr)); // Remove right subtree
    free(*root); // Remove current node
    *root = NULL;
  }
}
